#include <eepp/graphics/drawablegroup.hpp>
#include <eepp/math/rect.hpp>
#include <eepp/graphics/renderer/renderer.hpp>

namespace EE { namespace Graphics {

DrawableGroup::DrawableGroup() :
	Drawable( DRAWABLE_GROUP ),
	mNeedsUpdate(true),
	mClipEnabled(true),
	mDrawableOwner(true)
{
}

DrawableGroup::~DrawableGroup() {
	clearDrawables();
}

void DrawableGroup::clearDrawables() {
	if ( mDrawableOwner ) {
		for ( std::size_t i = 0; i < mGroup.size(); i++ ) {
			Drawable * drawable = mGroup[i];
			eeSAFE_DELETE( drawable );
		}
	}

	mGroup.clear();
	mPos.clear();
}

Drawable * DrawableGroup::addDrawable( Drawable * drawable ) {
	mGroup.push_back( drawable );
	mPos.push_back( drawable->getPosition() );
	return drawable;
}

Uint32 DrawableGroup::getDrawableCount() const {
	return mGroup.size();
}

bool DrawableGroup::isClipEnabled() const {
	return mClipEnabled;
}

void DrawableGroup::setClipEnabled(bool clipEnabled) {
	mClipEnabled = clipEnabled;
}

bool DrawableGroup::isDrawableOwner() const
{
	return mDrawableOwner;
}

void DrawableGroup::setDrawableOwner(bool drawableOwner)
{
	mDrawableOwner = drawableOwner;
}

Sizef DrawableGroup::getSize() {
	return mSize;
}

void DrawableGroup::draw(const Vector2f & position, const Sizef & size) {
	if ( position != mPosition ) {
		mPosition = position;
		mNeedsUpdate = true;
	}

	if ( size != mSize ) {
		mSize = size;
		mNeedsUpdate = true;
	}

	if ( mNeedsUpdate )
		update();

	if ( mGroup.empty() )
		return;

	if ( mClipEnabled )
		GLi->getClippingMask()->clipEnable( mPosition.x, mPosition.y, mSize.x, mSize.y );

	for ( std::size_t i = 0; i < mGroup.size(); i++ ) {
		Drawable * drawable = mGroup[i];
		drawable->draw();
	}

	if ( mClipEnabled )
		GLi->getClippingMask()->clipDisable();
}

void DrawableGroup::draw(const Vector2f & position) {
	draw( position, mSize );
}

void DrawableGroup::draw() {
	draw( mPosition, mSize );
}

void DrawableGroup::onPositionChange() {
	mNeedsUpdate = true;
}

void DrawableGroup::update() {
	Sizef nSize( mSize );

	for ( std::size_t i = 0; i < mGroup.size(); i++ ) {
		Drawable * drawable = mGroup[i];
		Vector2f pos( mPosition + mPos[i] );
		Sizef s( mPos[i] + drawable->getSize() );

		drawable->setPosition( pos );

		nSize.x = eemax( nSize.x, s.x );
		nSize.y = eemax( nSize.y, s.y );
	}

	if ( Vector2f::Zero == mSize ) {
		mSize = nSize;
	}

	mNeedsUpdate = false;
}

}} 
