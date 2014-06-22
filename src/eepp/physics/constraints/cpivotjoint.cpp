#include <eepp/physics/constraints/cpivotjoint.hpp>

#ifdef PHYSICS_RENDERER_ENABLED
#include <eepp/graphics/globalbatchrenderer.hpp>
using namespace EE::Graphics;
#endif

CP_NAMESPACE_BEGIN

cPivotJoint::cPivotJoint( cBody * a, cBody * b, cVect pivot )
#ifdef PHYSICS_RENDERER_ENABLED
	: mDrawPointSize( 10.f )
#endif
{
	mConstraint = cpPivotJointNew( a->Body(), b->Body(), tocpv( pivot ) );
	SetData();
}

cPivotJoint::cPivotJoint( cBody * a, cBody * b, cVect anchr1, cVect anchr2 )
#ifdef PHYSICS_RENDERER_ENABLED
	: mDrawPointSize( 10.f )
#endif
{
	mConstraint = cpPivotJointNew2( a->Body(), b->Body(), tocpv( anchr1 ), tocpv( anchr2 ) );
	SetData();
}

cVect cPivotJoint::Anchr1() {
	return tovect( cpPivotJointGetAnchr1( mConstraint ) );
}

void cPivotJoint::Anchr1( const cVect& anchr1 ) {
	cpPivotJointSetAnchr1( mConstraint, tocpv( anchr1 ) );
}

cVect cPivotJoint::Anchr2() {
	return tovect( cpPivotJointGetAnchr2( mConstraint ) );
}

void cPivotJoint::Anchr2( const cVect& anchr2 ) {
	cpPivotJointSetAnchr2( mConstraint, tocpv( anchr2 ) );
}

void cPivotJoint::Draw() {
	#ifdef PHYSICS_RENDERER_ENABLED
	if ( mDrawPointSize <= 0 )
		return;

	cpBody * body_a		= mConstraint->a;
	cpBody * body_b		= mConstraint->b;
	cpPivotJoint* joint	= (cpPivotJoint *)mConstraint;
	cVect a				= tovect( cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot)) );
	cVect b				= tovect( cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot)) );
	BatchRenderer * BR = GlobalBatchRenderer::instance();

	cpFloat ps = BR->GetPointSize();
	BR->SetTexture( NULL );
	BR->SetPointSize( mDrawPointSize );
	BR->PointsBegin();
	BR->PointSetColor( ColorA( 128, 255, 128, 255 ) );
	BR->BatchPoint( a.x, a.y );
	BR->BatchPoint( b.x, b.y );
	BR->Draw();
	BR->SetPointSize( ps );
	#endif
}

#ifdef PHYSICS_RENDERER_ENABLED
cpFloat cPivotJoint::DrawPointSize() {
	return mDrawPointSize;
}

void cPivotJoint::DrawPointSize( const cpFloat& size ) {
	mDrawPointSize = size;
}
#endif

CP_NAMESPACE_END
