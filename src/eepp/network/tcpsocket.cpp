#include <eepp/network/tcpsocket.hpp>
#include <eepp/network/ipaddress.hpp>
#include <eepp/network/packet.hpp>
#include <eepp/network/platform/platformimpl.hpp>
#include <algorithm>
#include <cstring>

#ifdef _MSC_VER
	#pragma warning(disable : 4127) // "conditional expression is constant" generated by the FD_SET macro
#endif

namespace
{
	// Define the low-level send/receive flags, which depend on the OS
	#if EE_PLATFORM == EE_PLATFORM_LINUX
		const int flags = MSG_NOSIGNAL;
	#else
		const int flags = 0;
	#endif
}

namespace EE { namespace Network {

TcpSocket::TcpSocket() :
	Socket(Tcp)
{
}

unsigned short TcpSocket::getLocalPort() const {
	if (getHandle() != Private::SocketImpl::invalidSocket()) {
		// Retrieve informations about the local end of the socket
		sockaddr_in address;
		Private::SocketImpl::AddrLength size = sizeof(address);
		if (getsockname(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1) {
			return ntohs(address.sin_port);
		}
	}

	// We failed to retrieve the port
	return 0;
}

IpAddress TcpSocket::getRemoteAddress() const {
	if (getHandle() != Private::SocketImpl::invalidSocket()) {
		// Retrieve informations about the remote end of the socket
		sockaddr_in address;
		Private::SocketImpl::AddrLength size = sizeof(address);
		if (getpeername(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1) {
			return IpAddress(ntohl(address.sin_addr.s_addr));
		}
	}

	// We failed to retrieve the address
	return IpAddress::None;
}

unsigned short TcpSocket::getRemotePort() const {
	if (getHandle() != Private::SocketImpl::invalidSocket()) {
		// Retrieve informations about the remote end of the socket
		sockaddr_in address;
		Private::SocketImpl::AddrLength size = sizeof(address);
		if (getpeername(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1) {
			return ntohs(address.sin_port);
		}
	}

	// We failed to retrieve the port
	return 0;
}

Socket::Status TcpSocket::connect(const IpAddress& remoteAddress, unsigned short remotePort, Time timeout) {
	// Create the internal socket if it doesn't exist
	create();

	// Create the remote address
	sockaddr_in address = Private::SocketImpl::createAddress(remoteAddress.toInteger(), remotePort);

	if (timeout <= Time::Zero) {
		// ----- We're not using a timeout: just try to connect -----

		// Connect the socket
		if (::connect(getHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
			return Private::SocketImpl::getErrorStatus();

		// Connection succeeded
		return Done;
	} else {
		// ----- We're using a timeout: we'll need a few tricks to make it work -----

		// Save the previous blocking state
		bool blocking = isBlocking();

		// Switch to non-blocking to enable our connection timeout
		if (blocking)
			setBlocking(false);

		// Try to connect to the remote address
		if (::connect(getHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) >= 0) {
			// We got instantly connected! (it may no happen a lot...)
			setBlocking(blocking);
			return Done;
		}

		// Get the error status
		Status status = Private::SocketImpl::getErrorStatus();

		// If we were in non-blocking mode, return immediatly
		if (!blocking)
			return status;

		// Otherwise, wait until something happens to our socket (success, timeout or error)
		if (status == Socket::NotReady) {
			// Setup the selector
			fd_set selector;
			FD_ZERO(&selector);
			FD_SET(getHandle(), &selector);

			// Setup the timeout
			timeval time;
			time.tv_sec  = static_cast<long>(timeout.asMicroseconds() / 1000000);
			time.tv_usec = static_cast<long>(timeout.asMicroseconds() % 1000000);

			// Wait for something to write on our socket (which means that the connection request has returned)
			if (select(static_cast<int>(getHandle() + 1), NULL, &selector, NULL, &time) > 0) {
				// At this point the connection may have been either accepted or refused.
				// To know whether it's a success or a failure, we must check the address of the connected peer
				if (getRemoteAddress() != IpAddress::None) {
					// Connection accepted
					status = Done;
				} else {
					// Connection refused
					status = Private::SocketImpl::getErrorStatus();
				}
			} else {
				// Failed to connect before timeout is over
				status = Private::SocketImpl::getErrorStatus();
			}
		}

		// Switch back to blocking mode
		setBlocking(true);

		return status;
	}
}

void TcpSocket::disconnect() {
	// Close the socket
	close();

	// Reset the pending packet data
	mPendingPacket = PendingPacket();
}

Socket::Status TcpSocket::send(const void* data, std::size_t size) {
	if ( !isBlocking() )
		eePRINTL( "Warning: Partial sends might not be handled properly." );

	std::size_t sent;

	return send(data, size, sent);
}

Socket::Status TcpSocket::send(const void* data, std::size_t size, std::size_t& sent) {
	// Check the parameters
	if (!data || (size == 0)) {
		eePRINTL( "Cannot send data over the network (no data to send)" );
		return Error;
	}

	// Loop until every byte has been sent
	int result = 0;
	for (sent = 0; sent < size; sent += result) {
		// Send a chunk of data
		result = ::send(getHandle(), static_cast<const char*>(data) + sent, size - sent, flags);

		// Check for errors
		if (result < 0) {
			Status status = Private::SocketImpl::getErrorStatus();

			if ((status == NotReady) && sent) {
				return Partial;
			}

			return status;
		}
	}

	return Done;
}

Socket::Status TcpSocket::receive(void* data, std::size_t size, std::size_t& received) {
	// First clear the variables to fill
	received = 0;

	// Check the destination buffer
	if (!data) {
		eePRINTL( "Cannot receive data from the network (the destination buffer is invalid)" );
		return Error;
	}

	// Receive a chunk of bytes
	int sizeReceived = recv(getHandle(), static_cast<char*>(data), static_cast<int>(size), flags);

	// Check the number of bytes received
	if (sizeReceived > 0) {
		received = static_cast<std::size_t>(sizeReceived);
		return Done;
	} else if (sizeReceived == 0) {
		return Socket::Disconnected;
	} else {
		return Private::SocketImpl::getErrorStatus();
	}
}

Socket::Status TcpSocket::send(Packet& packet) {
	// TCP is a stream protocol, it doesn't preserve messages boundaries.
	// This means that we have to send the packet size first, so that the
	// receiver knows the actual end of the packet in the data stream.

	// We allocate an extra memory block so that the size can be sent
	// together with the data in a single call. This may seem inefficient,
	// but it is actually required to avoid partial send, which could cause
	// data corruption on the receiving end.

	// Get the data to send from the packet
	std::size_t size = 0;
	const void* data = packet.onSend(size);

	// First convert the packet size to network byte order
	Uint32 packetSize = htonl(static_cast<Uint32>(size));
 
	// Allocate memory for the data block to send
	std::vector<char> blockToSend(sizeof(packetSize) + size);
 
	// Copy the packet size and data into the block to send
	std::memcpy(&blockToSend[0], &packetSize, sizeof(packetSize));
	if (size > 0)
		std::memcpy(&blockToSend[0] + sizeof(packetSize), data, size);

	// Send the data block
	std::size_t sent;
	Status status = send(&blockToSend[0] + packet.mSendPos, blockToSend.size() - packet.mSendPos, sent );

	// In the case of a partial send, record the location to resume from
	if (status == Partial) {
		packet.mSendPos += sent;
	} else if (status == Done) {
		packet.mSendPos = 0;
	}

	return status;
}

Socket::Status TcpSocket::receive(Packet& packet) {
	// First clear the variables to fill
	packet.clear();

	// We start by getting the size of the incoming packet
	Uint32 packetSize = 0;
	std::size_t received = 0;
	if (mPendingPacket.SizeReceived < sizeof(mPendingPacket.Size)) {
		// Loop until we've received the entire size of the packet
		// (even a 4 byte variable may be received in more than one call)
		while (mPendingPacket.SizeReceived < sizeof(mPendingPacket.Size)) {
			char* data = reinterpret_cast<char*>(&mPendingPacket.Size) + mPendingPacket.SizeReceived;
			Status status = receive(data, sizeof(mPendingPacket.Size) - mPendingPacket.SizeReceived, received);
			mPendingPacket.SizeReceived += received;

			if (status != Done)
				return status;
		}

		// The packet size has been fully received
		packetSize = ntohl(mPendingPacket.Size);
	} else {
		// The packet size has already been received in a previous call
		packetSize = ntohl(mPendingPacket.Size);
	}

	// Loop until we receive all the packet data
	char buffer[1024];
	while (mPendingPacket.Data.size() < packetSize) {
		// Receive a chunk of data
		std::size_t sizeToGet = eemin(static_cast<std::size_t>(packetSize - mPendingPacket.Data.size()), sizeof(buffer));
		Status status = receive(buffer, sizeToGet, received);
		if (status != Done)
			return status;

		// Append it into the packet
		if (received > 0) {
			mPendingPacket.Data.resize(mPendingPacket.Data.size() + received);
			char* begin = &mPendingPacket.Data[0] + mPendingPacket.Data.size() - received;
			std::memcpy(begin, buffer, received);
		}
	}

	// We have received all the packet data: we can copy it to the user packet
	if (!mPendingPacket.Data.empty())
		packet.onReceive(&mPendingPacket.Data[0], mPendingPacket.Data.size());

	// Clear the pending packet data
	mPendingPacket = PendingPacket();

	return Done;
}

TcpSocket::PendingPacket::PendingPacket() :
	Size		(0),
	SizeReceived(0),
	Data		()
{
}

}}
