#include <eepp/ui/uiscrollbar.hpp>
#include <eepp/ui/uimanager.hpp>
#include <eepp/graphics/subtexture.hpp>

namespace EE { namespace UI {

UIScrollBar::UIScrollBar( const UIScrollBar::CreateParams& Params ) :
	UIComplexControl( Params )
{
	UIControlAnim::CreateParams CParams = Params;
	CParams.Size = Sizei( 16, 16 );
	CParams.setParent( this );

	mBtnDown	= eeNew( UIControlAnim, ( CParams ) );
	mBtnUp		= eeNew( UIControlAnim, ( CParams ) );

	mBtnDown->setVisible( true );
	mBtnDown->setEnabled( true );
	mBtnUp->setVisible( true );
	mBtnUp->setEnabled( true );

	UISlider::CreateParams SParams;
	SParams.Background = Params.Background;
	SParams.Blend = Params.Blend;
	SParams.Border = Params.Border;
	SParams.Flags = Params.Flags;
	SParams.setParent( this );
	SParams.Pos = Params.Pos;
	SParams.Size = Params.Size;
	SParams.VerticalSlider = Params.VerticalScrollBar;
	SParams.AllowHalfSliderOut = false;
	SParams.ExpandBackground = true;

	mSlider		= eeNew( UISlider, ( SParams ) );
	mSlider->setVisible( true );
	mSlider->setEnabled( true );

	mSlider->addEventListener( UIEvent::EventOnValueChange, cb::Make1( this, &UIScrollBar::onValueChangeCb ) );

	adjustChilds();

	applyDefaultTheme();
}

UIScrollBar::UIScrollBar() :
	UIComplexControl()
{
	mFlags |= UI_AUTO_SIZE;

	mBtnDown	= eeNew( UIControlAnim, () );
	mBtnUp		= eeNew( UIControlAnim, () );
	mBtnUp->setParent( this );
	mBtnUp->setVisible( true );
	mBtnUp->setEnabled( true );
	mBtnUp->setSize( 16, 16 );
	mBtnDown->setParent( this );
	mBtnDown->setVisible( true );
	mBtnDown->setEnabled( true );
	mBtnDown->setSize( 16, 16 );

	mSlider		= eeNew( UISlider, () );
	mSlider->setParent( this );
	mSlider->setVisible( true );
	mSlider->setEnabled( true );

	mSlider->addEventListener( UIEvent::EventOnValueChange, cb::Make1( this, &UIScrollBar::onValueChangeCb ) );

	adjustChilds();

	applyDefaultTheme();
}

UIScrollBar::~UIScrollBar() {
}

Uint32 UIScrollBar::getType() const {
	return UI_TYPE_SCROLLBAR;
}

bool UIScrollBar::isType( const Uint32& type ) const {
	return UIScrollBar::getType() == type ? true : UIComplexControl::isType( type );
}

void UIScrollBar::setTheme( UITheme * Theme ) {
	if ( !isVertical() ) {
		UIControl::setThemeControl( Theme, "hscrollbar" );
		mSlider->setThemeControl( Theme, "hscrollbar_slider" );
		mSlider->getBackSlider()->setThemeControl( Theme, "hscrollbar_bg" );
		mSlider->getSliderButton()->setThemeControl( Theme, "hscrollbar_button" );
		mBtnUp->setThemeControl( Theme, "hscrollbar_btnup" );
		mBtnDown->setThemeControl( Theme, "hscrollbar_btndown" );
	} else {
		UIControl::setThemeControl( Theme, "vscrollbar" );
		mSlider->setThemeControl( Theme, "vscrollbar_slider" );
		mSlider->getBackSlider()->setThemeControl( Theme, "vscrollbar_bg" );
		mSlider->getSliderButton()->setThemeControl( Theme, "vscrollbar_button" );
		mBtnUp->setThemeControl( Theme, "vscrollbar_btnup" );
		mBtnDown->setThemeControl( Theme, "vscrollbar_btndown" );
	}

	UISkin * tSkin = NULL;

	tSkin = mBtnUp->getSkin();

	if ( NULL != tSkin ) {
		mBtnUp->setSize( tSkin->getSize() );
	}

	tSkin = mBtnDown->getSkin();

	if ( NULL != tSkin ) {
		mBtnDown->setSize( tSkin->getSize() );
	}

	adjustChilds();

	mSlider->adjustChilds();
}

void UIScrollBar::onAutoSize() {
	UISkin * tSkin = mSlider->getBackSlider()->getSkin();

	if ( NULL != tSkin ) {
		Sizei size = tSkin->getSize();

		mMinControlSize = PixelDensity::pxToDpI( size );

		if ( mFlags & UI_AUTO_SIZE ) {
			if ( mSlider->isVertical() ) {
				mSlider->setSize( size.getWidth() , mSize.getHeight() );
				setSize( size.getWidth(), mSize.getHeight() );
			} else {
				mSlider->setSize( mSize.getWidth(), size.getHeight() );
				setSize( mSize.getWidth(), size.getHeight() );
			}
		}
	}
}

void UIScrollBar::onSizeChange() {
	adjustChilds();

	mSlider->adjustChilds();

	UIComplexControl::onSizeChange();
}

void UIScrollBar::adjustChilds() {
	onAutoSize();

	mBtnUp->setPosition( 0, 0 );

	if ( !isVertical() ) {
		mBtnDown->setPosition( mSize.getWidth() - mBtnDown->getSize().getWidth(), 0 );
		mSlider->setSize( mSize.getWidth() - mBtnDown->getSize().getWidth() - mBtnUp->getSize().getWidth(), mSize.getHeight() );
		mSlider->setPosition( mBtnUp->getSize().getWidth(), 0 );

		mBtnDown->centerVertical();
		mBtnUp->centerVertical();
		mSlider->centerVertical();
	} else {
		mBtnDown->setPosition( 0, mSize.getHeight() - mBtnDown->getSize().getHeight() );
		mSlider->setSize( mSize.getWidth(), mSize.getHeight() - mBtnDown->getSize().getHeight() - mBtnUp->getSize().getHeight() );
		mSlider->setPosition( 0, mBtnUp->getSize().getHeight() );

		mBtnDown->centerHorizontal();
		mBtnUp->centerHorizontal();
		mSlider->centerHorizontal();
	}
}

void UIScrollBar::update() {
	UIControlAnim::update();

	if ( mBtnUp->isMouseOver() || mBtnDown->isMouseOver() ) {
		manageClick( UIManager::instance()->getInput()->getClickTrigger() );
	}
}

void UIScrollBar::manageClick( const Uint32& Flags ) {
	if ( Flags & EE_BUTTONS_WUWD ) {
		if ( Flags & EE_BUTTON_WUMASK )
			mSlider->setValue( getValue() + getClickStep() );
		else
			mSlider->setValue( getValue() - getClickStep() );
	}
}

Uint32 UIScrollBar::onMessage( const UIMessage * Msg ) {
	switch ( Msg->getMsg() ) {
		case UIMessage::MsgClick:
		{
			if ( Msg->getFlags() & EE_BUTTON_LMASK ) {
				if ( Msg->getSender() == mBtnUp ) {
					mSlider->setValue( getValue() - getClickStep() );
				} else if ( Msg->getSender() == mBtnDown ) {
					mSlider->setValue( getValue() + getClickStep() );
				}
			}

			return 1;
		}
	}

	return 0;
}

void UIScrollBar::setValue( Float Val ) {
	mSlider->setValue( Val );
}

const Float& UIScrollBar::getValue() const {
	return mSlider->getValue();
}

void UIScrollBar::setMinValue( const Float& MinVal ) {
	mSlider->setMinValue( MinVal );
}

const Float& UIScrollBar::getMinValue() const {
	return mSlider->getMinValue();
}

void UIScrollBar::setMaxValue( const Float& MaxVal ) {
	mSlider->setMaxValue( MaxVal );
}

const Float& UIScrollBar::getMaxValue() const {
	return mSlider->getMaxValue();
}

void UIScrollBar::setClickStep( const Float& step ) {
	mSlider->setClickStep( step );
}

const Float& UIScrollBar::getClickStep() const {
	return mSlider->getClickStep();
}

Float UIScrollBar::getPageStep() const {
	return mSlider->getPageStep();
}

void UIScrollBar::setPageStep(const Float& pageStep) {
	mSlider->setPageStep( pageStep );
}

bool UIScrollBar::isVertical() const {
	return mSlider->isVertical();
}

void UIScrollBar::onValueChangeCb( const UIEvent * Event ) {
	onValueChange();
}

UISlider * UIScrollBar::getSlider() const {
	return mSlider;
}

UIControlAnim * UIScrollBar::getButtonUp() const {
	return mBtnUp;
}

UIControlAnim * UIScrollBar::getButtonDown() const {
	return mBtnDown;
}

bool UIScrollBar::getExpandBackground() const {
	return mSlider->getExpandBackground();
}

void UIScrollBar::setExpandBackground( bool expandBackground ) {
	mSlider->setExpandBackground( expandBackground );

	adjustChilds();
}

UI_ORIENTATION UIScrollBar::getOrientation() const {
	return mSlider->getOrientation();
}

UIControl * UIScrollBar::setOrientation( const UI_ORIENTATION & orientation ) {
	mSlider->setOrientation( orientation );

	applyDefaultTheme();

	return this;
}


void UIScrollBar::onAlphaChange() {
	UIControlAnim::onAlphaChange();
	
	mSlider->setAlpha( mAlpha );
	mBtnUp->setAlpha( mAlpha );
	mBtnDown->setAlpha( mAlpha );
}

}}
