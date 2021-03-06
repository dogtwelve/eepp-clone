#include <eepp/physics/shapesegment.hpp>
#include <eepp/physics/space.hpp>
#include <eepp/helper/chipmunk/chipmunk_unsafe.h>

#ifdef PHYSICS_RENDERER_ENABLED
#include <eepp/graphics/glextensions.hpp>
#include <eepp/graphics/renderer/gl.hpp>
#include <eepp/graphics/primitives.hpp>
using namespace EE::Graphics;
#endif

CP_NAMESPACE_BEGIN

ShapeSegment * ShapeSegment::New( Physics::Body * body, cVect a, cVect b, cpFloat radius ) {
	return cpNew( ShapeSegment, ( body, a, b, radius ) );
}

ShapeSegment::ShapeSegment( Physics::Body * body, cVect a, cVect b, cpFloat radius ) {
	mShape = cpSegmentShapeNew( body->GetBody(), tocpv( a ), tocpv( b ), radius );
	SetData();
}

cVect ShapeSegment::A() {
	return tovect( cpSegmentShapeGetA( mShape ) );
}

cVect ShapeSegment::B() {
	return tovect( cpSegmentShapeGetB( mShape ) );
}

cVect ShapeSegment::Normal() {
	return tovect( cpSegmentShapeGetNormal( mShape ) );
}

cpFloat ShapeSegment::Radius() {
	return cpSegmentShapeGetRadius( mShape );
}

void ShapeSegment::Radius( const cpFloat& radius ) {
	cpSegmentShapeSetRadius( mShape, radius );
}

void ShapeSegment::Endpoints( const cVect& a, const cVect& b ) {
	cpSegmentShapeSetEndpoints( mShape, tocpv( a ), tocpv( b ) );
}

bool ShapeSegment::Query( cVect a, cVect b, cpSegmentQueryInfo * info ) {
	return 0 != cpShapeSegmentQuery( mShape, tocpv( a ), tocpv( b ), info );
}

cVect ShapeSegment::QueryHitPoint( const cVect start, const cVect end, const cpSegmentQueryInfo info ) {
	return tovect( cpSegmentQueryHitPoint( tocpv( start ), tocpv( end ), info ) );
}

cpFloat ShapeSegment::QueryHitDist( const cVect start, const cVect end, const cpSegmentQueryInfo info ) {
	return cpSegmentQueryHitDist( tocpv( start ), tocpv( end ), info );
}

void ShapeSegment::Draw( Space * space ) {
	#ifdef PHYSICS_RENDERER_ENABLED
	static const float pillVAR[] = {
		 0.0000f,  1.0000f, 1.0f,
		 0.2588f,  0.9659f, 1.0f,
		 0.5000f,  0.8660f, 1.0f,
		 0.7071f,  0.7071f, 1.0f,
		 0.8660f,  0.5000f, 1.0f,
		 0.9659f,  0.2588f, 1.0f,
		 1.0000f,  0.0000f, 1.0f,
		 0.9659f, -0.2588f, 1.0f,
		 0.8660f, -0.5000f, 1.0f,
		 0.7071f, -0.7071f, 1.0f,
		 0.5000f, -0.8660f, 1.0f,
		 0.2588f, -0.9659f, 1.0f,
		 0.0000f, -1.0000f, 1.0f,

		 0.0000f, -1.0000f, 0.0f,
		-0.2588f, -0.9659f, 0.0f,
		-0.5000f, -0.8660f, 0.0f,
		-0.7071f, -0.7071f, 0.0f,
		-0.8660f, -0.5000f, 0.0f,
		-0.9659f, -0.2588f, 0.0f,
		-1.0000f, -0.0000f, 0.0f,
		-0.9659f,  0.2588f, 0.0f,
		-0.8660f,  0.5000f, 0.0f,
		-0.7071f,  0.7071f, 0.0f,
		-0.5000f,  0.8660f, 0.0f,
		-0.2588f,  0.9659f, 0.0f,
		 0.0000f,  1.0000f, 0.0f,
	};
	static const int pillVAR_count = sizeof(pillVAR)/sizeof(float)/3;

	cpSegmentShape * seg = (cpSegmentShape *)mShape;
	cVect a = tovect( seg->CP_PRIVATE(ta) );
	cVect b = tovect( seg->CP_PRIVATE(tb) );

	if ( seg->CP_PRIVATE(r) ) {
		GLi->Disable( GL_TEXTURE_2D );
		GLi->DisableClientState( GL_TEXTURE_COORD_ARRAY );

		std::vector<ColorA> tcolors( pillVAR_count * 4 );

		GLi->PushMatrix();

		cVect d = b - a;
		cVect r = d * ( seg->CP_PRIVATE(r) / cpvlength( tocpv( d ) ) );

		const float matrix[] = {
			(float)r.x	, (float)r.y, 0.0f, 0.0f,
			(float)-r.y	, (float)r.x, 0.0f, 0.0f,
			(float)d.x	, (float)d.y, 0.0f, 0.0f,
			(float)a.x	, (float)a.y, 0.0f, 1.0f,
		};

		GLi->MultMatrixf( matrix );

		GLi->VertexPointer( 3, GL_FLOAT, 0, pillVAR, pillVAR_count * sizeof(float) * 3 );

		if( !seg->CP_PRIVATE(shape).sensor ) {
			ColorA C = ColorForShape( mShape, space->GetSpace() );

			tcolors.assign( tcolors.size(), C );

			GLi->ColorPointer( 4, GL_UNSIGNED_BYTE, 0, reinterpret_cast<const void*>( &tcolors[0] ), pillVAR_count * 4 );

			GLi->DrawArrays( GL_TRIANGLE_FAN, 0, pillVAR_count );
		}

		tcolors.assign( tcolors.size(), ColorA( 102, 102, 102, 255 ) );

		GLi->ColorPointer( 4, GL_UNSIGNED_BYTE, 0, reinterpret_cast<const void*>( &tcolors[0] ), pillVAR_count *  4 );

		GLi->DrawArrays( GL_LINE_LOOP, 0, pillVAR_count );

		GLi->PopMatrix();

		GLi->Enable( GL_TEXTURE_2D );
		GLi->EnableClientState( GL_TEXTURE_COORD_ARRAY );
	} else {
		Primitives p;
		p.DrawLine( Line2f( Vector2f( a.x, a.y ), Vector2f( b.x, b.y ) ) );
	}
	#endif
}

CP_NAMESPACE_END
