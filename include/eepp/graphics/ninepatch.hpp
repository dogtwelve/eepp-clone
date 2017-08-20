#ifndef EE_GRAPHICS_NINEPATCH_HPP 
#define EE_GRAPHICS_NINEPATCH_HPP

#include <eepp/core.hpp>
#include <eepp/graphics/drawable.hpp>
#include <eepp/graphics/subtexture.hpp>

namespace EE { namespace Graphics {

class EE_API NinePatch : public DrawableResource {
	public:
		enum NinePatchSides {
			Left = 0,
			Right,
			Down,
			Up,
			UpLeft,
			UpRight,
			DownLeft,
			DownRight,
			Center,
			SideCount
		};

		NinePatch( const Uint32& TexId, int left, int top, int right, int bottom, const std::string& name = "" );
		
		NinePatch( SubTexture * subTexture, int left, int top, int right, int bottom, const std::string& name = "" );

		~NinePatch();
		
		virtual Sizef getSize();

		virtual void draw();

		virtual void draw( const Vector2f& position );

		virtual void draw( const Vector2f& position, const Sizef& size );

		const Sizef& getDestSize() const;

		void setDestSize( const Sizef& destSize );

		const Vector2i& getOffset() const;

		void setOffset( const Vector2i& offset );
	protected:
		Drawable * 	mDrawable[ SideCount ];
		Rect mRect;
		Sizei mSize;
		Sizef mDestSize;
		Vector2i mOffset;

		void createFromTexture( const Uint32& TexId, int left, int top, int right, int bottom );
};

}}

#endif
