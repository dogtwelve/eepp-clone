#ifndef EE_GAMINGCGAMEOBJECTSHAPE_HPP
#define EE_GAMINGCGAMEOBJECTSHAPE_HPP

#include <eepp/gaming/base.hpp>
#include <eepp/gaming/cgameobject.hpp>

#include <eepp/graphics/cshape.hpp>
using namespace EE::Graphics;

namespace EE { namespace Gaming {

class EE_API cGameObjectShape : public cGameObject {
	public:
		cGameObjectShape( const Uint32& Flags, cLayer * Layer, cShape * Shape = NULL, const eeVector2f& Pos = eeVector2f() );

		virtual ~cGameObjectShape();

		virtual void Draw();

		virtual eeVector2f Pos() const;

		virtual void Pos( eeVector2f pos );

		virtual eeVector2i TilePos() const;

		virtual void TilePos( eeVector2i pos );

		virtual eeSize Size();

		cShape * Shape() const;

		void Shape( cShape * shape );

		virtual Uint32 Type() const;

		virtual bool IsType( const Uint32& type );

		virtual Uint32 DataId();

		virtual void DataId( Uint32 Id );
	protected:
		cShape *	mShape;
		eeVector2f	mPos;
		eeVector2i	mTilePos;
};

}}

#endif