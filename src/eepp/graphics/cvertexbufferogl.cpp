#include <eepp/graphics/cvertexbufferogl.hpp>
#include <eepp/graphics/renderer/cgl.hpp>

namespace EE { namespace Graphics {

cVertexBufferOGL::cVertexBufferOGL( const Uint32& VertexFlags, EE_DRAW_MODE DrawType, const Int32& ReserveVertexSize, const Int32& ReserveIndexSize, EE_VBO_USAGE_TYPE UsageType ) :
	cVertexBuffer( VertexFlags, DrawType, ReserveVertexSize, ReserveIndexSize, UsageType )
{
}

cVertexBufferOGL::~cVertexBufferOGL() {
}

void cVertexBufferOGL::Bind() {
	SetVertexStates();
}

bool cVertexBufferOGL::Compile() {
	return true;
}

void cVertexBufferOGL::Draw() {
	if( VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_USE_INDICES ) ) {
		Int32 lSize = mElemDraw;

		if( mElemDraw < 0 )
			lSize = GetIndexCount();

		GLi->DrawElements( mDrawType, lSize, GL_UNSIGNED_INT, &mIndexArray[0] );
	} else {
		GLi->DrawArrays( mDrawType, 0, GetVertexCount() );
	}
}

void cVertexBufferOGL::SetVertexStates() {
	Uint32 alloc	= GetVertexCount() * sizeof(eeFloat) * 2;
	Uint32 allocC	= GetVertexCount() * 4;

	/// POSITION
	if( VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_POSITION ) ) {
		GLi->EnableClientState( GL_VERTEX_ARRAY );
		GLi->VertexPointer( eeVertexElements[ VERTEX_FLAG_POSITION ], GL_FP, sizeof(eeFloat) * eeVertexElements[ VERTEX_FLAG_POSITION ], &mVertexArray[ VERTEX_FLAG_POSITION ][0], alloc );
	} else {
		GLi->DisableClientState( GL_VERTEX_ARRAY );
	}

	/// COLOR
	if( VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_COLOR ) ) {
		GLi->EnableClientState( GL_COLOR_ARRAY );
		GLi->ColorPointer( eeVertexElements[ VERTEX_FLAG_COLOR ], GL_UNSIGNED_BYTE, sizeof(Uint8) * eeVertexElements[ VERTEX_FLAG_COLOR ], &mColorArray[0], allocC );
	} else {
		GLi->DisableClientState( GL_COLOR_ARRAY );
	}

	/// TEXTURES
	if ( GLi->IsExtension( EEGL_ARB_multitexture ) ) {
		for ( Int32 i = 0; i < EE_MAX_TEXTURE_UNITS; i++ ) {
			if( VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_TEXTURE0 + i ) ) {
				GLi->ClientActiveTexture( GL_TEXTURE0 + i );
				GLi->EnableClientState( GL_TEXTURE_COORD_ARRAY );

				GLi->TexCoordPointer( eeVertexElements[ VERTEX_FLAG_TEXTURE0 + i ], GL_FP, sizeof(eeFloat) * eeVertexElements[ VERTEX_FLAG_TEXTURE0 + i ], &mVertexArray[ VERTEX_FLAG_TEXTURE0 + i ][0], alloc );
			} else {
				if ( 0 == i ) {
					GLi->Disable( GL_TEXTURE_2D );
					GLi->DisableClientState( GL_TEXTURE_COORD_ARRAY );
					break;
				}
			}
		}
	} else {
		if ( VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_TEXTURE0 ) ) {
			GLi->EnableClientState( GL_TEXTURE_COORD_ARRAY );
			GLi->TexCoordPointer( eeVertexElements[ VERTEX_FLAG_TEXTURE0 ], GL_FP, sizeof(eeFloat) * eeVertexElements[ VERTEX_FLAG_TEXTURE0 ], &mVertexArray[ VERTEX_FLAG_TEXTURE0 ][0], alloc );
		} else {
			GLi->Disable( GL_TEXTURE_2D );
			GLi->DisableClientState( GL_TEXTURE_COORD_ARRAY );
		}
	}

	GLi->ClientActiveTexture( GL_TEXTURE0 );
}


void cVertexBufferOGL::Unbind() {
	if( !VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_POSITION ) ) {
		GLi->EnableClientState( GL_VERTEX_ARRAY );
	}

	if( !VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_COLOR ) ) {
		GLi->EnableClientState( GL_COLOR_ARRAY );
	}

	if( !VERTEX_FLAG_QUERY( mVertexFlags, VERTEX_FLAG_TEXTURE0 ) ) {
		GLi->Enable( GL_TEXTURE_2D );
		GLi->EnableClientState( GL_TEXTURE_COORD_ARRAY );
	}
}

void cVertexBufferOGL::Update( const Uint32& Types, bool Indices ) {
}

void cVertexBufferOGL::Reload() {
}

}}
