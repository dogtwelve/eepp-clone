#ifndef EE_PHYSICS_PHYSICSMANAGER_HPP
#define EE_PHYSICS_PHYSICSMANAGER_HPP

#include <eepp/physics/base.hpp>

CP_NAMESPACE_BEGIN

class cBody;
class cShape;
class cConstraint;
class cSpace;

class CP_API cPhysicsManager {
	SINGLETON_DECLARE_HEADERS(cPhysicsManager)

	public:
		class cDrawSpaceOptions {
			public:
				cDrawSpaceOptions() :
					DrawBBs( false ),
					DrawShapes( true ),
	#ifdef EE_GLES
					DrawShapesBorders( false ),
	#else
					DrawShapesBorders( true ),
	#endif
					CollisionPointSize( 0.0f ),
					BodyPointSize( 0.0f ),
					LineThickness( 0.0f )
				{}

				bool	DrawBBs;
				bool	DrawShapes;
				bool	DrawShapesBorders;
				cpFloat CollisionPointSize;
				cpFloat BodyPointSize;
				cpFloat LineThickness;
		};

		~cPhysicsManager();

		/** The Memory Manager will keep track of all the allocations from cSpace, cBody, cShape and cConstraint and will release any non-released pointer.
		***	This is a lazy deallocation for the lazy programmers. It is disabled by default.
		*** To work properly set as active before allocating anything, activate it just after the singleton instantiation.
		*/
		void MemoryManager( bool MemoryManager );

		const bool& MemoryManager() const;

		cPhysicsManager::cDrawSpaceOptions * GetDrawOptions();
	protected:
		cDrawSpaceOptions	mOptions;

		friend class cBody;
		friend class cShape;
		friend class cConstraint;
		friend class cSpace;

		bool						mMemoryManager;
		std::list<cBody *>			mBodysFree;
		std::list<cShape *>			mShapesFree;
		std::list<cConstraint *>	mConstraintFree;
		std::list<cSpace*>			mSpaces;

		cPhysicsManager();

		void AddBodyFree( cBody * body );

		void RemoveBodyFree( cBody * body );

		void AddShapeFree( cShape * shape );

		void RemoveShapeFree( cShape * shape );

		void AddConstraintFree( cConstraint * constraint );

		void RemoveConstraintFree( cConstraint * constraint );

		void AddSpace( cSpace * space );

		void RemoveSpace( cSpace * space );
};

CP_NAMESPACE_END

#endif
