#include <eepp/ui/cuicomplexcontrol.hpp>
#include <eepp/ui/cuimanager.hpp>

namespace EE { namespace UI {

cUIComplexControl::cUIComplexControl( const cUIComplexControl::CreateParams& Params ) :
	cUIControlAnim( Params ),
	mTooltip( NULL ),
	mMinControlSize( Params.MinControlSize )
{
	mControlFlags |= UI_CTRL_FLAG_COMPLEX;

	UpdateAnchorsDistances();

	TooltipText( Params.TooltipText );
}

cUIComplexControl::~cUIComplexControl() {
	eeSAFE_DELETE( mTooltip );
}

Uint32 cUIComplexControl::Type() const {
	return UI_TYPE_CONTROL_COMPLEX;
}

bool cUIComplexControl::IsType( const Uint32& type ) const {
	return cUIComplexControl::Type() == type ? true : cUIControlAnim::IsType( type );
}

void cUIComplexControl::UpdateAnchorsDistances() {
	if ( NULL != mParentCtrl ) {
		mDistToBorder	= Recti( mPos.x, mPos.y, mParentCtrl->Size().x - ( mPos.x + mSize.x ), mParentCtrl->Size().y - ( mPos.y + mSize.y ) );
	}
}

void cUIComplexControl::Update() {
	if ( mVisible && NULL != mTooltip && mTooltip->Text().size() ) {
		if ( IsMouseOverMeOrChilds() ) {
			Vector2i Pos = cUIManager::instance()->GetMousePos();
			Pos.x += cUIThemeManager::instance()->CursorSize().x;
			Pos.y += cUIThemeManager::instance()->CursorSize().y;

			if ( Pos.x + mTooltip->Size().Width() > cUIManager::instance()->MainControl()->Size().Width() ) {
				Pos.x = cUIManager::instance()->GetMousePos().x - mTooltip->Size().Width();
			}

			if ( Pos.y + mTooltip->Size().Height() > cUIManager::instance()->MainControl()->Size().Height() ) {
				Pos.y = cUIManager::instance()->GetMousePos().y - mTooltip->Size().Height();
			}

			if ( Time::Zero == cUIThemeManager::instance()->TooltipTimeToShow() ) {
				if ( !mTooltip->Visible() || cUIThemeManager::instance()->TooltipFollowMouse() )
					mTooltip->Pos( Pos );

				mTooltip->Show();
			} else {
				if ( -1.f != mTooltip->TooltipTime().AsMilliseconds() ) {
					mTooltip->TooltipTimeAdd( cUIManager::instance()->Elapsed() );
				}

				if ( mTooltip->TooltipTime() >= cUIThemeManager::instance()->TooltipTimeToShow() ) {
					if ( mTooltip->TooltipTime().AsMilliseconds() != -1.f ) {
						mTooltip->Pos( Pos );

						mTooltip->Show();

						mTooltip->TooltipTime( Milliseconds( -1.f ) );
					}
				}
			}

			if ( cUIThemeManager::instance()->TooltipFollowMouse() ) {
				mTooltip->Pos( Pos );
			}
		} else {
			mTooltip->TooltipTime( Milliseconds( 0.f ) );

			if ( mTooltip->Visible() )
				mTooltip->Hide();
		}
	}

	cUIControlAnim::Update();
}

void cUIComplexControl::CreateTooltip() {
	if ( NULL != mTooltip )
		return;

	cUITheme * tTheme = cUIThemeManager::instance()->DefaultTheme();

	if ( NULL != tTheme ) {
		mTooltip = tTheme->CreateTooltip( this, cUIManager::instance()->MainControl() );
		mTooltip->Visible( false );
		mTooltip->Enabled( false );
	} else {
		cUITooltip::CreateParams Params;
		Params.Parent( cUIManager::instance()->MainControl() );
		Params.Flags = UI_VALIGN_CENTER | UI_HALIGN_CENTER | UI_AUTO_PADDING | UI_AUTO_SIZE;
		mTooltip = eeNew( cUITooltip, ( Params, this ) );
	}
}

void cUIComplexControl::TooltipText( const String& Text ) {
	if ( NULL == mTooltip ) {	// If the tooltip wasn't created it will avoid to create a new one if the string is ""
		if ( Text.size() ) {
			CreateTooltip();

			mTooltip->Text( Text );
		}
	} else { // but if it's created, i will allow it
		mTooltip->Text( Text );
	}
}

String cUIComplexControl::TooltipText() {
	if ( NULL != mTooltip )
		return mTooltip->Text();

	return String();
}

void cUIComplexControl::TooltipRemove() {
	mTooltip = NULL;
}

void cUIComplexControl::Size( const Sizei &Size ) {
	Sizei s( Size );

	if ( s.x < mMinControlSize.x )
		s.x = mMinControlSize.x;

	if ( s.y < mMinControlSize.y )
		s.y = mMinControlSize.y;

	cUIControlAnim::Size( s );
}

void cUIComplexControl::Size( const Int32& Width, const Int32& Height ) {
	cUIControlAnim::Size( Width, Height );
}

const Sizei& cUIComplexControl::Size() {
	return cUIControlAnim::Size();
}

void cUIComplexControl::OnParentSizeChange( const Vector2i& SizeChange ) {
	Sizei newSize( mSize );

	if ( mFlags & UI_ANCHOR_LEFT ) {
		// Nothing ?
	} else {
		Pos( mPos.x += SizeChange.x, mPos.y );
	}

	if ( mFlags & UI_ANCHOR_RIGHT ) {
		if ( NULL != mParentCtrl ) {
			newSize.x = mParentCtrl->Size().Width() - mPos.x - mDistToBorder.Right;

			if ( newSize.x < mMinControlSize.Width() )
				newSize.x = mMinControlSize.Width();
		}
	}

	if ( mFlags & UI_ANCHOR_TOP ) {
		// Nothing ?
	} else {
		Pos( mPos.x, mPos.y += SizeChange.y );
	}

	if ( mFlags & UI_ANCHOR_BOTTOM ) {
		if ( NULL != mParentCtrl ) {
			newSize.y = mParentCtrl->Size().y - mPos.y - mDistToBorder.Bottom;

			if ( newSize.y < mMinControlSize.Height() )
				newSize.y = mMinControlSize.Height();
		}
	}

	if ( newSize != mSize )
		Size( newSize );

	cUIControlAnim::OnParentSizeChange( SizeChange );
}

}}
