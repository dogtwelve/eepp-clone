#include <eepp/ui/uiwinmenu.hpp>
#include <eepp/ui/uimanager.hpp>
#include <eepp/graphics/subtexture.hpp>

namespace EE { namespace UI {

UIWinMenu * UIWinMenu::New() {
	return eeNew( UIWinMenu, () );
}

UIWinMenu::UIWinMenu() :
	UIComplexControl(),
	mCurrentMenu( NULL )
{
	if ( !(mFlags & UI_ANCHOR_RIGHT) )
		mFlags |= UI_ANCHOR_RIGHT;

	UITheme * theme = UIThemeManager::instance()->getDefaultTheme();

	if ( NULL != theme )
		mStyleConfig = theme->getWinMenuStyleConfig();

	onParentChange();

	applyDefaultTheme();
}

UIWinMenu::~UIWinMenu() {
	destroyMenues();
}

Uint32 UIWinMenu::getType() const {
	return UI_TYPE_WINMENU;
}

bool UIWinMenu::isType( const Uint32& type ) const {
	return UIWinMenu::getType() == type ? true : UIComplexControl::isType( type );
}

void UIWinMenu::addMenuButton( const String& ButtonText, UIPopUpMenu * Menu ) {
	eeASSERT( NULL != Menu );

	UISelectButton * Button = UISelectButton::New();

	if ( mFlags & UI_DRAW_SHADOW )
		Button->setFlags( UI_DRAW_SHADOW );

	Button->setFontStyleConfig( mStyleConfig );
	Button->setParent( this );
	Button->setText( ButtonText );
	Button->setVisible( true );
	Button->setEnabled( true );

	if ( NULL != mSkinState && NULL != mSkinState->getSkin() )
		Button->setThemeControl( mSkinState->getSkin()->getTheme(), "winmenubutton" );

	Menu->setVisible( false );
	Menu->setEnabled( false );
	Menu->setParent( getParent() );
	Menu->addEventListener( UIEvent::EventOnComplexControlFocusLoss, cb::Make1( this, &UIWinMenu::onMenuFocusLoss ) );

	mButtons.push_back( std::make_pair( Button, Menu ) );

	refreshButtons();
}

void UIWinMenu::setTheme( UITheme * Theme ) {
	UIComplexControl::setThemeControl( Theme, "winmenu" );

	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		it->first->setThemeControl( Theme, "winmenubutton" );
	}

	if ( 0 == mStyleConfig.MenuHeight && NULL != getSkin() && NULL != getSkin() ) {
		mStyleConfig.MenuHeight = getSkin()->getSize().getHeight();

		setSize( getParent()->getSize().getWidth(), mStyleConfig.MenuHeight );

		updateAnchorsDistances();
	}
}

void UIWinMenu::removeMenuButton( const String& ButtonText ) {
	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		if ( it->first->getText() == ButtonText ) {
			it->second->close();

			mButtons.erase( it );

			break;
		}
	}
}

UISelectButton * UIWinMenu::getButton( const String& ButtonText ) {
	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		if ( it->first->getText() == ButtonText ) {
			return it->first;
		}
	}

	return NULL;
}

UIPopUpMenu * UIWinMenu::getPopUpMenu( const String& ButtonText ) {
	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		if ( it->first->getText() == ButtonText ) {
			return it->second;
		}
	}

	return NULL;
}

Uint32 UIWinMenu::getMarginBetweenButtons() const {
	return mStyleConfig.MarginBetweenButtons;
}

void UIWinMenu::setMarginBetweenButtons(const Uint32 & marginBetweenButtons) {
	mStyleConfig.MarginBetweenButtons = marginBetweenButtons;
	refreshButtons();
}

FontStyleConfig UIWinMenu::getFontStyleConfig() const {
	return FontStyleConfig(mStyleConfig);
}

void UIWinMenu::setFontStyleConfig(const FontStyleConfig & fontStyleConfig) {
	mStyleConfig = fontStyleConfig;
	refreshButtons();
}

WinMenuStyleConfig UIWinMenu::getStyleConfig() const {
	return mStyleConfig;
}

void UIWinMenu::setStyleConfig(const WinMenuStyleConfig & styleConfig) {
	mStyleConfig = styleConfig;
	refreshButtons();
}

void UIWinMenu::refreshButtons() {
	Uint32 xpos = mStyleConfig.FirstButtonMargin;
	Int32 h = 0, th = 0, ycenter = 0;

	UISkin * skin = getSkin();

	if ( NULL != skin ) {
		h = skin->getSize().getHeight();

		if ( mButtons.begin() != mButtons.end() ) {
			UISelectButton * tbut = mButtons.begin()->first;

			skin = tbut->getSkin();

			if ( NULL != skin ) {
				th = skin->getSize( UISkinState::StateSelected ).getHeight();

				switch ( VAlignGet( getFlags() ) ) {
					case UI_VALIGN_CENTER:
						ycenter = ( h - th ) / 2;
						break;
					case UI_VALIGN_BOTTOM:
						ycenter = ( h - th );
						break;
					case UI_VALIGN_TOP:
						ycenter = 0;
						break;
				}
			}
		}
	}

	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		UISelectButton * pbut	= it->first;
		UITextBox * tbox		= pbut->getTextBox();

		pbut->setFontStyleConfig( mStyleConfig );
		pbut->setSize( PixelDensity::pxToDpI( tbox->getTextWidth() ) + mStyleConfig.ButtonMargin, getSize().getHeight() );
		pbut->setPosition( xpos, ycenter );

		xpos += pbut->getSize().getWidth() + mStyleConfig.MarginBetweenButtons;
	}
}

Uint32 UIWinMenu::onMessage( const UIMessage * Msg ) {
	switch ( Msg->getMsg() ) {
		case UIMessage::MsgMouseUp:
		case UIMessage::MsgMouseEnter:
		{
			if ( Msg->getSender()->isType( UI_TYPE_SELECTBUTTON ) ) {
				UISelectButton * tbut	= reinterpret_cast<UISelectButton*> ( Msg->getSender() );
				UIPopUpMenu * tpop		= getMenuFromButton( tbut );

				Vector2i pos( tbut->getPosition().x, tbut->getPosition().y + tbut->getSize().getHeight() );
				tpop->setPosition( pos );

				if ( Msg->getMsg() == UIMessage::MsgMouseEnter ) {
					if ( NULL != mCurrentMenu ) {
						mCurrentMenu = tpop;

						tbut->select();
						tpop->show();
					}
				} else {
					if ( Msg->getFlags() & EE_BUTTON_LMASK ) {
						mCurrentMenu = tpop;

						tbut->select();
						tpop->show();
					}
				}

				return 1;
			}

			break;
		}
		case UIMessage::MsgSelected:
		{
			for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
				if ( it->first != Msg->getSender() ) {
					it->first->unselect();
				}
			}

			return 1;
		}
		case UIMessage::MsgFocusLoss:
		{
			UIControl * FocusCtrl = UIManager::instance()->getFocusControl();

			if ( !isParentOf( FocusCtrl ) && !isPopUpMenuChild( FocusCtrl ) ) {
				onComplexControlFocusLoss();
			}

			return 1;
		}
	}

	return 0;
}

void UIWinMenu::onParentChange() {
	setSize( getParent()->getSize().getWidth(), mStyleConfig.MenuHeight );

	updateAnchorsDistances();
}

void UIWinMenu::unselectButtons() {
	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		it->first->unselect();
	}
}

UIPopUpMenu * UIWinMenu::getMenuFromButton( UISelectButton * Button ) {
	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		if ( it->first == Button ) {
			return it->second;
		}
	}

	return NULL;
}

bool UIWinMenu::isPopUpMenuChild( UIControl * Ctrl ) {
	for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
		if ( it->second == Ctrl || it->second->isParentOf( Ctrl ) ) {
			return true;
		}
	}

	return false;
}

void UIWinMenu::onMenuFocusLoss( const UIEvent * Event ) {
	UIControl * FocusCtrl = UIManager::instance()->getFocusControl();

	if ( !isParentOf( FocusCtrl ) && !isPopUpMenuChild( FocusCtrl ) ) {
		onComplexControlFocusLoss();
	}
}

void UIWinMenu::onComplexControlFocusLoss() {
	UIComplexControl::onComplexControlFocusLoss();

	if ( NULL != mCurrentMenu ) {
		mCurrentMenu->hide();

		mCurrentMenu = NULL;
	}

	unselectButtons();
}

void UIWinMenu::destroyMenues() {
	if ( !UIManager::instance()->isShootingDown() ) {
		for ( WinMenuList::iterator it = mButtons.begin(); it != mButtons.end(); it++ ) {
			it->second->close();
		}
	}
}

}}
