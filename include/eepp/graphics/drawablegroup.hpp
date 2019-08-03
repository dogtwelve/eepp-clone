#ifndef EE_GRAPHICS_DRAWABLE_GROUP
#define EE_GRAPHICS_DRAWABLE_GROUP

#include <eepp/graphics/drawable.hpp>
#include <vector>

namespace EE { namespace Graphics {

class EE_API DrawableGroup : public Drawable {
	public:
		DrawableGroup();

		virtual ~DrawableGroup();

		virtual Sizef getSize();

		virtual void draw();

		virtual void draw( const Vector2f& position );

		virtual void draw( const Vector2f& position, const Sizef& size );

		void clearDrawables();
		
		Drawable * addDrawable( Drawable * drawable );
		
		Uint32 getDrawableCount() const;

		bool isClipEnabled() const;

		void setClipEnabled(bool clipEnabled);

		bool isDrawableOwner() const;

		void setDrawableOwner(bool drawableOwner);
	protected:
		std::vector<Drawable*> mGroup;
		std::vector<Vector2f> mPos;
		Sizef mSize;
		bool mNeedsUpdate;
		bool mClipEnabled;
		bool mDrawableOwner;
		
		virtual void onPositionChange();

		void update();
};

}}

#endif 
