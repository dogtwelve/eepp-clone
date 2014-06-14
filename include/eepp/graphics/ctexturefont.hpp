#ifndef EECTEXTUREFONT_H
#define EECTEXTUREFONT_H

#include <eepp/graphics/base.hpp>
#include <eepp/graphics/ctexturefactory.hpp>
#include <eepp/graphics/cfont.hpp>

namespace EE { namespace Graphics {

/** @brief This class loads texture fonts and draw strings to the screen. */
class EE_API cTextureFont : public cFont {
	public:
		/** Creates an instance of a texture font */
		static cTextureFont * New( const std::string FontName );

		/** The destructor will not unload the texture from memory. If you want that you'll have to remove it manually ( cTextureFactory::instance()->Remove( MyFontInstance->GetTexId() ) ). */
		virtual ~cTextureFont();

		/** Load's a texture font
		* @param TexId The texture id returned by cTextureFactory
		* @param StartChar The fist char represented on the texture
		* @param Spacing The space between every char ( default 0 means TextureWidth / TexColumns )
		* @param TexColumns The number of chars per column
		* @param TexRows The number of chars per row
		* @param NumChars The number of characters to read from the texture
		* @return True if success
		*/
		bool Load( const Uint32& TexId, const unsigned int& StartChar = 0, const unsigned int& Spacing = 0, const unsigned int& TexColumns = 16, const unsigned int& TexRows = 16, const Uint16& NumChars = 256 );

		/** Load's a texture font and then load's the character coordinates file ( generated by the cTTFFont class )
		* @param TexId The texture id returned by cTextureFactory
		* @param CoordinatesDatPath The character coordinates file
		* @return True if success
		*/
		bool Load( const Uint32& TexId, const std::string& CoordinatesDatPath );

		/**
		* @param TexId The texture id returned by cTextureFactory
		* @param Pack Pointer to the pack instance
		* @param FilePackPath The path of the file inside the pack
		* @return True success
		*/
		bool LoadFromPack( const Uint32& TexId, Pack * Pack, const std::string& FilePackPath );

		/** Load's a texture font and then load's the character coordinates file previously loaded on memory ( generated by the cTTFFont class )
		* @param TexId The texture id returned by cTextureFactory
		* @param CoordData The character coordinates buffer pointer
		* @param CoordDataSize The size of CoordData
		* @return True if success
		*/
		bool LoadFromMemory( const Uint32& TexId, const char* CoordData, const Uint32& CoordDataSize );

		/** Load's a texture font and then load's the character coordinates from a IO stream file ( generated by the cTTFFont class )
		* @param TexId The texture id returned by cTextureFactory
		* @param IOS IO stream file for the coordinates
		* @return True if success
		*/
		bool LoadFromStream( const Uint32& TexId, IOStream& IOS );
	private:
		unsigned int mStartChar;
		unsigned int mTexColumns;
		unsigned int mTexRows;
		unsigned int mSpacing;
		unsigned int mNumChars;

		Float mtX;
		Float mtY;
		Float mFWidth;
		Float mFHeight;

		bool mLoadedCoords;

		cTextureFont( const std::string FontName );

		void BuildFont();

		void BuildFromGlyphs();
};

}}

#endif
