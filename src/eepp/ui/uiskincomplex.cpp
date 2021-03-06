#include <eepp/ui/uiskincomplex.hpp>
#include <eepp/graphics/textureatlasmanager.hpp>

namespace EE { namespace UI {

static const char SideSuffix[ UISkinComplex::SideCount ][4] = {
	"ml", "mr","d","u","ul","ur","dl","dr","m"
};

std::string UISkinComplex::GetSideSuffix( const Uint32& Side ) {
	eeASSERT( Side < UISkinComplex::SideCount );

	return std::string( SideSuffix[ Side ] );
}

UISkinComplex::UISkinComplex( const std::string& Name ) :
	UISkin( Name, SkinComplex )
{
	for ( Int32 x = 0; x < UISkinState::StateCount; x++ )
		for ( Int32 y = 0; y < SideCount; y++ )
			mSubTexture[ x ][ y ] = NULL;

	SetSkins();
}

UISkinComplex::~UISkinComplex() {

}

void UISkinComplex::Draw( const Float& X, const Float& Y, const Float& Width, const Float& Height, const Uint32& Alpha, const Uint32& State ) {
	if ( 0 == Alpha )
		return;

	SubTexture * tSubTexture = mSubTexture[ State ][ UpLeft ];
	mTempColor		= mColor[ State ];

	if ( mTempColor.Alpha != Alpha ) {
		mTempColor.Alpha = (Uint8)( (Float)mTempColor.Alpha * ( (Float)Alpha / 255.f ) );
	}

	Sizei uls;

	if ( NULL != tSubTexture ) {
		uls = tSubTexture->RealSize();

		tSubTexture->Draw( X, Y, mTempColor );
	}

	tSubTexture = mSubTexture[ State ][ DownLeft ];

	Sizei dls;

	if ( NULL != tSubTexture ) {
		dls = tSubTexture->RealSize();

		tSubTexture->Draw( X, Y + Height - dls.Height(), mTempColor );
	}

	tSubTexture = mSubTexture[ State ][ UpRight ];

	Sizei urs;

	if ( NULL != tSubTexture ) {
		urs = tSubTexture->RealSize();

		tSubTexture->Draw( X + Width - urs.Width(), Y, mTempColor );
	}

	tSubTexture = mSubTexture[ State ][ DownRight ];

	Sizei drs;

	if ( NULL != tSubTexture ) {
		drs = tSubTexture->RealSize();

		tSubTexture->Draw( X + Width - drs.Width(), Y + Height - drs.Height(), mTempColor );
	}

	tSubTexture = mSubTexture[ State ][ Left ];

	if ( NULL != tSubTexture ) {
		tSubTexture->DestSize( Sizef( tSubTexture->DestSize().x, Height - uls.Height() - dls.Height() ) );

		tSubTexture->Draw( X, Y + uls.Height(), mTempColor );

		tSubTexture->ResetDestSize();

		if ( uls.Width() == 0 )
			uls.x = tSubTexture->RealSize().Width();
	}

	tSubTexture = mSubTexture[ State ][ Up ];

	if ( NULL != tSubTexture ) {
		tSubTexture->DestSize( Sizef( Width - uls.Width() - urs.Width(), tSubTexture->DestSize().y ) );

		tSubTexture->Draw( X + uls.Width(), Y, mTempColor );

		tSubTexture->ResetDestSize();

		if ( urs.Height() == 0 )
			urs.y = tSubTexture->RealSize().Height();

		if ( uls.Height() == 0 )
			uls.y = tSubTexture->RealSize().Height();
	}

	tSubTexture = mSubTexture[ State ][ Right ];

	if ( NULL != tSubTexture ) {
		if ( urs.Width() == 0 )
			urs.x = tSubTexture->RealSize().Width();

		tSubTexture->DestSize( Sizef( tSubTexture->DestSize().x, Height - urs.Height() - drs.Height() ) );

		tSubTexture->Draw( X + Width - tSubTexture->RealSize().Width(), Y + urs.Height(), mTempColor );

		tSubTexture->ResetDestSize();
	}

	tSubTexture = mSubTexture[ State ][ Down ];

	if ( NULL != tSubTexture ) {
		tSubTexture->DestSize( Sizef( Width - dls.Width() - drs.Width(), tSubTexture->DestSize().y ) );

		tSubTexture->Draw( X + dls.Width(), Y + Height - tSubTexture->RealSize().Height(), mTempColor );

		tSubTexture->ResetDestSize();

		if ( dls.Height() == 0 && drs.Height() == 0 )
			dls.Height( tSubTexture->RealSize().Height() );
	}

	tSubTexture = mSubTexture[ State ][ Center ];

	if ( NULL != tSubTexture ) {
		tSubTexture->DestSize( Sizef( Width - uls.Width() - urs.Width(), Height - uls.Height() - dls.Height() ) );

		tSubTexture->Draw( X + uls.Width(), Y + uls.Height(), mTempColor );

		tSubTexture->ResetDestSize();
	}
}

void UISkinComplex::SetSkin( const Uint32& State ) {
	eeASSERT ( State < UISkinState::StateCount );

	for ( Uint32 Side = 0; Side < SideCount; Side++ ) {

		SubTexture * SubTexture = TextureAtlasManager::instance()->GetSubTextureByName( std::string( mName + "_" + UISkin::GetSkinStateName( State ) + "_" + SideSuffix[ Side ] ) );

		if ( NULL != SubTexture )
			mSubTexture[ State ][ Side ] = SubTexture;
	}
}

SubTexture * UISkinComplex::GetSubTexture( const Uint32& State ) const {
	eeASSERT ( State < UISkinState::StateCount );

	return mSubTexture[ State ][ Center ];
}

SubTexture * UISkinComplex::GetSubTextureSide( const Uint32& State, const Uint32& Side ) {
	eeASSERT ( State < UISkinState::StateCount && Side < UISkinComplex::SideCount );

	return mSubTexture[ State ][ Side ];
}

void UISkinComplex::StateNormalToState( const Uint32& State ) {
	if ( NULL == mSubTexture[ State ][ 0 ] ) {
		for ( Uint32 Side = 0; Side < SideCount; Side++ ) {
			mSubTexture[ State ][ Side ] = mSubTexture[ UISkinState::StateNormal ][ Side ];
		}
	}
}

UISkinComplex * UISkinComplex::Copy( const std::string& NewName, const bool& CopyColorsState ) {
	UISkinComplex * SkinC = eeNew( UISkinComplex, ( NewName ) );

	if ( CopyColorsState ) {
		SkinC->mColorDefault = mColorDefault;

		memcpy( &SkinC->mColor[0], &mColor[0], UISkinState::StateCount * sizeof(ColorA) );
	}

	memcpy( &SkinC->mSubTexture[0], &mSubTexture[0], UISkinState::StateCount * SideCount * sizeof(SubTexture*) );

	return SkinC;
}

UISkin * UISkinComplex::Copy() {
	return Copy( mName, true );
}

}}
