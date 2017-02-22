#include <eepp/ui/uitab.hpp>
#include <eepp/ui/uitabwidget.hpp>
#include <eepp/ui/uimanager.hpp>

namespace EE { namespace UI {

UITab::UITab( UISelectButton::CreateParams& Params, UIControl * CtrlOwned ) :
	UISelectButton( Params ),
	mCtrlOwned( CtrlOwned )
{
	applyDefaultTheme();
}

UITab::~UITab() {
}

Uint32 UITab::getType() const {
	return UI_TYPE_TAB;
}

bool UITab::isType( const Uint32& type ) const {
	return UITab::getType() == type ? true : UISelectButton::isType( type );
}

UITabWidget * UITab::getTabWidget() {
	if ( getParent()->getParent()->isType( UI_TYPE_TABWIDGET ) ) {
		return reinterpret_cast<UITabWidget*> ( getParent()->getParent() );
	}

	return NULL;
}

void UITab::setTheme( UITheme * Theme ) {
	std::string tabPos = "tab";

	UITabWidget * tTabW = getTabWidget();

	if ( NULL != tTabW ) {
		if ( tTabW->mSpecialBorderTabs ) {
			if ( 0 == tTabW->getTabIndex( this ) ) {
				tabPos = "tab_left";
			} else if ( tTabW->getCount() > 0 && ( tTabW->getCount() - 1 ) == tTabW->getTabIndex( this ) ) {
				tabPos = "tab_right";
			}
		}
	}

	UIControl::setThemeControl( Theme, tabPos );

	doAftersetTheme();
}

Uint32 UITab::onMouseClick( const Vector2i &Pos, const Uint32 Flags ) {
	UISelectButton::onMouseClick( Pos, Flags );

	UITabWidget * tTabW = getTabWidget();

	if ( NULL != tTabW ) {
		if ( Flags & EE_BUTTON_LMASK ) {
			tTabW->setTabSelected( this );
		}
	}

	return 1;
}

void UITab::onStateChange() {
	UISelectButton::onStateChange();

	UITabWidget * tTabW = getTabWidget();

	if ( NULL != tTabW ) {
		Int32 skinSize = getSkin()->getSize( mSkinState->getState() ).getHeight();

		setPixelsSize( mRealSize.getWidth(), skinSize );

		if ( mSkinState->getState() == UISkinState::StateSelected ) {
			mTextBox->setFontColor( tTabW->mFontSelectedColor );
		} else if ( mSkinState->getState() == UISkinState::StateMouseEnter ) {
			mTextBox->setFontColor( tTabW->mFontOverColor );
		} else {
			mTextBox->setFontColor( tTabW->mFontColor );
		}
	}
}

const String& UITab::getText() {
	return UIPushButton::getText();
}

void UITab::setText( const String &text ) {
	UITabWidget * tTabW = getTabWidget();

	if ( NULL != tTabW ) {
		if ( text.size() > tTabW->mMaxTextLength ) {
			UIPushButton::setText( text.substr( 0, tTabW->mMaxTextLength ) );

			autoSize();

			return;
		}
	}

	UIPushButton::setText( text );

	autoSize();
}

void UITab::autoSize() {
	if ( mFlags & UI_AUTO_SIZE ) {
		Uint32 w = pxToDpI( mTextBox->getTextWidth() ) + getSkinSize().getWidth();

		UITabWidget * tTabW = getTabWidget();

		if ( NULL != tTabW ) {
			w = eemax( w, tTabW->mMinTabWidth );
			w = eemin( w, tTabW->mMaxTabWidth );
		}

		setSize( w, mSize.getHeight() );
	}
}

UIControl * UITab::ctrlOwned() const {
	return mCtrlOwned;
}

void UITab::update() {
	UISelectButton::update();

	if ( mEnabled && mVisible ) {
		if ( isMouseOver() ) {
			UITabWidget * tTabW	= getTabWidget();

			if ( NULL != tTabW ) {
				Uint32 Flags 			= UIManager::instance()->getInput()->getClickTrigger();

				if ( Flags & EE_BUTTONS_WUWD ) {
					if ( Flags & EE_BUTTON_WUMASK ) {
						tTabW->selectPrev();
					} else if ( Flags & EE_BUTTON_WDMASK ) {
						tTabW->selectNext();
					}
				} else if ( tTabW->mTabsClosable && ( Flags & EE_BUTTON_MMASK ) ) {
					tTabW->remove( this );
				}
			}
		}
	}
}

}}
