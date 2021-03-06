#ifndef EE_PHYSICS_CSHAPEPOINT_HPP
#define EE_PHYSICS_CSHAPEPOINT_HPP

#include <eepp/physics/shape.hpp>

CP_NAMESPACE_BEGIN

class CP_API ShapePoint : public Shape {
	public:
		static ShapePoint * New( Physics::Body * body, cpFloat radius, cVect offset );

		ShapePoint( Physics::Body * body, cpFloat radius, cVect offset );

		cVect Offset();

		virtual void Offset( const cVect& offset );

		cpFloat Radius();

		virtual void Radius( const cpFloat& radius );

		virtual void Draw( Space * space );

		#ifdef PHYSICS_RENDERER_ENABLED
		cpFloat DrawRadius();

		virtual void DrawRadius( const cpFloat& radius );
	protected:
		cpFloat mDrawRadius;
		#endif
};

CP_NAMESPACE_END

#endif
