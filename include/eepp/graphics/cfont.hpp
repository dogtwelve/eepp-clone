#ifndef EE_GRAPHICSCFONT_H
#define EE_GRAPHICSCFONT_H

#include <eepp/graphics/base.hpp>
#include <eepp/graphics/fonthelper.hpp>
#include <eepp/graphics/ctexturefactory.hpp>
#include <eepp/graphics/ctextcache.hpp>

namespace EE { namespace Graphics {

/** @brief Font interface class. */
class EE_API cFont {
	public:
		virtual ~cFont();

		/** Set a text to render
		 * @param Text The Text
		 */
		void SetText( const String& Text );

		/** @return The width of the string rendered */
		eeFloat GetTextWidth();

		/** @return Assign a new text and then returns his width */
		eeFloat GetTextWidth( const String& Text );

		/** @return The current text height */
		eeFloat GetTextHeight();

		/** @return The number of lines of the current text */
		virtual eeInt GetNumLines();

		/** @return The Font Color */
		const eeColorA& Color() const;

		/** Set the color of the string rendered */
		void Color(const eeColorA& Color);

		/** @return The Shadow Font Color */
		const eeColorA& ShadowColor() const;

		/** Set the shadow color of the string rendered */
		void ShadowColor(const eeColorA& Color);

		/** @return The current font size */
		Uint32 GetFontSize() const;

		/** @return The current font height */
		Uint32 GetFontHeight() const;

		/** @return The recommended line spacing */
		Int32 GetLineSkip() const;

		/** @return The font highest ascent (height above base) */
		Int32 GetFontAscent() const;

		/** @return The font lowest descent (height below base) */
		Int32 GetFontDescent() const;

		/** @return The current text */
		String GetText();

		/** @return The last text rendered or setted lines width */
		const std::vector<eeFloat>& GetLinesWidth();

		/** Draw a String on the screen
		* @param Text The text to draw
		* @param X The start x position
		* @param Y The start y position
		* @param Flags Set some flags to the rendering ( for text align )
		* @param Scale The string rendered scale
		* @param Angle The angle of the string rendered
		* @param Effect Set the Blend Mode ( default ALPHA_NORMAL )
		*/
		void Draw( const String& Text, const eeFloat& X, const eeFloat& Y, const Uint32& Flags = FONT_DRAW_LEFT, const eeVector2f& Scale = eeVector2f::One, const eeFloat& Angle = 0, const EE_BLEND_MODE& Effect = ALPHA_NORMAL );

		/** Draw the string seted on the screen
		* @param X The start x position
		* @param Y The start y position
		* @param Flags Set some flags to the rendering ( for text align )
		* @param Scale The string rendered scale
		* @param Angle The angle of the string rendered
		* @param Effect Set the Blend Mode ( default ALPHA_NORMAL )
		*/
		void Draw( const eeFloat& X, const eeFloat& Y, const Uint32& Flags = FONT_DRAW_LEFT, const eeVector2f& Scale = eeVector2f::One, const eeFloat& Angle = 0, const EE_BLEND_MODE& Effect = ALPHA_NORMAL );

		/** Draw a string on the screen from a cached text
		* @param TextCache The cached text
		* @param X The start x position
		* @param Y The start y position
		* @param Flags Set some flags to the rendering ( for text align )
		* @param Scale The string rendered scale
		* @param Angle The angle of the string rendered
		* @param Effect Set the Blend Mode ( default ALPHA_NORMAL )
		*/
		void Draw( cTextCache& TextCache, const eeFloat& X, const eeFloat& Y, const Uint32& Flags = FONT_DRAW_LEFT, const eeVector2f& Scale = eeVector2f::One, const eeFloat& Angle = 0, const EE_BLEND_MODE& Effect = ALPHA_NORMAL );

		/** Shrink the String to a max width
		* @param Str The string to shrink
		* @param MaxWidth The Max Width posible
		*/
		void ShrinkText( String& Str, const Uint32& MaxWidth );

		/** Shrink the string to a max width
		* @param Str The string to shrink
		* @param MaxWidth The Max Width posible
		*/
		void ShrinkText( std::string& Str, const Uint32& MaxWidth );

		/** Cache the with of the current text */
		void CacheWidth( const String& Text, std::vector<eeFloat>& LinesWidth, eeFloat& CachedWidth, eeInt& NumLines, eeInt& LargestLineCharCount );

		/** @return The font texture id */
		const Uint32& GetTexId() const;

		/** @return The type of the instance of the font, can be FONT_TYPE_TTF ( true type font ) or FONT_TYPE_TEX ( texture font ) */
		const Uint32& Type() const;

		/** @return The font name */
		const std::string&	Name() const;

		/** Change the font name ( and id, because it's the font name hash ) */
		void Name( const std::string& name );

		/** @return The font id */
		const Uint32& Id();

		/** Finds the closest cursor position to the point position */
		Int32 FindClosestCursorPosFromPoint( const String & Text, const eeVector2i& pos );

		/** Simulates a selection request and return the initial and end cursor position when the selection worked. Otherwise both parameters will be -1. */
		void SelectSubStringFromCursor( const String& Text, const Int32& CurPos, Int32& InitCur, Int32& EndCur );

		/** @return The cursor position inside the string */
		eeVector2i GetCursorPos( const String& Text, const Uint32& Pos );
	protected:
		Uint32 						mType;
		std::string					mFontName;
		Uint32						mFontHash;
		Uint32 						mTexId;
		Uint32 						mHeight;
		Uint32 						mSize;
		Int32						mLineSkip;
		Int32						mAscent;
		Int32						mDescent;

		std::vector<eeGlyph> 		mGlyphs;
		std::vector<eeTexCoords> 	mTexCoords;

		cTextCache					mTextCache;

		cFont( const Uint32& Type, const std::string& Name );

		void CacheWidth();
};

}}

#endif
