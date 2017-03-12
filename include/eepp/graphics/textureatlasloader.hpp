#ifndef EE_GRAPHICSCTEXTUREATLASLOADER_HPP
#define EE_GRAPHICSCTEXTUREATLASLOADER_HPP

#include <eepp/graphics/base.hpp>
#include <eepp/graphics/packerhelper.hpp>
#include <eepp/graphics/textureloader.hpp>
#include <eepp/graphics/texturefactory.hpp>
#include <eepp/system/resourceloader.hpp>
#include <eepp/system/iostream.hpp>

namespace EE { namespace Graphics {

using namespace Private;

class TextureAtlas;

/** @brief The Texture Atlas Loader loads any previously created Texture Atlas. */
class EE_API TextureAtlasLoader {
	public:
		typedef cb::Callback1<void, TextureAtlasLoader *> GLLoadCallback;

		/** Creates an empty loader. The texture atlas can be loaded callin any Load* function. */
		TextureAtlasLoader();

		/** Loads a texture atlas from its path ( the texture atlas binary is expected, not the texture, the ".eta" file ).
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param TextureAtlasPath The texture atlas path.
		*	@param Threaded Indicates if the loading is done in another thread.
		*	@param LoadCallback The load notification callback.
		*/
		TextureAtlasLoader( const std::string& TextureAtlasPath, const bool& threaded = false, GLLoadCallback LoadCallback = GLLoadCallback() );

		/** Loads a texture atlas from memory.
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param Data The texture atlas buffer pointer
		*	@param DataSize The texture atlas buffer pointer size
		*	@param TextureAtlasName Since the texture atlas is loaded from memory, the name can't be obtained from the file name. So it needs to be indicated manually.
		*	@param Threaded Indicates if the loading is done in another thread.
		*	@param LoadCallback The load notification callback.
		*/
		TextureAtlasLoader( const Uint8* Data, const Uint32& DataSize, const std::string& TextureAtlasName, const bool& threaded = false, GLLoadCallback LoadCallback = GLLoadCallback() );

		/** Loads a texture atlas from a pack file.
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param Pack The pointer of the pack instance to be used to load the file.
		*	@param FilePackPath The path of the file inside the pack.
		*	@param Threaded Indicates if the loading is done in another thread.
		*	@param LoadCallback The load notification callback.
		*/
		TextureAtlasLoader( Pack * Pack, const std::string& FilePackPath, const bool& threaded = false, GLLoadCallback LoadCallback = GLLoadCallback() );

		/** Loads a texture atlas from a io stream.
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param IOS The io stream to use for the loading.
		*	@param Threaded Indicates if the loading is done in another thread.
		*	@param LoadCallback The load notification callback.
		*/
		TextureAtlasLoader( IOStream& IOS, const bool& threaded = false, GLLoadCallback LoadCallback = GLLoadCallback() );

		~TextureAtlasLoader();

		/** In the case that the loader is threaded, to know if the texture atlas was loaded, the main thread must call Update to update the state of the load. And finish the job. */
		void					update();

		/** Loads a texture atlas from its path ( the texture atlas binary is expected, not the texture, the ".eta" file ).
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param TextureAtlasPath The texture atlas path.
		*/
		void					load( const std::string& TextureAtlasPath = "" );

		/** Loads a texture atlas from a io stream.
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param IOS The io stream to use for the loading.
		*/
		void					loadFromStream( IOStream& IOS );

		/** Loads a texture atlas from memory.
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param Data The texture atlas buffer pointer
		*	@param DataSize The texture atlas buffer pointer size
		*	@param TextureAtlasName Since the texture atlas is loaded from memory, the name can't be obtained from the file name. So it needs to be indicated manually.
		*/
		void					loadFromMemory( const Uint8* Data, const Uint32& DataSize, const std::string& TextureAtlasName );

		/** Loads a texture atlas from a pack file.
		*	If the loader is not threaded, it will load the atlas immediately.
		*	@param Pack The pointer of the pack instance to be used to load the file.
		*	@param FilePackPath The path of the file inside the pack.
		*/
		void					loadFromPack( Pack * Pack, const std::string& FilePackPath );

		/** @return If the loader is threaded ( asynchronous ). */
		bool					isThreaded() const;

		/** If threaded is true sets the loader as asynchronous. This must be called before the loading is done. */
		void					setThreaded( const bool& threaded );

		/** @return True if the texture atlas is loaded. */
		const bool&				isLoaded() const;
		
		/** @return True if the texture atlas is loading. */
		const bool&				isLoading() const;

		/** @brief The function will check if the texture atlas is updated.
			Checks if all the images inside the images path are inside the texture atlas, and if they have the same date and size, otherwise it will recreate or update the texture atlas.
		*/
		bool					updateTextureAtlas( std::string TextureAtlasPath, std::string ImagesPath );

		/** Rewrites the texture atlas file. Usefull if the SubTextures where modified and need to be updated inside the texture atlas. */
		bool					updateTextureAtlas();

		/** @return The texture that corresponds to the texture atlas.
		* @param texnum The texture index. A texture atlas can use more than one texture, so it can be 0 to GetTexturesLoadedCount(). Usually a texture atlas corresponds to only one texture, so the texture index is 0.
		*/
		Texture *				getTexture( const Uint32& texnum = 0 ) const;

		/** @return The number of textures linked to the texture atlas. */
		Uint32					getTexturesLoadedCount();

		/** @return The texture atlas instance pointer ( NULL if the atlas isn't loaded yet ). */
		TextureAtlas *			getTextureAtlas() const;

		/** Sets a load notification callback. */
		void					setLoadCallback( GLLoadCallback LoadCallback );
	protected:
		ResourceLoader			mRL;
		std::string				mTextureAtlasPath;
		bool					mThreaded;
		bool					mLoaded;
		Pack *					mPack;
		bool					mSkipResourceLoad;
		bool					mIsLoading;
		TextureAtlas *			mTextureAtlas;
		GLLoadCallback			mLoadCallback;
		std::vector<Texture*>	mTexuresLoaded;

		typedef struct sTempTexAtlasS {
			sTextureHdr 			Texture;
			std::vector<sSubTextureHdr>	SubTextures;
		} sTempTexAtlas;

		sTextureAtlasHdr mTexGrHdr;
		std::vector<sTempTexAtlas> mTempAtlass;

		void createSubTextures();
};

}}

#endif
