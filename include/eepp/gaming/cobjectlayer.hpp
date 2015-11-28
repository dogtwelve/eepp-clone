#ifndef EE_GAMINGCOBJECTLAYER_HPP
#define EE_GAMINGCOBJECTLAYER_HPP

#include <eepp/gaming/clayer.hpp>
#include <eepp/gaming/cgameobject.hpp>
#include <list>

namespace EE { namespace Gaming {

class cMap;

class EE_API cObjectLayer : public cLayer {
	public:
		enum SEARCH_TYPE {
			SEARCH_OBJECT = 1,
			SEARCH_POLY,
			SEARCH_ALL
		};

		typedef std::list<cGameObject*> ObjList;

		virtual ~cObjectLayer();

		virtual void Draw( const eeVector2f &Offset = eeVector2f(0,0) );

		virtual void Update();

		virtual void AddGameObject( cGameObject * obj );

		virtual void RemoveGameObject( cGameObject * obj );

		virtual void RemoveGameObject( const eeVector2i& pos );

		virtual cGameObject * GetObjectOver( const eeVector2i& pos, SEARCH_TYPE type = SEARCH_ALL );

		virtual Uint32 GetObjectCount() const;
	protected:
		friend class cMap;

		ObjList		mObjects;

		cObjectLayer( cMap * map, Uint32 flags, std::string name = "", eeVector2f offset = eeVector2f(0,0) );

		void AllocateLayer();

		void DeallocateLayer();

		ObjList& GetObjectList();
};

}}

#endif
