#include <eepp/graphics/arcdrawable.hpp>
#include <eepp/graphics/vertexbuffer.hpp>

namespace EE { namespace Graphics {

ArcDrawable::ArcDrawable() :
	PrimitiveDrawable( DRAWABLE_ARC ),
	mRadius( 0 ),
	mSegmentsCount( 64 ),
	mArcAngle( 360 ),
	mArcStartAngle( 0 )
{
}

ArcDrawable::ArcDrawable( const Float & radius, Uint32 segmentsCount, const Float & arcAngle, const Float & arcStartAngle ) :
	PrimitiveDrawable( DRAWABLE_ARC ),
	mRadius( radius ),
	mSegmentsCount( segmentsCount ),
	mArcAngle( arcAngle ),
	mArcStartAngle( arcStartAngle )
{
	if(mSegmentsCount < 6) mSegmentsCount = 6;
	mSegmentsCount = mSegmentsCount > 360 ? 360 : mSegmentsCount;
}

Sizef ArcDrawable::getSize() {
	return Sizef( mRadius * 2, mRadius * 2 );
}

void ArcDrawable::draw(const Vector2f & position) {
	draw( position, getSize() );
}

void ArcDrawable::draw( const Vector2f& position, const Sizef& size ) {
	if ( size.getWidth() * 0.5f != mRadius ) {
		mRadius = size.getWidth() * 0.5f;
		mNeedsUpdate = true;
	}

	PrimitiveDrawable::draw( position, size );
}

Float ArcDrawable::getRadius() const {
	return mRadius;
}

void ArcDrawable::setRadius(const Float & radius) {
	mRadius = radius;
	mNeedsUpdate = true;
}

Float ArcDrawable::getArcAngle() const {
	return mArcAngle;
}

void ArcDrawable::setArcAngle(const Float & arcAngle) {
	mArcAngle = arcAngle;
	mNeedsUpdate = true;
}

Float ArcDrawable::getArcStartAngle() const {
	return mArcStartAngle;
}

void ArcDrawable::setArcStartAngle(const Float & arcStartAngle) {
	mArcStartAngle = arcStartAngle;
	mNeedsUpdate = true;
}

Uint32 ArcDrawable::getSegmentsCount() const {
	return mSegmentsCount;
}

void ArcDrawable::setSegmentsCount(const Uint32 & segmentsCount) {
	mSegmentsCount = segmentsCount;
	if(mSegmentsCount < 6) mSegmentsCount = 6;
	mSegmentsCount = mSegmentsCount > 360 ? 360 : mSegmentsCount;
	mNeedsUpdate = true;
}

void ArcDrawable::updateVertex() {
	prepareVertexBuffer( mFillMode == DRAW_LINE ? DM_LINE_STRIP : DM_TRIANGLE_FAN );

	Float angleShift =  360 / static_cast<Float>(mSegmentsCount);
	Float arcAngleA = mArcAngle > 360 ? mArcAngle - 360 * std::floor( mArcAngle / 360 ) : mArcAngle;

	switch( mFillMode ) {
		case DRAW_LINE:
		{
			Uint32 segmentsCount = Uint32( (Float)mSegmentsCount * (Float)eeabs( arcAngleA ) / 360 );
			Float startAngle = Math::radians(mArcStartAngle);
			Float theta = Math::radians(arcAngleA) / Float(segmentsCount - 1);
			Float tangetialFactor = eetan(theta);
			Float radialFactor = eecos(theta);
			Float x = mRadius * eecos(startAngle);
			Float y = mRadius * eesin(startAngle);

			for( Uint32 ii = 0; ii < segmentsCount; ii++ ) {
				mVertexBuffer->addVertex( Vector2f( x + mPosition.x, y + mPosition.y ) );
				mVertexBuffer->addColor( mColor );

				Float tx = -y;
				Float ty = x;

				x += tx * tangetialFactor;
				y += ty * tangetialFactor;

				x *= radialFactor;
				y *= radialFactor;
			}

			break;
		}
		case DRAW_FILL:
		{
			for( Float i = 0; i < arcAngleA; i+= angleShift ) {
				Float startAngle = mArcStartAngle + i;

				mVertexBuffer->addVertex( Vector2f( mPosition.x , mPosition.y ) );
				mVertexBuffer->addColor( mColor );

				mVertexBuffer->addVertex( Vector2f( mPosition.x + mRadius * Math::sinAng( startAngle ), mPosition.y + mRadius * Math::cosAng( startAngle ) ) );
				mVertexBuffer->addColor( mColor );

				mVertexBuffer->addVertex( Vector2f( mPosition.x + mRadius * Math::sinAng( startAngle + angleShift ), mPosition.y + mRadius * Math::cosAng( startAngle + angleShift ) ) );
				mVertexBuffer->addColor( mColor );
			}

			break;
		}
	}

	mNeedsUpdate = false;
}

}}
