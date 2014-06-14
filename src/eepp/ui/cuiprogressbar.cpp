#include <eepp/ui/cuiprogressbar.hpp>

namespace EE { namespace UI {

cUIProgressBar::cUIProgressBar( const cUIProgressBar::CreateParams& Params ) :
	cUIComplexControl( Params ),
	mVerticalExpand( Params.VerticalExpand ),
	mSpeed( Params.MovementSpeed ),
	mFillerMargin( Params.FillerMargin ),
	mDisplayPercent( Params.DisplayPercent ),
	mProgress( 0.f ),
	mTotalSteps( 100.f ),
	mParallax( NULL )
{
	cUITextBox::CreateParams TxtBoxParams = Params;

	TxtBoxParams.Parent( this );
	TxtBoxParams.Flags = UI_VALIGN_CENTER | UI_HALIGN_CENTER;
	TxtBoxParams.PosSet( 0, 0 );

	mTextBox = eeNew( cUITextBox, ( TxtBoxParams ) );
	mTextBox->Enabled( false );

	UpdateTextBox();

	ApplyDefaultTheme();
}

cUIProgressBar::~cUIProgressBar() {
	eeSAFE_DELETE( mParallax );
}

Uint32 cUIProgressBar::Type() const {
	return UI_TYPE_PROGRESSBAR;
}

bool cUIProgressBar::IsType( const Uint32& type ) const {
	return cUIProgressBar::Type() == type ? true : cUIComplexControl::IsType( type );
}

void cUIProgressBar::Draw() {
	cUIControlAnim::Draw();

	if ( NULL != mParallax && 0.f != mAlpha ) {
		ColorA C( mParallax->Color() );
		C.Alpha = (Uint8)mAlpha;

		mParallax->Color( C );
		mParallax->Position( eeVector2f( mScreenPos.x + mFillerMargin.Left, mScreenPos.y + mFillerMargin.Top ) );
		mParallax->Draw();
	}
}

void cUIProgressBar::SetTheme( cUITheme * Theme ) {
	cUIControl::SetThemeControl( Theme, "progressbar" );

	if ( mFlags & UI_AUTO_SIZE ) {
		Size( mSize.x, GetSkinSize().Height() );
	}

	cUISkin * tSkin = Theme->GetByName( Theme->Abbr() + "_progressbar_filler" );

	if ( tSkin ) {
		cSubTexture * tSubTexture = tSkin->GetSubTexture( cUISkinState::StateNormal );

		if ( NULL != tSubTexture ) {
			eeSAFE_DELETE( mParallax );

			Float Height = (Float)GetSkinSize().Height();

			if ( !mVerticalExpand )
				Height = (Float)tSubTexture->RealSize().Height();

			if ( Height > mSize.Height() )
				Height = mSize.Height();

			mParallax = eeNew( cScrollParallax, ( tSubTexture, eeVector2f( mScreenPos.x + mFillerMargin.Left, mScreenPos.y + mFillerMargin.Top ), eeSizef( ( ( mSize.Width() - mFillerMargin.Left - mFillerMargin.Right ) * mProgress ) / mTotalSteps, Height - mFillerMargin.Top - mFillerMargin.Bottom ), mSpeed ) );
		}
	}
}

Uint32 cUIProgressBar::OnValueChange() {
	cUIControlAnim::OnValueChange();

	OnSizeChange();

	return 1;
}

void cUIProgressBar::OnSizeChange() {
	if ( NULL != mParallax ) {
		Float Height = (Float)mSize.Height();

		if ( !mVerticalExpand && mParallax->SubTexture() )
			Height = (Float)mParallax->SubTexture()->RealSize().Height();

		if ( Height > mSize.Height() )
			Height = mSize.Height();

		mParallax->Size( eeSizef( ( ( mSize.Width() - mFillerMargin.Left - mFillerMargin.Right ) * mProgress ) / mTotalSteps, Height - mFillerMargin.Top - mFillerMargin.Bottom ) );
	}

	UpdateTextBox();
}

void cUIProgressBar::Progress( Float Val ) {
	mProgress = Val;

	OnValueChange();
	UpdateTextBox();
}

const Float& cUIProgressBar::Progress() const {
	return mProgress;
}

void cUIProgressBar::TotalSteps( const Float& Steps ) {
	mTotalSteps = Steps;

	OnSizeChange();
	UpdateTextBox();
}

const Float& cUIProgressBar::TotalSteps() const {
	return mTotalSteps;
}

void cUIProgressBar::MovementSpeed( const eeVector2f& Speed ) {
	mSpeed = Speed;

	if ( NULL != mParallax )
		mParallax->Speed( mSpeed );
}

const eeVector2f& cUIProgressBar::MovementSpeed() const {
	return mSpeed;
}

void cUIProgressBar::VerticalExpand( const bool& VerticalExpand ) {
	if ( VerticalExpand != mVerticalExpand ) {
		mVerticalExpand = VerticalExpand;

		OnSizeChange();
	}
}

const bool& cUIProgressBar::VerticalExpand() const {
	return mVerticalExpand;
}

void cUIProgressBar::FillerMargin( const eeRectf& margin ) {
	mFillerMargin = margin;

	OnPosChange();
	OnSizeChange();
}

const eeRectf& cUIProgressBar::FillerMargin() const {
	return mFillerMargin;
}

void cUIProgressBar::DisplayPercent( const bool& DisplayPercent ) {
	mDisplayPercent = DisplayPercent;

	UpdateTextBox();
}

const bool& cUIProgressBar::DisplayPercent() const {
	return mDisplayPercent;
}

void cUIProgressBar::UpdateTextBox() {
	mTextBox->Visible( mDisplayPercent );
	mTextBox->Size( mSize );
	mTextBox->Text( String::ToStr( (Int32)( ( mProgress / mTotalSteps ) * 100.f ) ) + "%" );
}

cUITextBox * cUIProgressBar::TextBox() const {
	return mTextBox;
}

void cUIProgressBar::OnAlphaChange() {
	cUIControlAnim::OnAlphaChange();
	
	mTextBox->Alpha( mAlpha );
}

}}
