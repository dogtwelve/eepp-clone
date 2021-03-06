#ifndef EE_NETWORKCIPADDRESS_HPP
#define EE_NETWORKCIPADDRESS_HPP

#include <eepp/network/base.hpp>
#include <eepp/system/time.hpp>
using namespace EE::System;

#include <istream>
#include <ostream>
#include <string>

namespace EE { namespace Network {

/** @brief Encapsulate an IPv4 network address */
class EE_API IpAddress
{
	public:
		/** @brief Default constructor
		**  This constructor creates an empty (invalid) address */
		IpAddress();

		/** @brief Construct the address from a string
		**  Here @a address can be either a decimal address
		**  (ex: "192.168.1.56") or a network name (ex: "localhost").
		**  @param address IP address or network name */
		IpAddress(const std::string& address);

		/** @brief Construct the address from a string
		**  Here @a address can be either a decimal address
		**  (ex: "192.168.1.56") or a network name (ex: "localhost").
		**  This is equivalent to the constructor taking a std::string
		**  parameter, it is defined for convenience so that the
		**  implicit conversions from literal strings to IpAddress work.
		**  @param address IP address or network name */
		IpAddress(const char* address);

		/** @brief Construct the address from 4 bytes
		**  Calling IpAddress(a, b, c, d) is equivalent to calling
		**  IpAddress("a.b.c.d"), but safer as it doesn't have to
		**  parse a string to get the address components.
		**  @param byte0 First byte of the address
		**  @param byte1 Second byte of the address
		**  @param byte2 Third byte of the address
		**  @param byte3 Fourth byte of the address */
		IpAddress(Uint8 byte0, Uint8 byte1, Uint8 byte2, Uint8 byte3);

		/** @brief Construct the address from a 32-bits integer
		**  This constructor uses the internal representation of
		**  the address directly. It should be used for optimization
		**  purposes, and only if you got that representation from
		**  IpAddress::ToInteger().
		**  @param address 4 bytes of the address packed into a 32-bits integer
		**  @see ToInteger */
		explicit IpAddress(Uint32 address);

		/** @brief Get a string representation of the address
		**  The returned string is the decimal representation of the
		**  IP address (like "192.168.1.56"), even if it was constructed
		**  from a host name.
		**  @return String representation of the address
		**  @see ToInteger */
		std::string ToString() const;

		/** @brief Get an integer representation of the address
		**  The returned number is the internal representation of the
		**  address, and should be used for optimization purposes only
		**  (like sending the address through a socket).
		**  The integer produced by this function can then be converted
		**  back to a IpAddress with the proper constructor.
		**  @return 32-bits unsigned integer representation of the address
		**  @see ToString */
		Uint32 ToInteger() const;

		/** @brief Get the computer's local address
		**  The local address is the address of the computer from the
		**  LAN point of view, i.e. something like 192.168.1.56. It is
		**  meaningful only for communications over the local network.
		**  Unlike GetPublicAddress, this function is fast and may be
		**  used safely anywhere.
		**  @return Local IP address of the computer
		**  @see GetPublicAddress */
		static IpAddress GetLocalAddress();

		/** @brief Get the computer's public address
		**  The public address is the address of the computer from the
		**  internet point of view, i.e. something like 89.54.1.169.
		**  It is necessary for communications over the world wide web.
		**  The only way to get a public address is to ask it to a
		**  distant website; as a consequence, this function depends on
		**  both your network connection and the server, and may be
		**  very slow. You should use it as few as possible. Because
		**  this function depends on the network connection and on a distant
		**  server, you may use a time limit if you don't want your program
		**  to be possibly stuck waiting in case there is a problem; this
		**  limit is deactivated by default.
		**  @param timeout Maximum time to wait
		**  @return Public IP address of the computer
		**  @see GetLocalAddress */
		static IpAddress GetPublicAddress(Time timeout = Time::Zero);

		// Static member data
		static const IpAddress None;	  ///< Value representing an empty/invalid address
		static const IpAddress LocalHost; ///< The "localhost" address (for connecting a computer to itself locally)
		static const IpAddress Broadcast; ///< The "broadcast" address (for sending UDP messages to everyone on a local network)
	private :
		// Member data
		Uint32 mAddress; ///< Address stored as an unsigned 32 bits integer
};

/** @brief Overload of == operator to compare two IP addresses
**  @param left  Left operand (a IP address)
**  @param right Right operand (a IP address)
**  @return True if both addresses are equal */
EE_API bool operator ==(const IpAddress& left, const IpAddress& right);

/** @brief Overload of != operator to compare two IP addresses
**  @param left  Left operand (a IP address)
**  @param right Right operand (a IP address)
**  @return True if both addresses are different */
EE_API bool operator !=(const IpAddress& left, const IpAddress& right);

/** @brief Overload of < operator to compare two IP addresses
**  @param left  Left operand (a IP address)
**  @param right Right operand (a IP address)
**  @return True if @a left is lesser than @a right */
EE_API bool operator <(const IpAddress& left, const IpAddress& right);

/** @brief Overload of > operator to compare two IP addresses
**  @param left  Left operand (a IP address)
**  @param right Right operand (a IP address)
**  @return True if @a left is greater than @a right */
EE_API bool operator >(const IpAddress& left, const IpAddress& right);

/** @brief Overload of <= operator to compare two IP addresses
**  @param left  Left operand (a IP address)
**  @param right Right operand (a IP address)
**  @return True if @a left is lesser or equal than @a right */
EE_API bool operator <=(const IpAddress& left, const IpAddress& right);

/** @brief Overload of >= operator to compare two IP addresses
**  @param left  Left operand (a IP address)
**  @param right Right operand (a IP address)
**  @return True if @a left is greater or equal than @a right */
EE_API bool operator >=(const IpAddress& left, const IpAddress& right);

/** @brief Overload of >> operator to extract an IP address from an input stream
**  @param stream  Input stream
**  @param address IP address to extract
**  @return Reference to the input stream */
EE_API std::istream& operator >>(std::istream& stream, IpAddress& address);

/** @brief Overload of << operator to print an IP address to an output stream
**  @param stream  Output stream
**  @param address IP address to print
**  @return Reference to the output stream */
EE_API std::ostream& operator <<(std::ostream& stream, const IpAddress& address);

}}

#endif // EE_NETWORKCIPADDRESS_HPP

/**
@class IpAddress
@ingroup Network
IpAddress is a utility class for manipulating network
addresses. It provides a set a implicit constructors and
conversion functions to easily build or transform an IP
address from/to various representations.

Usage example:
@code
IpAddress a0;									 // an invalid address
IpAddress a1 = IpAddress::None;			   // an invalid address (same as a0)
IpAddress a2("127.0.0.1");						// the local host address
IpAddress a3 = IpAddress::Broadcast;		  // the broadcast address
IpAddress a4(192, 168, 1, 56);					// a local address
IpAddress a5("my_computer");					  // a local address created from a network name
IpAddress a6("89.54.1.169");					  // a distant address
IpAddress a7("www.google.com");				   // a distant address created from a network name
IpAddress a8 = IpAddress::GetLocalAddress();  // my address on the local network
IpAddress a9 = IpAddress::GetPublicAddress(); // my address on the internet
@endcode
Note that IpAddress currently doesn't support IPv6
nor other types of network addresses.
*/
