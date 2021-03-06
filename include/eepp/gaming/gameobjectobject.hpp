#ifndef EE_GAMINGCGAMEOBJECTOBJECT_HPP
#define EE_GAMINGCGAMEOBJECTOBJECT_HPP

#include <eepp/gaming/base.hpp>
#include <eepp/gaming/gameobject.hpp>
#include <eepp/graphics/subtexture.hpp>
using namespace EE::Graphics;

namespace EE { namespace Gaming {

class MapLayer;

class EE_API GameObjectObject : public GameObject {
	public:
		typedef std::map<std::string, std::string> PropertiesMap;

		GameObjectObject( Uint32 DataId, const Rectf& rect, MapLayer * Layer, const Uint32& Flags = GObjFlags::GAMEOBJECT_STATIC );

		virtual ~GameObjectObject();

		virtual void Draw();

		virtual Vector2f Pos() const;

		virtual Sizei Size();

		virtual void Pos( Vector2f pos );

		virtual Uint32 Type() const;

		virtual bool IsType( const Uint32& type );

		virtual Uint32 DataId();

		virtual void DataId( Uint32 Id );

		void AddProperty( std::string Text, std::string Value );

		void EditProperty( std::string Text, std::string Value );

		void RemoveProperty( std::string Text );

		void SetProperties( const PropertiesMap& prop );

		void ClearProperties();

		PropertiesMap& GetProperties();

		Uint32 GetPropertyCount();

		const std::string& Name() const;

		void Name( const std::string& name );

		const std::string& TypeName() const;

		void TypeName( const std::string& type );

		virtual bool PointInside( const Vector2f& p );

		Polygon2f& GetPolygon();

		const bool& Selected() const;

		void Selected( const bool& sel );

		virtual void SetPolygonPoint( Uint32 index, Vector2f p );

		virtual GameObjectObject * Copy();
	protected:
		Rectf			mRect;
		Polygon2f		mPoly;
		Vector2f		mPos;
		Uint32			mDataId;
		bool			mSelected;
		std::string		mName;
		std::string		mType;
		PropertiesMap	mProperties;
};

class GameObjectPolyData {
	public:
		std::string		Name;
		std::string		Type;
		GameObjectObject::PropertiesMap	Properties;
		Polygon2f		Poly;
};

}}

#endif
