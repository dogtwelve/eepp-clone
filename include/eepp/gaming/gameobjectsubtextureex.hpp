#ifndef CGAMEOBJECTSUBTEXTUREEX_HPP
#define CGAMEOBJECTSUBTEXTUREEX_HPP

#include <eepp/gaming/base.hpp>
#include <eepp/gaming/gameobjectsubtexture.hpp>

namespace EE { namespace Gaming {

class EE_API GameObjectSubTextureEx : public GameObjectSubTexture {
	public:
		GameObjectSubTextureEx( const Uint32& Flags, MapLayer * Layer, Graphics::SubTexture * SubTexture = NULL, const Vector2f& Pos = Vector2f(), EE_BLEND_MODE Blend = ALPHA_NORMAL, EE_RENDER_MODE Render = RN_NORMAL, Float Angle = 0.f, Vector2f Scale = Vector2f::One, ColorA Color = ColorA() );

		virtual ~GameObjectSubTextureEx();

		virtual void Draw();

		virtual Uint32 Type() const;

		virtual bool IsType( const Uint32& type );

		virtual void FlagSet( const Uint32& Flag );
	protected:
		EE_BLEND_MODE		mBlend;
		EE_RENDER_MODE		mRender;
		Float				mAngle;
		Vector2f			mScale;
		ColorA				mColor;
		ColorA *			mVertexColors;
};

}}

#endif
