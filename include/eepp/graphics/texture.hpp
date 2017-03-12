#ifndef EE_GRAPHICSCTEXTURE_H
#define EE_GRAPHICSCTEXTURE_H

#include <eepp/core.hpp>
#include <eepp/graphics/image.hpp>
#include <eepp/core/noncopyable.hpp>
#include <eepp/math/polygon2.hpp>
#include <eepp/math/originpoint.hpp>

namespace EE { namespace Graphics {

class EE_API Texture : public Image, private NonCopyable {
	public:
		static Uint32 getMaximumSize();

		/** Set the OpenGL Texture Id (texture handle) */
		void setHandle( const int& texture ) { mTexture = texture; }

		/** @return The OpenGL Texture Id (texture handle) */
		int getHandle() const { return mTexture; }

		/** @return The hash of the filename */
		const Uint32& getHashName() const;

		/** Set the Texture File Path */
		void setFilepath( const std::string& filepath ) { mFilepath = filepath; }

		/** @return The Texture File Path */
		std::string getFilepath() const { return mFilepath; }

		/** @return The Image Width */
		unsigned int getImageWidth() const { return mImgWidth; }

		/** @return The Image Height */
		unsigned int getImageHeight() const { return mImgHeight; }

		/** Set if the Texture use Mipmaps */
		void setMipmap( const bool& UseMipmap );

		/** @return If the texture use Mipmaps */
		bool getMipmap() const;

		/** Set the Texture Clamp Mode */
		void setClampMode( const EE_CLAMP_MODE& clampmode );

		/** @return The Texture Clamp Mode */
		EE_CLAMP_MODE getClampMode() const { return mClampMode; }

		/** Lock the Texture for direct access.
		**	It is needed to have any read/write access to the texture. This feature is not supported in OpenGL ES. */
		Uint8 * lock( const bool& ForceRGBA = false );

		/** @brief Unlock the previously locked Texture.
		*	Unlocking the texture will upload the local copy of the texture ( that could have been modified ) to the GPU.
		*	@param KeepData If true keeps the local copy of the texture un memory, otherwise it will be released.
		*	@param Modified The flag indicates of the texture was modified between the Lock and Unlock calls. This is to force reloading the texture from memory to VRAM. In the case that the texture in ram was modified using the Texture methods to do this, it will already know that this is true, so it will upload the changes to the GPU.
		*/
		bool unlock(const bool& KeepData = false, const bool& Modified = false);

		/** @return A pointer to the first pixel of the texture ( keeped with a local copy ). \n You must have a copy of the texture on local memory. For that you need to Lock the texture first. */
		const Uint8* getPixelsPtr();

		/** Set the Texture Filter Mode */
		void setFilter( const EE_TEX_FILTER& filter );

		/** @return The texture filter used by the texture */
		const EE_TEX_FILTER& getFilter() const;

		/** Save the Texture to a new File */
		bool saveToFile( const std::string& filepath, const EE_SAVE_TYPE& Format );

		/** Replace a color on the texture */
		void replaceColor( const ColorA& ColorKey, const ColorA& NewColor);

		/** Create an Alpha mask from a Color */
		void createMaskFromColor( const ColorA& ColorKey, Uint8 Alpha );

		/** Fill a texture with a color */
		void fillWithColor( const ColorA& Color );

		/** Resize the texture */
		void resize( const Uint32& newWidth, const Uint32& newHeight, EE_RESAMPLER_FILTER filter = RESAMPLER_LANCZOS4 );

		/** Scale the texture */
		void scale( const Float& scale, EE_RESAMPLER_FILTER filter = RESAMPLER_LANCZOS4 );

		/** Copy an image inside the texture */
		void copyImage( Image * image, const Uint32& x, const Uint32& y );

		/** @brief Update a part of the texture from an array of pixels
		**	The size of the @a pixel array must match the @a width and
		**	@a height arguments.
		**	No additional check is performed on the size of the pixel
		**	array or the bounds of the area to update, passing invalid
		**	arguments will lead to an undefined behaviour.
		**	This function does nothing if @a pixels is null or if the
		**	texture was not previously created.
		**	@param pixels Array of pixels to copy to the texture
		**	@param width  Width of the pixel region contained in @a pixels
		**	@param height Height of the pixel region contained in @a pixels
		**	@param x X offset in the texture where to copy the source pixels
		**	@param y Y offset in the texture where to copy the source pixels
		**	@param pf The pixel format of the @a pixel */
		void update( const Uint8* pixels, Uint32 width, Uint32 height, Uint32 x = 0, Uint32 y = 0, EE_PIXEL_FORMAT pf = PF_RGBA );

		/** @brief Update the whole texture from an array of pixels
		**	The @a pixel array is assumed to have the same size as
		**	the @a area rectangle.
		**	No additional check is performed on the size of the pixel
		**	array, passing invalid arguments will lead to an undefined
		**	behaviour.
		**	This function does nothing if @a pixels is null or if the
		**	texture was not previously created.
		**	@param pixels Array of pixels to copy to the texture */
		void update( const Uint8* pixels );

		/** @brief Update a part of the texture from an image
		**	The pixel format is automatically detected
		**	No additional check is performed on the size of the image,
		**	passing an invalid combination of image size and offset
		**	will lead to an undefined behaviour.
		**	This function does nothing if the texture was not
		**	previously created.
		**	@param image Image to copy to the texture
		**	@param x X offset in the texture where to copy the source image
		**	@param y Y offset in the texture where to copy the source image */
		void update( Image * image, Uint32 x = 0, Uint32 y = 0 );

		/** Replaces the current texture with the image provided, reusing the current texture id. */
		void replace( Image * image );

		/** Flip the texture ( rotate the texture 90º ). Warning: This is flipped in memory, a real flipping. */
		void flip();

		/** @return If the Texture has a copy on the local memory */
		bool hasLocalCopy();

		/** Unload the Texture from Memory */
		void deleteTexture();

		/** Set if the Texture is Grabed */
		void setGrabed( const bool& isGrabed );

		/** @return If the texture is Grabed */
		bool isGrabed() const;

		/** @return If the texture was compressed on load (DXT compression) */
		bool isCompressed() const;

		/** Render the texture on screen ( with less internal mess, a little bit faster way )
		* @param x The x position on screen
		* @param y The y position on screen
		* @param Angle The Angle of the texture rendered
		* @param Scale The Scale factor of the rendered texture
		* @param Color The texture color
		* @param Blend Set the Blend Mode ( default ALPHA_NORMAL )
		* @param width The width of the texture rendered
		* @param height The height of the texture rendered
		*/
		void drawFast( const Float& x, const Float& y, const Float& Angle = 0.0f, const Vector2f& scale = Vector2f::One, const ColorA& Color = ColorA(), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const Float &width = 0, const Float &height = 0 );

		/** Render the texture on screen
		* @param x The x position on screen
		* @param y The y position on screen
		* @param Angle The Angle of the texture rendered
		* @param Scale The Scale factor of the rendered texture
		* @param Color The texture color
		* @param Blend Set the Blend Mode ( default ALPHA_NORMAL )
		* @param Effect Set the Render Effect ( default RN_NORMAL, no effect )
		* @param Center The rotation and scaling center. The center point is relative to the top-left corner of the object.
		* @param texSector The texture sector to render. You can render only a part of the texture. ( default render all the texture )
		*/
		void draw( const Float &x, const Float &y, const Float &Angle = 0, const Vector2f &scale = Vector2f::One, const ColorA& Color = ColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const EE_RENDER_MODE &Effect = RN_NORMAL, OriginPoint Center = OriginPoint(OriginPoint::OriginCenter), const Recti& texSector = Recti(0,0,0,0) );

		/** Render the texture on screen. Extended because can set the vertex colors individually
		* @param x The x position on screen
		* @param y The y position on screen
		* @param width The width of the texture rendered ( when Scale = 1, otherwise this width will be scaled like width * Scale )
		* @param height The height of the texture rendered ( when Scale = 1, otherwise this height will be scaled like height * Scale )
		* @param Angle The Angle of the texture rendered
		* @param Scale The Scale factor of the rendered texture
		* @param Color0 The Left - Top vertex color
		* @param Color1 The Left - Bottom vertex color
		* @param Color2 The Right - Bottom vertex color
		* @param Color3 The Right - Top vertex color
		* @param Blend Set the Blend Mode ( default ALPHA_NORMAL )
		* @param Effect Set the Render Effect ( default RN_NORMAL, no effect )
		* @param Center The rotation and scaling center. The center point is relative to the top-left corner of the object.
		* @param texSector The texture sector to render. You can render only a part of the texture. ( default render all the texture )
		*/
		void drawEx( Float x, Float y, Float width = 0.0f, Float height = 0.0f, const Float &Angle = 0, const Vector2f &scale = Vector2f::One, const ColorA& Color0 = ColorA(255,255,255,255), const ColorA& Color1 = ColorA(255,255,255,255), const ColorA& Color2 = ColorA(255,255,255,255), const ColorA& Color3 = ColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const EE_RENDER_MODE &Effect = RN_NORMAL, OriginPoint Center = OriginPoint(OriginPoint::OriginCenter), const Recti& texSector = Recti(0,0,0,0) );

		/** Render a quad on Screen
		* @param Q The Quad2f
		* @param Offset The Offset applied to all the coordinates on Quad2f
		* @param Angle The Angle of the Quad2f rendered
		* @param Scale The Scale of the Quad2f rendered
		* @param Color The Quad2f color
		* @param Blend Set the Blend Mode ( default ALPHA_NORMAL )
		* @param texSector The texture sector to render. You can render only a part of the texture. ( default render all the texture )
		*/
		void drawQuad( const Quad2f& Q, const Vector2f& Offset = Vector2f(), const Float &Angle = 0.0f, const Vector2f &scale = Vector2f::One, const ColorA& Color = ColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const Recti& texSector = Recti(0,0,0,0) );

		/** Render a quad on Screen
		* @param Q The Quad2f
		* @param Offset The Offset applied to all the coordinates on Quad2f
		* @param Angle The Angle of the Quad2f rendered
		* @param Scale The Scale of the Quad2f rendered
		* @param Color0 The Left - Top vertex color
		* @param Color1 The Left - Bottom vertex color
		* @param Color2 The Right - Bottom vertex color
		* @param Color3 The Right - Top vertex color
		* @param Blend Set the Blend Mode ( default ALPHA_NORMAL )
		* @param texSector The texture sector to render. You can render only a part of the texture. ( default render all the texture )
		*/
		void drawQuadEx( Quad2f Q, const Vector2f& Offset = Vector2f(), const Float &Angle = 0.0f, const Vector2f &scale = Vector2f::One, const ColorA& Color0 = ColorA(255,255,255,255), const ColorA& Color1 = ColorA(255,255,255,255), const ColorA& Color2 = ColorA(255,255,255,255), const ColorA& Color3 = ColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, Recti texSector = Recti(0,0,0,0) );

		/** Set the texture factory internal id of the texture */
		void setId( const Uint32& id );

		/** @return The texture factory internal id of the texture */
		const Uint32& getId() const;

		/** Reload the texture from the current local copy. */
		void reload();

		/** Set a pixel to the locked texture. */
		void setPixel( const unsigned int& x, const unsigned int& y, const ColorA& Color );

		/** Bind the texture. Activate the texture for rendering. */
		void bind();

		virtual ~Texture();
	protected:
		enum TEXTURE_FLAGS
		{
			TEX_FLAG_MIPMAP		=	( 1 << 0 ),
			TEX_FLAG_MODIFIED	=	( 1 << 1 ),
			TEX_FLAG_COMPRESSED	=	( 1 << 2 ),
			TEX_FLAG_LOCKED		= 	( 1 << 3 ),
			TEX_FLAG_GRABED		=	( 1 << 4 )
		};

		friend class TextureFactory;

		Texture();

		Texture( const Uint32& texture, const unsigned int& width, const unsigned int& height, const unsigned int& imgwidth, const unsigned int& imgheight, const bool& UseMipmap, const unsigned int& channels, const std::string& filepath, const EE_CLAMP_MODE& clampMode, const bool& CompressedTexture, const Uint32& memSize = 0, const Uint8* data = NULL );

		Texture( const Texture& copy );

		void create( const Uint32& texture, const unsigned int& width, const unsigned int& height, const unsigned int& imgwidth, const unsigned int& imgheight, const bool& UseMipmap, const unsigned int& channels, const std::string& filepath, const EE_CLAMP_MODE& clampMode, const bool& CompressedTexture, const Uint32& memSize = 0, const Uint8* data = NULL );

		std::string 	mFilepath;

		Uint32 			mId;
		Uint32 			mTexId;
		int 			mTexture;

		unsigned int 	mImgWidth;
		unsigned int 	mImgHeight;

		Uint32			mFlags;

		EE_CLAMP_MODE 	mClampMode;
		EE_TEX_FILTER 	mFilter;

		int				mInternalFormat;

		void 			applyClampMode();

		Uint8 * 		iLock( const bool& ForceRGBA, const bool& KeepFormat );

		void			iTextureFilter( const EE_TEX_FILTER& filter );
};

}}

#endif
