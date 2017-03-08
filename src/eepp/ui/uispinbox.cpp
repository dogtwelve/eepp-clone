#include <eepp/ui/uispinbox.hpp>
#include <eepp/graphics/subtexture.hpp>

namespace EE { namespace UI {

UISpinBox * UISpinBox::New() {
	return eeNew( UISpinBox, () );
}

UISpinBox::UISpinBox() :
	UIWidget(),
	mMinValue( 0.f ),
	mMaxValue( 1024.f ),
	mValue( 0 ),
	mClickStep( 1.f )
{
	mInput	= UITextInput::New();
	mInput->setVisible( true );
	mInput->setEnabled( true );
	mInput->setParent( this );

	mPushUp	= UIControlAnim::New();
	mPushUp->setVisible( true );
	mPushUp->setEnabled( true );
	mPushUp->setParent( this );
	mPushUp->setSize( 16, 16 );

	mPushDown = UIControlAnim::New();
	mPushDown->setVisible( true );
	mPushDown->setEnabled( true );
	mPushDown->setParent( this );
	mPushDown->setSize( 16, 16 );

	mInput->getInputTextBuffer()->setAllowOnlyNumbers( true, false );

	internalValue( mValue, true );

	adjustChilds();

	applyDefaultTheme();
}

UISpinBox::~UISpinBox() {
}

Uint32 UISpinBox::getType() const {
	return UI_TYPE_SPINBOX;
}

bool UISpinBox::isType( const Uint32& type ) const {
	return UISpinBox::getType() == type ? true : UIWidget::isType( type );
}

void UISpinBox::setTheme( UITheme * Theme ) {
	UIControl::setThemeControl( Theme, "spinbox" );

	mInput->setThemeControl( Theme, "spinbox_input" );
	mPushUp->setThemeControl( Theme, "spinbox_btnup" );
	mPushDown->setThemeControl( Theme, "spinbox_btndown" );

	UISkin * tSkin = NULL;

	tSkin = mPushUp->getSkin();

	if ( NULL != tSkin ) {
		mPushUp->setSize( tSkin->getSize() );
	}

	tSkin = mPushDown->getSkin();

	if ( NULL != tSkin ) {
		mPushDown->setSize( tSkin->getSize() );
	}

	adjustChilds();
}

void UISpinBox::adjustChilds() {
	mInput->setSize( mSize.getWidth() - mPushUp->getSize().getWidth(), mInput->getSize().getHeight() );

	if ( mInput->getSize().getHeight() < mInput->getSkinSize().getHeight() ) {
		mInput->setSize( mSize.getWidth() - mPushUp->getSize().getWidth(), mInput->getSkinSize().getHeight() );
	}

	if ( ( mFlags & UI_AUTO_SIZE ) || mSize.getHeight() < mInput->getSize().getHeight() ) {
		setInternalHeight( mInput->getSkinSize().getHeight() );
	}

	mInput->centerVertical();

	mPushUp->setPosition( mSize.getWidth() - mPushUp->getSize().getWidth(), mInput->getPosition().y );
	mPushDown->setPosition( mSize.getWidth() - mPushDown->getSize().getWidth(), mInput->getPosition().y + mPushUp->getSize().getHeight() );
}

void UISpinBox::setPadding( const Recti& padding ) {
	mInput->setPadding( padding );
}

const Recti& UISpinBox::getPadding() const {
	return mInput->getPadding();
}

void UISpinBox::setClickStep( const Float& step ) {
	mClickStep = step;
}

const Float& UISpinBox::getClickStep() const {
	return mClickStep;
}

Uint32 UISpinBox::onMessage( const UIMessage * Msg ) {
	switch ( Msg->getMsg() ) {
		case UIMessage::MsgClick:
		{
			if ( Msg->getFlags() & EE_BUTTON_LMASK ) {
				if ( Msg->getSender() == mPushUp ) {
					addValue( mClickStep );
				} else if ( Msg->getSender() == mPushDown ) {
					addValue( -mClickStep );
				}
			} else if ( Msg->getFlags() & EE_BUTTONS_WUWD ) {
				if ( Msg->getFlags() & EE_BUTTON_WUMASK )
					addValue( mClickStep );
				else
					addValue( -mClickStep );
			}

			return 1;
		}
	}

	return 0;
}

void UISpinBox::addValue( const Float& value ) {
	if ( !mInput->getText().size() )
		mInput->setText( String::toStr( static_cast<Int32>( mMinValue ) ) );

	this->setValue( mValue + value );
}

void UISpinBox::internalValue( const Float& Val, const bool& Force ) {
	if ( Force || Val != mValue ) {
		if ( Val >= mMinValue && Val <= mMaxValue ) {
			Float iValN	= (Float)(Int32) Val;
			Float fValN 	= (Float)iValN;

			if ( fValN == Val ) {
				mInput->setText( String::toStr( iValN ) );
			} else {
				mInput->setText( String::toStr( Val ) );
			}

			mValue = Val;

			onValueChange();
		}
	}
}

void UISpinBox::onSizeChange() {
	UIWidget::onSizeChange();

	adjustChilds();
}

UISpinBox * UISpinBox::setValue( const Float& Val ) {
	internalValue( Val, false );
	return this;
}

const Float& UISpinBox::getValue() const {
	return mValue;
}

UISpinBox * UISpinBox::setMinValue( const Float& MinVal ) {
	mMinValue = MinVal;

	if ( mValue < mMinValue )
		mValue = mMinValue;

	return this;
}

const Float& UISpinBox::getMinValue() const {
	return mMinValue;
}

UISpinBox * UISpinBox::setMaxValue( const Float& MaxVal ) {
	mMaxValue = MaxVal;

	if ( mValue > mMaxValue )
		mValue = mMaxValue;

	return this;
}

const Float& UISpinBox::getMaxValue() const {
	return mMaxValue;
}

void UISpinBox::update() {
	bool Changed = mInput->getInputTextBuffer()->changedSinceLastUpdate();

	UIControlAnim::update();

	if ( Changed ) {
		if ( !mInput->getText().size() ) {
			setValue( 0 );
		} else {
			Float Val = mValue;

			if ( '.' == mInput->getText()[ mInput->getText().size() - 1 ] ) {
				Uint32 pos = (Uint32)mInput->getText().find_first_of( "." );

				if ( pos != mInput->getText().size() - 1 )
					mInput->setText( mInput->getText().substr( 0, mInput->getText().size() - 1 ) );
			} else {
				bool Res 	= String::fromString<Float>( Val, mInput->getText() );

				if ( Res )
					setValue( Val );
			}
		}
	}
}

UIControlAnim * UISpinBox::getButtonPushUp() const {
	return mPushUp;
}

UIControlAnim * UISpinBox::getButtonPushDown() const {
	return mPushDown;
}

UITextInput * UISpinBox::getTextInput() const {
	return mInput;
}

UISpinBox * UISpinBox::setAllowOnlyNumbers(bool allow) {
	mInput->getInputTextBuffer()->setAllowOnlyNumbers( true, allow );

	return this;
}

bool UISpinBox::dotsInNumbersAllowed() {
	return mInput->getInputTextBuffer()->dotsInNumbersAllowed();
}

void UISpinBox::onAlphaChange() {
	UIControlAnim::onAlphaChange();
	
	mInput->setAlpha( mAlpha );
	mPushUp->setAlpha( mAlpha );
	mPushDown->setAlpha( mAlpha );
}

}}
