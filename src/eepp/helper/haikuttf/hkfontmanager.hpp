#ifndef HAIKUTTF_HKFONTMANAGER_HPP
#define HAIKUTTF_HKFONTMANAGER_HPP

#include "hkbase.hpp"
#include "hkglyph.hpp"
#include "hkfont.hpp"
#include "hkmutex.hpp"

namespace HaikuTTF {

class hkFontManager : private hkMutex {
	static hkFontManager * mSingleton;
	public:
		static hkFontManager * exists();

		static hkFontManager * instance();

		static void DestroySingleton();

		hkFontManager();

		~hkFontManager();

		int 				Init();

		void 				Destroy();

		int 				WasInit();

		void 				CloseFont( hkFont * Font );

		hkFont * 			OpenFromMemory( const u8* data, unsigned long size, int ptsize, long index = 0, unsigned int glyphCacheSize = 256 );

		hkFont * 			OpenFromFile( const char* filename, int ptsize, long index = 0, unsigned int glyphCacheSize = 256 );

		FT_Library 			Library() const;
	protected:
		friend class hkFont;
		
		FT_Library 			mLibrary;
		int 				mInitialized;

		hkFont * 			FontPrepare( hkFont * font, int ptsize );
		
		void MutexLock();
		
		void MutexUnlock();
};

}

#endif
