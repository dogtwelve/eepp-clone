#ifndef EE_WINDOWCCURSOR_HPP
#define EE_WINDOWCCURSOR_HPP

#include <eepp/window/base.hpp>

#include <eepp/graphics/cimage.hpp>
#include <eepp/graphics/ctexture.hpp>
using namespace EE::Graphics;

#include <eepp/window/cwindow.hpp>

namespace EE { namespace Window {

class EE_API cCursor {
	public:
		const Uint32& Id() const;

		const std::string& Name() const;

		const eeVector2i& HotSpot() const;

		cImage * Image() const;
		
		virtual ~cCursor();
	protected:
		friend class cCursorManager;

		Uint32			mId;
		std::string		mName;
		cImage *		mImage;
		eeVector2i		mHotSpot;
		cWindow *		mWindow;

		cCursor( cTexture * tex, const eeVector2i& hotspot, const std::string& name, Window::cWindow * window );

		cCursor( cImage * img, const eeVector2i& hotspot, const std::string& name, Window::cWindow * window );

		cCursor( const std::string& path, const eeVector2i& hotspot, const std::string& name, Window::cWindow * window );

		virtual void Create() = 0;
};

}}

#endif