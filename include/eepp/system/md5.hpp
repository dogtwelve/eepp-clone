#ifndef EE_SYSTEM_MD5_HPP
#define EE_SYSTEM_MD5_HPP

#include <eepp/system/iostream.hpp>

namespace EE { namespace System {

class MD5 {
	public:
		typedef struct {
			std::vector<Uint8> digest;

			std::string toHexString() {
				return MD5::hexDigest( digest );
			}
		} Result;

		/** @return Calculates the md5 hash from a stream */
		static Result fromStream( IOStream& stream );

		/** Calculates the md5 hash from a file */
		static Result fromFile( std::string path );

		/** Calculates the md5 hash from memory */
		static Result fromMemory( const Uint8* data, Uint64 size );

		/** Calculates the md5 hash from a string */
		static Result fromString( const std::string& str );

		/** Calculates the md5 hash from a string */
		static Result fromString( const String& str );
	protected:
		typedef unsigned int MD5_u32plus;

		typedef struct {
			MD5_u32plus lo, hi;
			MD5_u32plus a, b, c, d;
			unsigned char buffer[64];
			MD5_u32plus block[16];
		} Context;

		static const void * body( Context *ctx, const void *data, unsigned long size );

		static void init( Context * ctx );

		static void update( Context * ctx, const void *data, unsigned long size );

		static void final( unsigned char *result, Context * ctx );

		static std::string hexDigest( std::vector<Uint8>& digest );
};

}}

#endif
