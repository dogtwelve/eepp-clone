#ifndef EE_GRAPHICSFONTHELPER_HPP
#define EE_GRAPHICSFONTHELPER_HPP

namespace EE { namespace Graphics {

enum EE_FONT_TYPE {
	FONT_TYPE_TTF = 1,
	FONT_TYPE_TEX = 2
};

enum EE_FONT_HALIGN {
	FONT_DRAW_LEFT			= (0 << 0),
	FONT_DRAW_RIGHT			= (1 << 0),
	FONT_DRAW_CENTER		= (2 << 0),
	FONT_DRAW_HALIGN_MASK	= (3 << 0)
};

enum EE_FONT_VALIGN {
	FONT_DRAW_TOP			= (0 << 2),
	FONT_DRAW_BOTTOM		= (1 << 2),
	FONT_DRAW_MIDDLE		= (2 << 2),
	FONT_DRAW_VALIGN_MASK	= (3 << 2)
};

inline Uint32 fontHAlignGet( Uint32 Flags ) {
	return Flags & FONT_DRAW_HALIGN_MASK;
}

inline Uint32 fontVAlignGet( Uint32 Flags ) {
	return Flags & FONT_DRAW_VALIGN_MASK;
}

#define FONT_DRAW_ALIGN_MASK	( FONT_DRAW_VALIGN_MASK | FONT_DRAW_HALIGN_MASK )

struct VertexCoords {
	Float TexCoords[2];
	Float Vertex[2];
};

#define EE_TTF_FONT_MAGIC ( ( 'E' << 0 ) | ( 'E' << 8 ) | ( 'F' << 16 ) | ( 'N' << 24 ) )

}}

#endif
