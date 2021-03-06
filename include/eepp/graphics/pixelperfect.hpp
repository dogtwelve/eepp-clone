#ifndef EE_GRAPHICS_PIXELPERFECT_H
#define EE_GRAPHICS_PIXELPERFECT_H

#include <eepp/graphics/texture.hpp>

namespace EE { namespace Graphics {

/** Pixel Perfect Collition implementation.
* @param Tex1 First Texture Id
* @param x1 Screen X axis position for the first texture
* @param y1 Screen Y axis position for the first texture
* @param Tex2 Second Texture Id
* @param x2 Screen X axis position for the second texture
* @param y2 Screen Y axis position for the second texture
* @param Tex1_SrcRECT The sector of the texture from TexId_1 that you are rendering, the sector you want to collide ( on Sprite the SprSrcRECT )
* @param Tex2_SrcRECT  The sector of the texture from TexId_2 that you are rendering, the sector you want to collide ( on Sprite the SprSrcRECT )
* @warning Stress the CPU easily. \n Creates a copy of the texture on the app contex. \n It will not work with scaled or rotated textures.
* @return True if collided
*/
bool EE_API PixelPerfectCollide( Texture * Tex1, const unsigned int& x1, const unsigned int& y1, Texture * Tex2, const unsigned int& x2, const unsigned int& y2, const Rectu& Tex1_SrcRECT = Rectu(0,0,0,0), const Rectu& Tex2_SrcRECT = Rectu(0,0,0,0) );

/** Pixel Perfect Collition implementation between texture and a point
* @param Tex First Texture Id
* @param x1 Screen X axis position for the first texture
* @param y1 Screen Y axis position for the first texture
* @param x2 Screen X axis position for the point on screen
* @param y2 Screen Y axis position for the point on screen
* @param Tex1_SrcRECT The sector of the texture from TexId_1 that you are rendering, the sector you want to collide ( on Sprite the SprSrcRECT )
* @return True if collided
*/
bool EE_API PixelPerfectCollide( Texture * Tex, const unsigned int& x1, const unsigned int& y1, const unsigned int& x2, const unsigned int& y2, const Rectu& Tex1_SrcRECT = Rectu(0,0,0,0) );

}}

#endif
