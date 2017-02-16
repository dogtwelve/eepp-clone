#include <eepp/ui/uisprite.hpp>
#include <eepp/graphics/sprite.hpp>

namespace EE { namespace UI {

UISprite::UISprite( const UISprite::CreateParams& Params ) :
	UIComplexControl( Params ),
	mSprite( Params.Sprite ),
	mRender( Params.SpriteRender ),
	mAlignOffset(0,0),
	mSubTextureLast(NULL)
{
	if ( Params.DealloSprite )
		mControlFlags |= UI_CTRL_FLAG_FREE_USE;

	if ( ( Flags() & UI_AUTO_SIZE ) || ( Params.Size.x == -1 && Params.Size.y == -1 ) ) {
		if ( NULL != mSprite && NULL != mSprite->getCurrentSubTexture() ) {
			Size( mSprite->getCurrentSubTexture()->size() );
		}
	}
}

UISprite::~UISprite() {
	if ( DealloSprite() )
		eeSAFE_DELETE( mSprite );
}

Uint32 UISprite::Type() const {
	return UI_TYPE_SPRITE;
}

bool UISprite::IsType( const Uint32& type ) const {
	return UISprite::Type() == type ? true : UIComplexControl::IsType( type );
}

Uint32 UISprite::DealloSprite() {
	return mControlFlags & UI_CTRL_FLAG_FREE_USE;
}

void UISprite::Sprite( Graphics::Sprite * sprite ) {
	if ( DealloSprite() )
		eeSAFE_DELETE( mSprite );

	mSprite = sprite;
	
	UpdateSize();
}

void UISprite::Draw() {
	UIControlAnim::Draw();

	if ( mVisible ) {
		if ( NULL != mSprite && 0.f != mAlpha ) {
			CheckSubTextureUpdate();
			mSprite->position( (Float)( mScreenPos.x + mAlignOffset.x ), (Float)( mScreenPos.y + mAlignOffset.y ) );
			mSprite->draw( Blend(), mRender );
		}
	}
}

void UISprite::CheckSubTextureUpdate() {
	if ( NULL != mSprite && NULL != mSprite->getCurrentSubTexture() && mSprite->getCurrentSubTexture() != mSubTextureLast ) {
		UpdateSize();
		AutoAlign();
		mSubTextureLast = mSprite->getCurrentSubTexture();
	}
}

void UISprite::Alpha( const Float& alpha ) {
	UIControlAnim::Alpha( alpha );
	
	if ( NULL != mSprite )
		mSprite->alpha( alpha );
}

Graphics::Sprite * UISprite::Sprite() const {
	return mSprite;
}

ColorA UISprite::Color() const {
	if ( NULL != mSprite )
		return mSprite->color();

	return ColorA();
}

void UISprite::Color( const ColorA& color ) {
	if ( NULL != mSprite )
		mSprite->color( color );
	
	Alpha( color.a() );
}

const EE_RENDER_MODE& UISprite::RenderMode() const {
	return mRender;
}

void UISprite::RenderMode( const EE_RENDER_MODE& render ) {
	mRender = render;
}

void UISprite::UpdateSize() {
	if ( Flags() & UI_AUTO_SIZE ) {
		if ( NULL != mSprite ) {
			if ( NULL != mSprite->getCurrentSubTexture() && mSprite->getCurrentSubTexture()->size() != mSize )
				Size( mSprite->getCurrentSubTexture()->size() );
		}
	}
}

void UISprite::AutoAlign() {
	if ( NULL == mSprite || NULL == mSprite->getCurrentSubTexture() )
		return;

	SubTexture * tSubTexture = mSprite->getCurrentSubTexture();

	if ( HAlignGet( mFlags ) == UI_HALIGN_CENTER ) {
		mAlignOffset.x = mSize.width() / 2 - tSubTexture->size().width() / 2;
	} else if ( FontHAlignGet( mFlags ) == UI_HALIGN_RIGHT ) {
		mAlignOffset.x =  mSize.width() - tSubTexture->size().width();
	} else {
		mAlignOffset.x = 0;
	}

	if ( VAlignGet( mFlags ) == UI_VALIGN_CENTER ) {
		mAlignOffset.y = mSize.height() / 2 - tSubTexture->size().height() / 2;
	} else if ( FontVAlignGet( mFlags ) == UI_VALIGN_BOTTOM ) {
		mAlignOffset.y = mSize.height() - tSubTexture->size().height();
	} else {
		mAlignOffset.y = 0;
	}
}

const Vector2i& UISprite::AlignOffset() const {
	return mAlignOffset;
}

void UISprite::OnSizeChange() {
	AutoAlign();
}

}}
