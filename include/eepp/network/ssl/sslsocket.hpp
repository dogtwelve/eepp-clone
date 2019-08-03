#ifndef EE_NETWORKCSSLSOCKET_HPP
#define EE_NETWORKCSSLSOCKET_HPP

#include <eepp/network/tcpsocket.hpp>

namespace EE { namespace Network { namespace SSL {

class SSLSocketImpl;

class EE_API SSLSocket : public TcpSocket {
	public:
		static std::string CertificatesPath;

		static bool init();
		
		static bool end();

		/** @return True when the library was compiled with SSL support. */
		static bool isSupported();

		SSLSocket( std::string hostname, bool validateCertificate, bool validateHostname );

		virtual ~SSLSocket();

		Status connect(const IpAddress& remoteAddress, unsigned short remotePort, Time timeout = Time::Zero);

		void disconnect();

		Status send(const void* data, std::size_t size);

		Status receive(void* data, std::size_t size, std::size_t& received);

		Status send(Packet& packet);

		Status receive(Packet& packet);
	protected:
		friend class SSLSocketImpl;
		friend class OpenSSLSocket;
		
		SSLSocketImpl *			mImpl;
		std::string				mHostName;
		bool					mValidateCertificate;
		bool					mValidateHostname;
};

}}}

#endif
