#include <eepp/physics/constraints/dampedspring.hpp>

#ifdef PHYSICS_RENDERER_ENABLED
#include <eepp/graphics/glextensions.hpp>
#include <eepp/graphics/renderer/gl.hpp>
#include <eepp/graphics/globalbatchrenderer.hpp>
using namespace EE::Graphics;
#endif


CP_NAMESPACE_BEGIN

DampedSpring::DampedSpring( Body * a, Body * b, cVect anchr1, cVect anchr2, cpFloat restLength, cpFloat stiffness, cpFloat damping )
#ifdef PHYSICS_RENDERER_ENABLED
	: mDrawPointSize( 5.f )
#endif
{
	mConstraint = cpDampedSpringNew( a->GetBody(), b->GetBody(), tocpv( anchr1 ), tocpv( anchr2 ), restLength, stiffness, damping );
	SetData();
}

cVect DampedSpring::Anchr1() {
	return tovect( cpDampedSpringGetAnchr1( mConstraint ) );
}

void DampedSpring::Anchr1( const cVect& anchr1 ) {
	cpDampedSpringSetAnchr1( mConstraint, tocpv( anchr1 ) );
}

cVect DampedSpring::Anchr2() {
	return tovect( cpDampedSpringGetAnchr2( mConstraint ) );
}

void DampedSpring::Anchr2( const cVect& anchr2 ) {
	cpDampedSpringSetAnchr2( mConstraint, tocpv( anchr2 ) );
}

cpFloat DampedSpring::RestLength() {
	return cpDampedSpringGetRestLength( mConstraint );
}

void DampedSpring::RestLength( const cpFloat& restlength ) {
	cpDampedSpringSetRestLength( mConstraint, restlength );
}

cpFloat DampedSpring::Stiffness() {
	return cpDampedSpringGetStiffness( mConstraint );
}

void DampedSpring::Stiffness( const cpFloat& stiffness ) {
	cpDampedSpringSetStiffness( mConstraint, stiffness );
}

cpFloat DampedSpring::Damping() {
	return cpDampedSpringGetDamping( mConstraint );
}

void DampedSpring::Damping( const cpFloat& damping ) {
	cpDampedSpringSetDamping( mConstraint, damping );
}

void DampedSpring::Draw() {
	#ifdef PHYSICS_RENDERER_ENABLED
	static const float springVAR[] = {
		0.00f, 0.0f,
		0.20f, 0.0f,
		0.25f, 3.0f,
		0.30f,-6.0f,
		0.35f, 6.0f,
		0.40f,-6.0f,
		0.45f, 6.0f,
		0.50f,-6.0f,
		0.55f, 6.0f,
		0.60f,-6.0f,
		0.65f, 6.0f,
		0.70f,-3.0f,
		0.75f, 6.0f,
		0.80f, 0.0f,
		1.00f, 0.0f,
	};
	static const int springVAR_count = sizeof(springVAR)/sizeof(float)/2;

	if ( mDrawPointSize <= 0 )
		return;

	cpDampedSpring * spring = (cpDampedSpring*)mConstraint;
	cpBody * body_a = mConstraint->a;
	cpBody * body_b = mConstraint->b;

	cVect a = tovect( cpvadd(body_a->p, cpvrotate(spring->anchr1, body_a->rot)) );
	cVect b = tovect( cpvadd(body_b->p, cpvrotate(spring->anchr2, body_b->rot)) );

	GLi->PointSize( mDrawPointSize );

	BatchRenderer * BR = GlobalBatchRenderer::instance();
	BR->SetTexture( NULL );
	BR->PointsBegin();
	BR->BatchPoint( a.x, a.y );
	BR->BatchPoint( b.x, b.y );
	BR->Draw();

	cVect delta = b - a;

	GLi->Disable( GL_TEXTURE_2D );
	GLi->DisableClientState( GL_TEXTURE_COORD_ARRAY );

	std::vector<ColorA> tcolors( springVAR_count * 4, ColorA( 0, 255, 0, 255 ) );
	GLi->ColorPointer( 4, GL_UNSIGNED_BYTE, 0, reinterpret_cast<const void*>( &tcolors[0] ), springVAR_count * 4 );
	GLi->VertexPointer( 2, GL_FLOAT, 0, springVAR, springVAR_count * sizeof(float) * 2 );

	GLi->PushMatrix();

	float x = a.x;
	float y = a.y;
	float cos = delta.x;
	float sin = delta.y;
	float s = 1.0f / cpvlength( tocpv( delta ) );

	const float matrix[] = {
		cos		, sin		, 0.0f, 0.0f,
		-sin * s, cos * s	, 0.0f, 0.0f,
		0.0f	, 0.0f		, 1.0f, 0.0f,
		 x		, y			, 0.0f, 1.0f,
	};
	GLi->MultMatrixf( matrix );

	GLi->DrawArrays( GL_LINE_STRIP, 0, springVAR_count );

	GLi->PopMatrix();

	GLi->Enable( GL_TEXTURE_2D );
	GLi->EnableClientState( GL_TEXTURE_COORD_ARRAY );
	#endif
}


#ifdef PHYSICS_RENDERER_ENABLED
cpFloat DampedSpring::DrawPointSize() {
	return mDrawPointSize;
}

void DampedSpring::DrawPointSize( const cpFloat& size ) {
	mDrawPointSize = size;
}
#endif

CP_NAMESPACE_END
