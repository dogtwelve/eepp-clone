#include <eepp/gaming/clight.hpp>

namespace EE { namespace Gaming {

cLight::cLight() :
	mRadius( 0 ),
	mColor( 255, 255, 255 ),
	mType( LIGHT_NORMAL ),
	mActive( true )
{
}

cLight::~cLight() {
}

cLight::cLight( const Float& Radius, const Float& x, const Float& y, const eeColor& Color, LIGHT_TYPE Type ) :
	mActive( true )
{
	Create( Radius, x, y, Color, Type );
}

void cLight::Create( const Float& Radius, const Float& x, const Float& y, const eeColor& Color, LIGHT_TYPE Type ) {
	mRadius	= Radius;
	mColor	= Color;
	mType	= Type;

	UpdatePos( x, y );
}

eeColor cLight::ProcessVertex( const Float& PointX, const Float& PointY, const eeColor& VertexColor, const eeColor& BaseColor ) {
	Float VertexDist;

	if ( mActive ) {
		if ( mType == LIGHT_NORMAL )
			VertexDist = eeabs( mPos.Distance( eeVector2f( PointX, PointY ) ) );
		else {
			Float XDist = eeabs(mPos.x - PointX) * 0.5f;
			Float YDist = eeabs(mPos.y - PointY);
			VertexDist = eesqrt( XDist * XDist + YDist * YDist ) * 2.0f;
		}

		if ( VertexDist <= mRadius ) {
			eeColor	TmpRGB;
			Uint8	TmpColor;
			Float	LightC;

			LightC			= eeabs( static_cast<Float> ( mColor.R() - BaseColor.R() ) ) / mRadius;
			TmpColor		= Uint8( (Float)mColor.R() - (VertexDist * LightC) );
			TmpRGB.Red		= VertexColor.R() + ( TmpColor - VertexColor.R() );

			LightC			= eeabs( static_cast<Float> ( mColor.G() - BaseColor.G() ) ) / mRadius;
			TmpColor		= Uint8( (Float)mColor.G() - (VertexDist * LightC) );
			TmpRGB.Green	= VertexColor.G() + ( TmpColor - VertexColor.G() );

			LightC			= eeabs( static_cast<Float> ( mColor.B() - BaseColor.B() ) ) / mRadius;
			TmpColor		= Uint8( (Float)mColor.B() - (VertexDist * LightC) );
			TmpRGB.Blue		= VertexColor.B() + ( TmpColor - VertexColor.B() );

			if ( TmpRGB.R() < VertexColor.R() ) TmpRGB.Red		= VertexColor.R();
			if ( TmpRGB.G() < VertexColor.G() ) TmpRGB.Green	= VertexColor.G();
			if ( TmpRGB.B() < VertexColor.B() ) TmpRGB.Blue		= VertexColor.B();
			
			return TmpRGB;
		}
	}

	return BaseColor;
}

eeColorA cLight::ProcessVertex( const Float& PointX, const Float& PointY, const eeColorA& VertexColor, const eeColorA& BaseColor ) {
	Float VertexDist;

	if ( mActive ) {
		if ( mType == LIGHT_NORMAL )
			VertexDist = eeabs( mPos.Distance( eeVector2f( PointX, PointY ) ) );
		else {
			Float XDist = eeabs(mPos.x - PointX) * 0.5f;
			Float YDist = eeabs(mPos.y - PointY);
			VertexDist = eesqrt( XDist * XDist + YDist * YDist ) * 2.0f;
		}

		if ( VertexDist <= mRadius ) {
			eeColorA	TmpRGB;
			Uint8		TmpColor;
			Float		LightC;

			LightC			= eeabs( static_cast<Float> ( mColor.R() - BaseColor.R() ) ) / mRadius;
			TmpColor		= Uint8( (Float)mColor.R() - (VertexDist * LightC) );
			TmpRGB.Red		= VertexColor.R() + ( TmpColor - VertexColor.R() );

			LightC			= eeabs( static_cast<Float> ( mColor.G() - BaseColor.G() ) ) / mRadius;
			TmpColor		= Uint8( (Float)mColor.G() - (VertexDist * LightC) );
			TmpRGB.Green	= VertexColor.G() + ( TmpColor - VertexColor.G() );

			LightC			= eeabs( static_cast<Float> ( mColor.B() - BaseColor.B() ) ) / mRadius;
			TmpColor		= Uint8( (Float)mColor.B() - (VertexDist * LightC) );
			TmpRGB.Blue		= VertexColor.B() + ( TmpColor - VertexColor.B() );

			if ( TmpRGB.R() < VertexColor.R() ) TmpRGB.Red		= VertexColor.R();
			if ( TmpRGB.G() < VertexColor.G() ) TmpRGB.Green	= VertexColor.G();
			if ( TmpRGB.B() < VertexColor.B() ) TmpRGB.Blue		= VertexColor.B();

			return TmpRGB;
		}
	}

	return BaseColor;
}

eeColor cLight::ProcessVertex( const eeVector2f& Pos, const eeColor& VertexColor, const eeColor& BaseColor ) {
	return ProcessVertex( Pos.x, Pos.y, VertexColor, BaseColor );
}

eeColorA cLight::ProcessVertex( const eeVector2f& Pos, const eeColorA& VertexColor, const eeColorA& BaseColor ) {
	return ProcessVertex( Pos.x, Pos.y, VertexColor, BaseColor );
}

void cLight::UpdatePos( const Float& x, const Float& y ) {
	mPos.x = x;
	mPos.y = y;
	UpdateAABB();
}

void cLight::Position( const eeVector2f& newPos ) {
	UpdatePos( newPos.x, newPos.y );
}

void cLight::UpdatePos( const eeVector2f& newPos ) {
	UpdatePos( newPos.x, newPos.y );
}

void cLight::Move( const Float& addtox, const Float& addtoy ) {
	UpdatePos( mPos.x + addtox, mPos.y + addtoy );
}

eeAABB cLight::GetAABB() const {
	return mAABB;
}

void cLight::UpdateAABB() {
	if ( mType == LIGHT_NORMAL )
		mAABB = eeAABB( mPos.x - mRadius, mPos.y - mRadius, mPos.x + mRadius, mPos.y + mRadius );
	else
		mAABB = eeAABB( mPos.x - mRadius, mPos.y - mRadius * 0.5f, mPos.x + mRadius, mPos.y + mRadius * 0.5f );
}

const Float& cLight::Radius() const {
	return mRadius;
}

void cLight::Radius( const Float& radius ) {
	if ( radius > 0 ) {
		mRadius = radius;
		UpdateAABB();
	}
}

const bool& cLight::Active() const {
	return mActive;
}

void cLight::Active( const bool& active ) {
	mActive = active;
}

void cLight::Color( const eeColor& color ) {
	mColor = color;
}

const eeColor& cLight::Color() const {
	return mColor;
}

void cLight::Type( const LIGHT_TYPE& type ) {
	mType = type;
	UpdateAABB();
}

const LIGHT_TYPE& cLight::Type() const {
	return mType;
}

const eeVector2f& cLight::Position() const {
	return mPos;
}

}}
