#ifndef EE_NETWORKCHTTP_HPP
#define EE_NETWORKCHTTP_HPP

#include <eepp/network/base.hpp>
#include <eepp/network/ipaddress.hpp>
#include <eepp/network/tcpsocket.hpp>
#include <eepp/core/noncopyable.hpp>
#include <eepp/system/time.hpp>
#include <eepp/system/threadlocalptr.hpp>
#include <eepp/system/thread.hpp>
#include <eepp/system/mutex.hpp>
#include <eepp/system/lock.hpp>
#include <eepp/helper/PlusCallback/callback.hpp>
#include <map>
#include <string>
#include <list>

using namespace EE::System;

namespace EE { namespace Network {

/** @brief A HTTP client */
class EE_API Http : NonCopyable {
	public :
		/** @brief Define a HTTP request */
		class EE_API Request {
			public :
			/** @brief Enumerate the available HTTP methods for a request */
			enum Method {
				Get,	///< Request in get mode, standard method to retrieve a page
				Post,	///< Request in post mode, usually to send data to a page
				Head,	///< Request a page's header only
				Put,	///< Request in put mode, useful for a REST API
				Delete	///< Request in delete mode, useful for a REST API
			};

			/** @brief Default constructor
			**  This constructor creates a GET request, with the root
			**  URI ("/") and an empty body.
			**  @param uri	Target URI
			**  @param method Method to use for the request
			**  @param body   Content of the request's body
			**  @param validateCertificate Enables certificate validation for https request
			**  @param validateHostname Enables hostname validation for https request */
			Request(const std::string& uri = "/", Method method = Get, const std::string& body = "", bool validateCertificate = false, bool validateHostname = false );

			/** @brief Set the value of a field
			**  The field is created if it doesn't exist. The name of
			**  the field is case insensitive.
			**  By default, a request doesn't contain any field (but the
			**  mandatory fields are added later by the HTTP client when
			**  sending the request).
			**  @param field Name of the field to set
			**  @param value Value of the field */
			void SetField(const std::string& field, const std::string& value);

			/** @brief Set the request method
			**  See the Method enumeration for a complete list of all
			**  the availale methods.
			**  The method is Http::Request::Get by default.
			**  @param method Method to use for the request */
			void SetMethod(Method method);

			/** @brief Set the requested URI
			**  The URI is the resource (usually a web page or a file)
			**  that you want to get or post.
			**  The URI is "/" (the root page) by default.
			**  @param uri URI to request, relative to the host */
			void SetUri(const std::string& uri);

			/** @brief Set the HTTP version for the request
			**  The HTTP version is 1.0 by default.
			**  @param major Major HTTP version number
			**  @param minor Minor HTTP version number */
			void SetHttpVersion(unsigned int major, unsigned int minor);

			/** @brief Set the body of the request
			**  The body of a request is optional and only makes sense
			**  for POST requests. It is ignored for all other methods.
			**  The body is empty by default.
			**  @param body Content of the body */
			void SetBody(const std::string& body);

			/** @return The request Uri */
			const std::string& GetUri() const;

			/** @return If SSL certificate validation is enabled */
			const bool& ValidateCertificate() const;

			/** Enable/disable SSL certificate validation */
			void ValidateCertificate( bool enable );

			/** @return If SSL hostname validation is enabled */
			const bool& ValidateHostname() const;

			/** Enable/disable SSL hostname validation */
			void ValidateHostname( bool enable );
		private:
			friend class Http;

			/** @brief Prepare the final request to send to the server
			**  This is used internally by Http before sending the
			**  request to the web server.
			**  @return String containing the request, ready to be sent */
			std::string Prepare() const;

			/** @brief Check if the request defines a field
			**  This function uses case-insensitive comparisons.
			**  @param field Name of the field to test
			**  @return True if the field exists, false otherwise */
			bool HasField(const std::string& field) const;

			// Types
			typedef std::map<std::string, std::string> FieldTable;

			// Member data
			FieldTable		mFields;				///< Fields of the header associated to their value
			Method			mMethod;				///< Method to use for the request
			std::string		mUri;					///< Target URI of the request
			unsigned int	mMajorVersion;			///< Major HTTP version
			unsigned int	mMinorVersion;			///< Minor HTTP version
			std::string		mBody;					///< Body of the request
			bool			mValidateCertificate;	///< Validates the SSL certificate in case of an HTTPS request
			bool			mValidateHostname;		///< Validates the hostname in case of an HTTPS request
		};

		/** @brief Define a HTTP response */
		class EE_API Response {
			public:

			/** @brief Enumerate all the valid status codes for a response */
			enum Status {
				// 2xx: success
				Ok			 = 200, ///< Most common code returned when operation was successful
				Created		= 201, ///< The resource has successfully been created
				Accepted	   = 202, ///< The request has been accepted, but will be processed later by the server
				NoContent	  = 204, ///< The server didn't send any data in return
				ResetContent   = 205, ///< The server informs the client that it should clear the view (form) that caused the request to be sent
				PartialContent = 206, ///< The server has sent a part of the resource, as a response to a partial GET request

				// 3xx: redirection
				MultipleChoices  = 300, ///< The requested page can be accessed from several locations
				MovedPermanently = 301, ///< The requested page has permanently moved to a new location
				MovedTemporarily = 302, ///< The requested page has temporarily moved to a new location
				NotModified	  = 304, ///< For conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

				// 4xx: client error
				BadRequest		  = 400, ///< The server couldn't understand the request (syntax error)
				Unauthorized		= 401, ///< The requested page needs an authentification to be accessed
				Forbidden		   = 403, ///< The requested page cannot be accessed at all, even with authentification
				NotFound			= 404, ///< The requested page doesn't exist
				RangeNotSatisfiable = 407, ///< The server can't satisfy the partial GET request (with a "Range" header field)

				// 5xx: server error
				InternalServerError = 500, ///< The server encountered an unexpected error
				NotImplemented	  = 501, ///< The server doesn't implement a requested feature
				BadGateway		  = 502, ///< The gateway server has received an error from the source server
				ServiceNotAvailable = 503, ///< The server is temporarily unavailable (overloaded, in maintenance, ...)
				GatewayTimeout	  = 504, ///< The gateway server couldn't receive a response from the source server
				VersionNotSupported = 505, ///< The server doesn't support the requested HTTP version

				// 10xx: Custom codes
				InvalidResponse  = 1000, ///< Response is not a valid HTTP one
				ConnectionFailed = 1001  ///< Connection with server failed
			};

			/** @brief Default constructor
			**  Constructs an empty response. */
			Response();

			/** @brief Get the value of a field
			**  If the field @a field is not found in the response header,
			**  the empty string is returned. This function uses
			**  case-insensitive comparisons.
			**  @param field Name of the field to get
			**  @return Value of the field, or empty string if not found */
			const std::string& GetField(const std::string& field) const;

			/** @brief Get the response status code
			**  The status code should be the first thing to be checked
			**  after receiving a response, it defines whether it is a
			**  success, a failure or anything else (see the Status
			**  enumeration).
			**  @return Status code of the response */
			Status GetStatus() const;

			/** @brief Get the major HTTP version number of the response
			**  @return Major HTTP version number
			**  @see GetMinorHttpVersion */
			unsigned int GetMajorHttpVersion() const;

			/** @brief Get the minor HTTP version number of the response
			**  @return Minor HTTP version number
			**  @see GetMajorHttpVersion */
			unsigned int GetMinorHttpVersion() const;

			/** @brief Get the body of the response
			**  The body of a response may contain:
			**  @li the requested page (for GET requests)
			**  @li a response from the server (for POST requests)
			**  @li nothing (for HEAD requests)
			**  @li an error message (in case of an error)
			**  @return The response body */
			const std::string& GetBody() const;
			private :
			friend class Http;

			/** @brief Construct the header from a response string
			**  This function is used by Http to build the response
			**  of a request.
			**  @param data Content of the response to parse */
			void Parse(const std::string& data);

			/** @brief Read values passed in the answer header
			**  This function is used by Http to extract values passed
			**  in the response.
			**  @param in String stream containing the header values */
			void ParseFields(std::istream &in);

			// Types
			typedef std::map<std::string, std::string> FieldTable;

			// Member data
			FieldTable		mFields;		///< Fields of the header
			Status			mStatus;		///< Status code
			unsigned int	mMajorVersion;	///< Major HTTP version
			unsigned int	mMinorVersion;	///< Minor HTTP version
			std::string		mBody;			///< Body of the response
		};

		/** @brief Default constructor */
		Http();

		/** @brief Construct the HTTP client with the target host
		**  This is equivalent to calling SetHost(host, port).
		**  The port has a default value of 0, which means that the
		**  HTTP client will use the right port according to the
		**  protocol used (80 for HTTP, 443 for HTTPS). You should
		**  leave it like this unless you really need a port other
		**  than the standard one, or use an unknown protocol.
		**  @param host Web server to connect to
		**  @param port Port to use for connection
		**	@param useSSL force the SSL usage ( if compiled with the support of it ). If the host starts with https:// it will use it by default. */
		Http(const std::string& host, unsigned short port = 0, bool useSSL = false);

		~Http();

		/** @brief Set the target host
		**  This function just stores the host address and port, it
		**  doesn't actually connect to it until you send a request.
		**  The port has a default value of 0, which means that the
		**  HTTP client will use the right port according to the
		**  protocol used (80 for HTTP, 443 for HTTPS). You should
		**  leave it like this unless you really need a port other
		**  than the standard one, or use an unknown protocol.
		**  @param host Web server to connect to
		**  @param port Port to use for connection
		**	@param useSSL force the SSL usage ( if compiled with the support of it ). If the host starts with https:// it will use it by default. */
		void SetHost(const std::string& host, unsigned short port = 0, bool useSSL = false);

		/** @brief Send a HTTP request and return the server's response.
		**  You must have a valid host before sending a request (see SetHost).
		**  Any missing mandatory header field in the request will be added
		**  with an appropriate value.
		**  Warning: this function waits for the server's response and may
		**  not return instantly; use a thread if you don't want to block your
		**  application, or use a timeout to limit the time to wait. A value
		**  of Time::Zero means that the client will use the system defaut timeout
		**  (which is usually pretty long).
		**  @param request Request to send
		**  @param timeout Maximum time to wait
		**  @return Server's response */
		Response SendRequest(const Request& request, Time timeout = Time::Zero);

		/** Definition of the async callback response */
		typedef cb::Callback3<void, const Http&, Http::Request&, Http::Response&>		AsyncResponseCallback;

		/** @brief Sends the request and creates a new thread, when got the response informs the result to the callback.
		**	This function does not lock the caller thread.
		**  @see SendRequest */
		void SendAsyncRequest( AsyncResponseCallback cb, const Http::Request& request, Time timeout = Time::Zero );

		/** @return The host address */
		const IpAddress& GetHost() const;

		/** @return The host name */
		const std::string& GetHostName() const;

		/** @return The host port */
		const unsigned short& GetPort() const;
	private:
		class cAsyncRequest : public Thread {
			public:
				cAsyncRequest( Http * http, AsyncResponseCallback cb, Http::Request request, Time timeout );

				void Run();
			protected:
				friend class Http;
				Http *					mHttp;
				AsyncResponseCallback	mCb;
				Http::Request			mRequest;
				Time					mTimeout;
				bool					mRunning;
		};
		friend class cAsyncRequest;
		ThreadLocalPtr<TcpSocket>		mConnection;	///< Connection to the host
		IpAddress						mHost;			///< Web host address
		std::string						mHostName;		///< Web host name
		unsigned short					mPort;			///< Port used for connection with host
		std::list<cAsyncRequest*>		mThreads;
		Mutex							mThreadsMutex;
		bool							mIsSSL;

		void RemoveOldThreads();
};

}}

#endif // EE_NETWORKCHTTP_HPP

/**
@class Http
@ingroup Network
Http is a very simple HTTP client that allows you
to communicate with a web server. You can retrieve
web pages, send data to an interactive resource,
download a remote file, etc.
The HTTP client is split into 3 classes:
@li Http::Request
@li Http::Response
@li Http
Http::Request builds the request that will be
sent to the server. A request is made of:
@li a method (what you want to do)
@li a target URI (usually the name of the web page or file)
@li one or more header fields (options that you can pass to the server)
@li an optional body (for POST requests)
Http::Response parse the response from the web server
and provides getters to read them. The response contains:
@li a status code
@li header fields (that may be answers to the ones that you requested)
@li a body, which contains the contents of the requested resource
Http provides a simple function, SendRequest, to send a
Http::Request and return the corresponding Http::Response
from the server.
Usage example:
@code
// Create a new HTTP client
Http http;

// We'll work on http://www.google.com
http.SetHost("http://www.google.com");

// Prepare a request to get the 'features.php' page
Http::Request request("features.php");

// Send the request
Http::Response response = http.SendRequest(request);

// Check the status code and display the result
Http::Response::Status status = response.GetStatus();
if (status == Http::Response::Ok)
{
	std::cout << response.GetBody() << std::endl;
}
else
{
	std::cout << "Error " << status << std::endl;
}
@endcode
*/
