#include <eepp/window/ccursor.hpp>

namespace EE { namespace Window {

cCursor::cCursor( cTexture * tex, const eeVector2i& hotspot, const std::string& name, Window::cWindow * window ) :
	mId( String::Hash( name ) ),
	mName( name ),
	mImage( NULL ),
	mHotSpot( hotspot ),
	mWindow( window )
{
	if ( NULL != tex && tex->Lock() ) {
		mImage = eeNew( cImage, ( tex->GetPixelsPtr(), tex->Width(), tex->Height(), tex->Channels() ) );

		tex->Unlock();
	} else {
		eePRINTL( "cCursor::cCursor: Error creating cursor from cTexture." );
	}
}

cCursor::cCursor( cImage * img, const eeVector2i& hotspot, const std::string& name, Window::cWindow * window ) :
	mId( String::Hash( name ) ),
	mName( name ),
	mImage( NULL ),
	mHotSpot( hotspot ),
	mWindow( window )
{
	if ( img->MemSize() ) {
		mImage = eeNew( cImage, ( img->GetPixelsPtr(), img->Width(), img->Height(), img->Channels() ) );
	} else {
		eePRINTL( "cCursor::cCursor: Error creating cursor from cImage." );
	}
}

cCursor::cCursor( const std::string& path, const eeVector2i& hotspot, const std::string& name, Window::cWindow * window ) :
	mId( String::Hash( name ) ),
	mName( name ),
	mImage( NULL ),
	mHotSpot( hotspot ),
	mWindow( window )
{
	mImage = eeNew( cImage, ( path ) );

	if ( NULL == mImage->GetPixels() ) {
		eePRINTL( "cCursor::cCursor: Error creating cursor from path." );
	}
}

cCursor::~cCursor() {
	eeSAFE_DELETE( mImage );
}

const eeVector2i& cCursor::HotSpot() const {
	return mHotSpot;
}

const Uint32& cCursor::Id() const {
	return mId;
}

const std::string& cCursor::Name() const {
	return mName;
}

cImage * cCursor::Image() const {
	return mImage;
}

}}
