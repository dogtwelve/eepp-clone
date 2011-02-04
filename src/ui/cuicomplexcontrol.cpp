#include "cuicomplexcontrol.hpp"
#include "cuimanager.hpp"

namespace EE { namespace UI {

cUIComplexControl::cUIComplexControl( const cUIComplexControl::CreateParams& Params ) :
	cUIControlAnim( Params ),
	mTooltip( NULL )
{
	mType	|= UI_TYPE_GET( UI_TYPE_CONTROL_COMPLEX );

	TooltipText( Params.TooltipText );
}

cUIComplexControl::~cUIComplexControl() {
	eeSAFE_DELETE( mTooltip );
}

void cUIComplexControl::Update() {
	if ( mVisible && NULL != mTooltip && mTooltip->Text().size() ) {
		if ( IsMouseOverMeOrChilds() ) {
			eeVector2i Pos = cUIManager::instance()->GetMousePos();
			Pos.x += cUIThemeManager::instance()->CursorSize().x;
			Pos.y += cUIThemeManager::instance()->CursorSize().y;

			if ( Pos.x + mTooltip->Size().Width() > cUIManager::instance()->MainControl()->Size().Width() ) {
				Pos.x = cUIManager::instance()->GetMousePos().x - mTooltip->Size().Width();
			}

			if ( Pos.y + mTooltip->Size().Height() > cUIManager::instance()->MainControl()->Size().Height() ) {
				Pos.y = cUIManager::instance()->GetMousePos().y - mTooltip->Size().Height();
			}

			if ( 0 == cUIThemeManager::instance()->TooltipTimeToShow() ) {
				if ( !mTooltip->Visible() || cUIThemeManager::instance()->TooltipFollowMouse() )
					mTooltip->Pos( Pos );

				mTooltip->Show();
			} else {
				if ( -1.f != mTooltip->TooltipTime() ) {
					mTooltip->TooltipTimeAdd( cUIManager::instance()->Elapsed() );
				}

				if ( mTooltip->TooltipTime() >= cUIThemeManager::instance()->TooltipTimeToShow() ) {
					if ( mTooltip->TooltipTime() != -1.f ) {
						mTooltip->Pos( Pos );

						mTooltip->Show();

						mTooltip->TooltipTime( -1.f );
					}
				}
			}

			if ( cUIThemeManager::instance()->TooltipFollowMouse() ) {
				mTooltip->Pos( Pos );
			}
		} else {
			mTooltip->TooltipTime( 0.f );

			if ( mTooltip->Visible() )
				mTooltip->Hide();
		}
	}

	cUIControlAnim::Update();
}

void cUIComplexControl::CreateTooltip() {
	if ( NULL != mTooltip )
		return;

	cUITooltip::CreateParams Params;
	Params.Parent( cUIManager::instance()->MainControl() );
	Params.Flags = UI_VALIGN_CENTER | UI_HALIGN_CENTER | UI_AUTO_PADDING | UI_AUTO_SIZE;
	mTooltip = eeNew( cUITooltip, ( Params, this ) );
}

void cUIComplexControl::TooltipText( const std::wstring& Text ) {
	if ( NULL == mTooltip ) {	// If the tooltip wasn't created it will avoid to create a new one if the string is ""
		if ( Text.size() ) {
			CreateTooltip();

			mTooltip->Text( Text );
		}
	} else { // but if it's created, i will allow it
		mTooltip->Text( Text );
	}
}

void cUIComplexControl::TooltipText( const std::string& Text ) {
	TooltipText( stringTowstring( Text ) );
}

std::wstring cUIComplexControl::TooltipText() {
	if ( NULL != mTooltip )
		return mTooltip->Text();

	return std::wstring();
}

void cUIComplexControl::TooltipRemove() {
	mTooltip = NULL;
}

}}