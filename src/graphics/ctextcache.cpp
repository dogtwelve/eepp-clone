#include "ctextcache.hpp"
#include "cfont.hpp"

namespace EE { namespace Graphics {

cTextCache::cTextCache() :
	mFont(NULL),
	mCachedWidth(0.f),
	mNumLines(1),
	mFontColor(0xFFFFFFFF),
	mFontShadowColor(0xFF000000),
	mCachedCoords(false),
	mFlags(0)
{
}

cTextCache::cTextCache( cFont * font, const std::wstring& text, eeColorA FontColor, eeColorA FontShadowColor ) :
	mText( text ),
	mFont( font ),
	mCachedWidth(0.f),
	mNumLines(1)
{
	Cache();
	UpdateCoords();
	Color( FontColor );
	ShadowColor( FontShadowColor );
}

cTextCache::~cTextCache() {
}

cFont * cTextCache::Font() const {
	return mFont;
}

void cTextCache::Font( cFont * font ) {
	mFont = font;
	Cache();
}

std::wstring& cTextCache::Text() {
	return mText;
}

void cTextCache::UpdateCoords() {
	mRenderCoords.resize( mText.size() * 4 );
	mColors.resize( mText.size() * 4, mFontColor );
}

void cTextCache::Text( const std::wstring& text ) {
	bool needUpdate = false;

	if ( mText.size() != text.size() )
		needUpdate = true;

	mText = text;

	if ( needUpdate )
		UpdateCoords();

	Cache();
}

const eeColorA& cTextCache::Color() const {
	return mFontColor;
}

void cTextCache::Color(const eeColorA& Color) {
	if ( mFontColor != Color ) {
		mFontColor = Color;

		mColors.assign( mText.size() * 4, mFontColor );
	}
}

const eeColorA& cTextCache::ShadowColor() const {
	return mFontShadowColor;
}

void cTextCache::ShadowColor(const eeColorA& Color) {
	mFontShadowColor = Color;
}

std::vector<eeVertexCoords>& cTextCache::VertextCoords() {
	return mRenderCoords;
}

std::vector<eeColorA>& cTextCache::Colors() {
	return mColors;
}

void cTextCache::Text( const std::string& text ) {
	Text( stringTowstring( text ) );
}

void cTextCache::Cache() {
	if ( NULL != mFont && mText.size() )
		mFont->CacheWidth( mText, mLinesWidth, mCachedWidth, mNumLines );
	else
		mCachedWidth = 0;

	mCachedCoords = false;
}

eeFloat cTextCache::GetTextWidth() {
	return mCachedWidth;
}

eeFloat cTextCache::GetTextHeight() {
	return (eeFloat)mFont->GetFontSize() * (eeFloat)mNumLines;
}

const std::vector<eeFloat>& cTextCache::LinesWidth() {
	return mLinesWidth;
}

void cTextCache::Draw( const eeFloat& X, const eeFloat& Y, const Uint32& Flags, const eeFloat& Scale, const eeFloat& Angle, const EE_PRE_BLEND_FUNC& Effect ) {
	if ( NULL != mFont ) {
		eeColorA FontColor = mFont->Color();
		eeColorA FontShadowColor = mFont->ShadowColor();

		mFont->Color( mFontColor );
		mFont->ShadowColor( mFontShadowColor );

		if ( mFlags != Flags ) {
			mFlags = Flags;
			mCachedCoords = false;
		}

		glTranslatef( X, Y, 0.f );

		mFont->Draw( *this, 0, 0, Flags, Scale, Angle, Effect );

		glTranslatef( -X, -Y, 0.f );

		mFont->Color( FontColor );
		mFont->ShadowColor( FontShadowColor );
	}
}

const bool& cTextCache::CachedCoords() const {
	return mCachedCoords;
}

void cTextCache::CachedCoords( const bool& cached ) {
	mCachedCoords = cached;
}

}}
