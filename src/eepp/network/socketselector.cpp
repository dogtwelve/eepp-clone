#include <eepp/network/socketselector.hpp>
#include <eepp/network/socket.hpp>
#include <eepp/network/platform/platformimpl.hpp>
#include <algorithm>
#include <utility>

#if EE_PLATFORM == EE_PLATFORM_HAIKU
#include <sys/select.h>
#endif

#ifdef _MSC_VER
	#pragma warning(disable : 4127) // "conditional expression is constant" generated by the FD_SET macro
#endif

namespace EE { namespace Network {

struct SocketSelector::SocketSelectorImpl {
	fd_set AllSockets;   ///< Set containing all the sockets handles
	fd_set SocketsReady; ///< Set containing handles of the sockets that are ready
	int MaxSocket;	///< Maximum socket handle
	int SocketCount;  ///< Number of socket handles
};

SocketSelector::SocketSelector() :
	mImpl( eeNew( SocketSelectorImpl, () ) )
{
	clear();
}

SocketSelector::SocketSelector(const SocketSelector& copy) :
	mImpl( eeNew( SocketSelectorImpl, (*copy.mImpl) ) )
{
}

SocketSelector::~SocketSelector() {
	eeSAFE_DELETE( mImpl );
}

void SocketSelector::add(Socket& socket) {
	SocketHandle handle = socket.getHandle();

	if (handle != Private::SocketImpl::invalidSocket()) {
#if EE_PLATFORM == EE_PLATFORM_WIN
		if (mImpl->SocketCount >= FD_SETSIZE) {
			eePRINT( "The socket can't be added to the selector because its " );
			eePRINT( "ID is too high. This is a limitation of your operating " );
			eePRINT( "system's FD_SETSIZE setting." );
			return;
		}

		if (FD_ISSET(handle, &mImpl->AllSockets))
			return;

		mImpl->SocketCount++;
#else
		if (handle >= FD_SETSIZE) {
			eePRINT( "The socket can't be added to the selector because its " );
			eePRINT( "ID is too high. This is a limitation of your operating " );
			eePRINT( "system's FD_SETSIZE setting." );
			return;
		}

		// SocketHandle is an int in POSIX
		mImpl->MaxSocket = std::max(mImpl->MaxSocket, handle);
#endif

		FD_SET(handle, &mImpl->AllSockets);
	}
}

void SocketSelector::remove(Socket& socket) {
	SocketHandle handle = socket.getHandle();

	if (handle != Private::SocketImpl::invalidSocket()) {
#if EE_PLATFORM == EE_PLATFORM_WIN
		if (!FD_ISSET(handle, &mImpl->AllSockets))
			return;

		mImpl->SocketCount--;
#else
		if (handle >= FD_SETSIZE)
			return;
#endif

		FD_CLR(handle, &mImpl->AllSockets);
		FD_CLR(handle, &mImpl->SocketsReady);
	}
}

void SocketSelector::clear() {
	FD_ZERO(&mImpl->AllSockets);
	FD_ZERO(&mImpl->SocketsReady);

	mImpl->MaxSocket = 0;
	mImpl->SocketCount = 0;
}

bool SocketSelector::wait(Time timeout) {
	// Setup the timeout
	timeval time;
	time.tv_sec  = static_cast<long>(timeout.asMicroseconds() / 1000000);
	time.tv_usec = static_cast<long>(timeout.asMicroseconds() % 1000000);

	// Initialize the set that will contain the sockets that are ready
	mImpl->SocketsReady = mImpl->AllSockets;

	// Wait until one of the sockets is ready for reading, or timeout is reached
	// The first parameter is ignored on Windows
	int count = select(mImpl->MaxSocket + 1, &mImpl->SocketsReady, NULL, NULL, timeout != Time::Zero ? &time : NULL);

	return count > 0;
}

bool SocketSelector::isReady(Socket& socket) const {
	SocketHandle handle = socket.getHandle();

	if (handle != Private::SocketImpl::invalidSocket()) {
#if EE_PLATFORM == EE_PLATFORM_WIN
		if (handle >= FD_SETSIZE)
			return false;
#endif

		return FD_ISSET(handle, &mImpl->SocketsReady) != 0;
	}

	return false;
}

SocketSelector& SocketSelector::operator =(const SocketSelector& right) {
	SocketSelector temp(right);

	std::swap(mImpl, temp.mImpl);

	return *this;
}

}}
