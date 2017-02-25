#ifndef EE_GAMINGCGAMEOBJECT_HPP
#define EE_GAMINGCGAMEOBJECT_HPP

#include <eepp/gaming/base.hpp>
#include <eepp/gaming/maphelper.hpp>
#include <eepp/gaming/maplayer.hpp>

#include <eepp/graphics/renders.hpp>
using namespace EE::Graphics;

namespace EE { namespace Gaming {

class EE_API GameObject {
	public:
		GameObject( const Uint32& Flags, MapLayer * Layer );

		virtual ~GameObject();

		virtual void draw();

		virtual void update( const Time& dt );

		virtual Vector2f getPosition() const;

		virtual void setPosition( Vector2f pos );

		virtual Vector2i getTilePosition() const;

		virtual void setTilePosition( Vector2i pos );

		virtual Sizei getSize();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type );

		const Uint32& getFlags() const;

		Uint32 getFlag( const Uint32& Flag );

		virtual void setFlag( const Uint32& Flag );

		void clearFlag( const Uint32& Flag );

		bool isBlocked() const;

		void setBlocked( bool blocked );

		bool isRotated() const;

		void setRotated( bool rotated );

		bool isMirrored() const;

		void setMirrored( bool mirrored );

		bool isFliped() const;

		void setFliped( bool fliped );

		virtual Uint32 getDataId();

		virtual void setDataId( Uint32 Id );

		MapLayer * getLayer() const;
	protected:
		Uint32		mFlags;
		MapLayer *	mLayer;

		virtual EE_RENDER_MODE getRenderModeFromFlags();

		void autoFixTilePos();

		void assignTilePos();

		Float getRotation();
};

}}

#endif
