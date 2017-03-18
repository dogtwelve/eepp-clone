#include <eepp/ui/uimenucheckbox.hpp>
#include <eepp/ui/uimenu.hpp>

namespace EE { namespace UI {

UIMenuCheckBox * UIMenuCheckBox::New() {
	return eeNew( UIMenuCheckBox, () );
}

UIMenuCheckBox::UIMenuCheckBox() :
	UIMenuItem(),
	mActive( false ),
	mSkinActive( NULL ),
	mSkinInactive( NULL )
{
	applyDefaultTheme();
	mIcon->setFlags( UI_SKIN_KEEP_SIZE_ON_DRAW );
}

UIMenuCheckBox::~UIMenuCheckBox() {
}

Uint32 UIMenuCheckBox::getType() const {
	return UI_TYPE_MENUCHECKBOX;
}

bool UIMenuCheckBox::isType( const Uint32& type ) const {
	return UIMenuCheckBox::getType() == type ? true : UIMenuItem::isType( type );
}

void UIMenuCheckBox::setTheme( UITheme * Theme ) {
	UIWidget::setTheme( Theme );

	setThemeControl( Theme, "menuitem" );

	mSkinActive		= Theme->getSkin( "menucheckbox_active" );
	mSkinInactive	= Theme->getSkin( "menucheckbox_inactive" );

	setActive( mActive );

	onThemeLoaded();
}

const bool& UIMenuCheckBox::isActive() const {
	return mActive;
}

void UIMenuCheckBox::setActive( const bool& active ) {
	bool oActive = mActive;
	mActive = active;

	if ( mActive ) {
		if ( NULL != mSkinActive ) {
			if ( NULL == mIcon->getSkin() || mIcon->getSkin()->getName() != mSkinActive->getName() )
				mIcon->setSkin( mSkinActive );

			if ( mSkinState->getState() == UISkinState::StateSelected )
				mIcon->setSkinState( UISkinState::StateMouseEnter );
			else
				mIcon->setSkinState( UISkinState::StateNormal );
		} else {
			mIcon->removeSkin();
		}
	} else {
		if ( NULL != mSkinInactive ) {
			if ( NULL == mIcon->getSkin() || mIcon->getSkin()->getName() != mSkinInactive->getName() )
				mIcon->setSkin( mSkinInactive );

			if ( mSkinState->getState() == UISkinState::StateSelected )
				mIcon->setSkinState( UISkinState::StateMouseEnter );
			else
				mIcon->setSkinState( UISkinState::StateNormal );
		} else {
			mIcon->removeSkin();
		}
	}

	if ( oActive != active ) {
		UIMenu * Menu = reinterpret_cast<UIMenu*> ( getParent() );

		if ( !Menu->checkControlSize( this ) ) {
			if ( NULL != getIcon()->getSubTexture() ) {
				setPadding( Recti( 0, 0, 0, 0 ) );
			}
		}

		onValueChange();
	}
}

void UIMenuCheckBox::switchActive() {
	setActive( !mActive );
}

Uint32 UIMenuCheckBox::onMouseUp( const Vector2i &Pos, const Uint32 Flags ) {
	UIMenuItem::onMouseUp( Pos, Flags );

	if ( getParent()->isVisible() && ( Flags & EE_BUTTONS_LRM ) )
		switchActive();

	return 1;
}

void UIMenuCheckBox::onStateChange() {
	UIMenuItem::onStateChange();

	setActive( mActive );
}

bool UIMenuCheckBox::inheritsFrom( const Uint32 Type ) {
	if ( Type == UI_TYPE_MENUITEM )
		return true;

	return false;
}

}}
