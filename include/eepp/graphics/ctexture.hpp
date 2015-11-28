#ifndef EE_GRAPHICSCTEXTURE_H
#define EE_GRAPHICSCTEXTURE_H

#include <eepp/graphics/base.hpp>
#include <eepp/graphics/cimage.hpp>
#include <eepp/graphics/ctexturefactory.hpp>
#include <eepp/math/polygon2.hpp>
#include <eepp/math/originpoint.hpp>

namespace EE { namespace Graphics {

class EE_API cTexture : public cImage, private NonCopyable {
	public:
		/** Set the OpenGL Texture Id (texture handle) */
		void Handle( const int& texture ) { mTexture = texture; }

		/** @return The OpenGL Texture Id (texture handle) */
		int Handle() const { return mTexture; }

		/** @return The hash of the filename */
		const Uint32& HashName() const;

		/** Set the Texture File Path */
		void Filepath( const std::string& filepath ) { mFilepath = filepath; }

		/** @return The Texture File Path */
		std::string Filepath() const { return mFilepath; }

		/** @return The Image Width */
		eeUint ImgWidth() const { return mImgWidth; }

		/** @return The Image Height */
		eeUint ImgHeight() const { return mImgHeight; }

		/** Set if the Texture use Mipmaps */
		void Mipmap( const bool& UseMipmap );

		/** @return If the texture use Mipmaps */
		bool Mipmap() const;

		/** Set the Texture Clamp Mode */
		void ClampMode( const EE_CLAMP_MODE& clampmode );

		/** @return The Texture Clamp Mode */
		EE_CLAMP_MODE ClampMode() const { return mClampMode; }

		/** Lock the Texture for direct access.
		**	It is needed to have any read/write access to the texture. This feature is not supported in OpenGL ES. */
		Uint8 * Lock( const bool& ForceRGBA = false );

		/** @brief Unlock the previously locked Texture.
		*	Unlocking the texture will upload the local copy of the texture ( that could have been modified ) to the GPU.
		*	@param KeepData If true keeps the local copy of the texture un memory, otherwise it will be released.
		*	@param Modified The flag indicates of the texture was modified between the Lock and Unlock calls. This is to force reloading the texture from memory to VRAM. In the case that the texture in ram was modified using the cTexture methods to do this, it will already know that this is true, so it will upload the changes to the GPU.
		*/
		bool Unlock(const bool& KeepData = false, const bool& Modified = false);

		/** @return A pointer to the first pixel of the texture ( keeped with a local copy ). \n You must have a copy of the texture on local memory. For that you need to Lock the texture first. */
		const Uint8* GetPixelsPtr();

		/** Set the Texture Filter Mode */
		void Filter( const EE_TEX_FILTER& filter );

		/** @return The texture filter used by the texture */
		const EE_TEX_FILTER& Filter() const;

		/** Save the Texture to a new File */
		bool SaveToFile( const std::string& filepath, const EE_SAVE_TYPE& Format );

		/** Replace a color on the texture */
		void ReplaceColor( const eeColorA& ColorKey, const eeColorA& NewColor);

		/** Create an Alpha mask from a Color */
		void CreateMaskFromColor( const eeColorA& ColorKey, Uint8 Alpha );

		/** Fill a texture with a color */
		void FillWithColor( const eeColorA& Color );

		/** Resize the texture */
		void Resize( const Uint32& newWidth, const Uint32& newHeight, EE_RESAMPLER_FILTER filter = RESAMPLER_LANCZOS4 );

		/** Scale the texture */
		void Scale( const eeFloat& scale, EE_RESAMPLER_FILTER filter = RESAMPLER_LANCZOS4 );

		/** Copy an image inside the texture */
		void CopyImage( cImage * image, const Uint32& x, const Uint32& y );

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
		void Update( const Uint8* pixels, Uint32 width, Uint32 height, Uint32 x = 0, Uint32 y = 0, EE_PIXEL_FORMAT pf = PF_RGBA );

		/** @brief Update the whole texture from an array of pixels
		**	The @a pixel array is assumed to have the same size as
		**	the @a area rectangle.
		**	No additional check is performed on the size of the pixel
		**	array, passing invalid arguments will lead to an undefined
		**	behaviour.
		**	This function does nothing if @a pixels is null or if the
		**	texture was not previously created.
		**	@param pixels Array of pixels to copy to the texture */
		void Update( const Uint8* pixels );

		/** @brief Update a part of the texture from an image
		**	The pixel format is automatically detected
		**	No additional check is performed on the size of the image,
		**	passing an invalid combination of image size and offset
		**	will lead to an undefined behaviour.
		**	This function does nothing if the texture was not
		**	previously created.
		**	@param image Image to copy to the texture
		**	@param x     X offset in the texture where to copy the source image
		**	@param y     Y offset in the texture where to copy the source image */
		void Update( cImage * image, Uint32 x = 0, Uint32 y = 0 );

		/** Flip the texture ( rotate the texture 90º ). Warning: This is flipped in memory, a real flipping. */
		void Flip();

		/** @return If the Texture has a copy on the local memory */
		bool LocalCopy();

		/** Unload the Texture from Memory */
		void DeleteTexture();

		/** Set if the Texture is Grabed */
		void Grabed( const bool& isGrabed );

		/** @return If the texture is Grabed */
		bool Grabed() const;

		/** @return If the texture was compressed on load (DXT compression) */
		bool IsCompressed() const;

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
		void DrawFast( const eeFloat& x, const eeFloat& y, const eeFloat& Angle = 0.0f, const eeVector2f& Scale = eeVector2f::One, const eeColorA& Color = eeColorA(), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const eeFloat &width = 0, const eeFloat &height = 0 );

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
		void Draw( const eeFloat &x, const eeFloat &y, const eeFloat &Angle = 0, const eeVector2f &Scale = eeVector2f::One, const eeColorA& Color = eeColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const EE_RENDER_MODE &Effect = RN_NORMAL, eeOriginPoint Center = eeOriginPoint(eeOriginPoint::OriginCenter), const eeRecti& texSector = eeRecti(0,0,0,0) );

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
		void DrawEx( eeFloat x, eeFloat y, eeFloat width = 0.0f, eeFloat height = 0.0f, const eeFloat &Angle = 0, const eeVector2f &Scale = eeVector2f::One, const eeColorA& Color0 = eeColorA(255,255,255,255), const eeColorA& Color1 = eeColorA(255,255,255,255), const eeColorA& Color2 = eeColorA(255,255,255,255), const eeColorA& Color3 = eeColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const EE_RENDER_MODE &Effect = RN_NORMAL, eeOriginPoint Center = eeOriginPoint(eeOriginPoint::OriginCenter), const eeRecti& texSector = eeRecti(0,0,0,0) );

		/** Render a quad on Screen
		* @param Q The eeQuad2f
		* @param Offset The Offset applied to all the coordinates on eeQuad2f
		* @param Angle The Angle of the eeQuad2f rendered
		* @param Scale The Scale of the eeQuad2f rendered
		* @param Color The eeQuad2f color
		* @param Blend Set the Blend Mode ( default ALPHA_NORMAL )
		* @param texSector The texture sector to render. You can render only a part of the texture. ( default render all the texture )
		*/
		void DrawQuad( const eeQuad2f& Q, const eeVector2f& Offset = eeVector2f(), const eeFloat &Angle = 0.0f, const eeVector2f &Scale = eeVector2f::One, const eeColorA& Color = eeColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, const eeRecti& texSector = eeRecti(0,0,0,0) );

		/** Render a quad on Screen
		* @param Q The eeQuad2f
		* @param Offset The Offset applied to all the coordinates on eeQuad2f
		* @param Angle The Angle of the eeQuad2f rendered
		* @param Scale The Scale of the eeQuad2f rendered
		* @param Color0 The Left - Top vertex color
		* @param Color1 The Left - Bottom vertex color
		* @param Color2 The Right - Bottom vertex color
		* @param Color3 The Right - Top vertex color
		* @param Blend Set the Blend Mode ( default ALPHA_NORMAL )
		* @param texSector The texture sector to render. You can render only a part of the texture. ( default render all the texture )
		*/
		void DrawQuadEx( eeQuad2f Q, const eeVector2f& Offset = eeVector2f(), const eeFloat &Angle = 0.0f, const eeVector2f &Scale = eeVector2f::One, const eeColorA& Color0 = eeColorA(255,255,255,255), const eeColorA& Color1 = eeColorA(255,255,255,255), const eeColorA& Color2 = eeColorA(255,255,255,255), const eeColorA& Color3 = eeColorA(255,255,255,255), const EE_BLEND_MODE &Blend = ALPHA_NORMAL, eeRecti texSector = eeRecti(0,0,0,0) );

		/** Set the texture factory internal id of the texture */
		void Id( const Uint32& id );

		/** @return The texture factory internal id of the texture */
		const Uint32& Id() const;

		/** Reload the texture from the current local copy. */
		void Reload();

		/** Set a pixel to the locked texture. */
		void SetPixel( const eeUint& x, const eeUint& y, const eeColorA& Color );

		/** Bind the texture. Activate the texture for rendering. */
		void Bind();

		virtual ~cTexture();
	protected:
		enum TEXTURE_FLAGS
		{
			TEX_FLAG_MIPMAP		=	( 1 << 0 ),
			TEX_FLAG_MODIFIED	=	( 1 << 1 ),
			TEX_FLAG_COMPRESSED	=	( 1 << 2 ),
			TEX_FLAG_LOCKED		= 	( 1 << 3 ),
			TEX_FLAG_GRABED		=	( 1 << 4 )
		};

		friend class cTextureFactory;

		cTexture();

		cTexture( const Uint32& texture, const eeUint& width, const eeUint& height, const eeUint& imgwidth, const eeUint& imgheight, const bool& UseMipmap, const eeUint& Channels, const std::string& filepath, const EE_CLAMP_MODE& ClampMode, const bool& CompressedTexture, const Uint32& MemSize = 0, const Uint8* data = NULL );

		cTexture( const cTexture& Copy );

		void Create( const Uint32& texture, const eeUint& width, const eeUint& height, const eeUint& imgwidth, const eeUint& imgheight, const bool& UseMipmap, const eeUint& Channels, const std::string& filepath, const EE_CLAMP_MODE& ClampMode, const bool& CompressedTexture, const Uint32& MemSize = 0, const Uint8* data = NULL );

		std::string 	mFilepath;

		Uint32 			mId;
		Uint32 			mTexId;
		int 			mTexture;

		eeUint 			mImgWidth;
		eeUint 			mImgHeight;

		Uint32			mFlags;

		EE_CLAMP_MODE 	mClampMode;
		EE_TEX_FILTER 	mFilter;

		int				mInternalFormat;

		void 			ApplyClampMode();

		Uint8 * 		iLock( const bool& ForceRGBA, const bool& KeepFormat );

		void			iTextureFilter( const EE_TEX_FILTER& filter );
};

}}

#endif
