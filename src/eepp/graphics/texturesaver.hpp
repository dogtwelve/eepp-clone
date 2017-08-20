#ifndef EE_GRAPHICSCTEXTURESAVER_HPP
#define EE_GRAPHICSCTEXTURESAVER_HPP

#include <eepp/config.hpp>

namespace EE { namespace Graphics { namespace Private {

class TextureSaver {
	public:
		TextureSaver( int textureBind = -1 /** with -1 as default it avoid the auto-binding */ );
		
		~TextureSaver();
	private:
		int mTextureBinded;
		int mTextureToBind;
};

}}}

#endif
