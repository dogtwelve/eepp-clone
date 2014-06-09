#include <eepp/graphics/cshader.hpp>
#include <eepp/graphics/renderer/cgl.hpp>
#include <eepp/graphics/renderer/crenderergl3.hpp>
#include <eepp/graphics/renderer/crenderergl3cp.hpp>
#include <eepp/graphics/renderer/crenderergles2.hpp>

namespace EE { namespace Graphics {

bool cShader::sEnsure = true;

void cShader::Ensure( bool ensure ) {
	sEnsure = ensure;
}

bool cShader::Ensure() {
	return sEnsure;
}

cShader::cShader( const Uint32& Type ) {
	Init( Type );
}

cShader::cShader( const Uint32& Type, const std::string& Filename ) {
    Init( Type );

	mFilename = FileSystem::FileNameFromPath( Filename );

	if ( FileSystem::FileExists( Filename ) ) {
		SafeDataPointer PData;

		FileSystem::FileGet( Filename, PData );

		SetSource( (const char*)PData.Data, PData.DataSize );
	} else {
		std::string tPath = Filename;
		cPack * tPack = NULL;

		if ( cPackManager::instance()->FallbackToPacks() && NULL != ( tPack = cPackManager::instance()->Exists( tPath ) ) ) {
			SafeDataPointer PData;

			tPack->ExtractFileToMemory( tPath, PData );

			SetSource( reinterpret_cast<char*> ( PData.Data ), PData.DataSize );
		} else {
			eePRINTL( "Couldn't open shader object: %s", Filename.c_str() );
		}
	}

	Compile();
}

cShader::cShader( const Uint32& Type, const char * Data, const Uint32& DataSize ) {
	Init( Type );

	SetSource( Data, DataSize );

	Compile();
}

cShader::cShader( const Uint32& Type, cPack * Pack, const std::string& Filename ) {
	SafeDataPointer PData;

	Init( Type );

	mFilename = FileSystem::FileNameFromPath( Filename );

	if ( NULL != Pack && Pack->IsOpen() && -1 != Pack->Exists( Filename ) ) {
		Pack->ExtractFileToMemory( Filename, PData );

		SetSource( reinterpret_cast<char*> ( PData.Data ), PData.DataSize );
	}

	Compile();
}

cShader::cShader( const Uint32& Type, const char ** Data, const Uint32& NumLines ) {
	Init( Type );

	SetSource( Data, NumLines );

	Compile();
}

cShader::~cShader() {
	if ( 0 != mGLId ) {
		#ifdef EE_SHADERS_SUPPORTED
		glDeleteShader( mGLId );
		#endif
	}
}

void cShader::Init( const Uint32& Type ) {
	mType 		= Type;
	mValid 		= false;
	mCompiled 	= false;
	#ifdef EE_SHADERS_SUPPORTED
	mGLId 		= glCreateShader( mType );
	#endif
}

void cShader::Reload() {
	Init( mType );

	cShader::Ensure( false );
	SetSource( mSource );
	cShader::Ensure( true );

	Compile();
}

std::string cShader::GetName() {
	std::string name;

	if ( mFilename.size() ) {
		name = mFilename;
	} else {
		name = String::ToStr( mGLId );
	}

	return name;
}

void cShader::EnsureVersion() {
	#ifdef EE_GL3_ENABLED
	if ( cShader::Ensure() && ( GLi->Version() == GLv_3 || GLi->Version() == GLv_3CP || GLi->Version() == GLv_ES2 ) ) {
		eePRINTL( "Shader %s converted to programmable pipeline automatically.", GetName().c_str() );

		if ( GL_VERTEX_SHADER == mType ) {
			if ( mSource.find( "ftransform" ) != std::string::npos || mSource.find("dgl_Vertex") == std::string::npos ) {
				if ( GLi->Version() == GLv_3 ) {
					mSource = GLi->GetRendererGL3()->GetBaseVertexShader();
				} else if ( GLi->Version() == GLv_3CP ) {
					mSource = GLi->GetRendererGL3CP()->GetBaseVertexShader();
				} else {
					mSource = GLi->GetRendererGLES2()->GetBaseVertexShader();
				}
			}
		} else {
			if ( mSource.find( "gl_FragColor" ) != std::string::npos ) {
				#ifndef EE_GLES
				if ( GLi->Version() != GLv_3CP )
				#else
				if ( true )
				#endif
				{
					#ifdef EE_GLES
					std::string preSource = "#ifdef GL_ES\nprecision mediump float;\nprecision lowp int;\n#endif";
					#else
					std::string preSource = "#version 120";
					#endif

					mSource = preSource + "\nvarying	vec4		gl_Color;\nvarying	vec4		gl_TexCoord[ 1 ];\n" + mSource;
				} else {
					mSource = "#version 330\nin	vec4		gl_Color;\nin	vec4		gl_TexCoord[ 1 ];\nout		vec4		gl_FragColor;\n" + mSource;
				}

				String::ReplaceSubStr( mSource, "gl_Color"		, "dgl_Color"		);
				String::ReplaceSubStr( mSource, "gl_TexCoord"	, "dgl_TexCoord"	);

				if ( GLi->Version() == GLv_3CP ) {
					#ifndef EE_GLES
					String::ReplaceSubStr( mSource, "gl_FragColor"	, "dgl_FragColor"	);
					#endif
				}
			}
		}
	}

	if ( GLi->Version() == GLv_3CP ) {
		#ifndef EE_GLES
		String::ReplaceSubStr( mSource, "texture2D"	, "texture"	);
		#endif
	}
	#endif
}

void cShader::SetSource( const std::string& Source ) {
	if ( IsCompiled() ) {
		eePRINTL( "Shader %s report: can't set source for compiled shaders", GetName().c_str() );
		return;
	}

	mSource = Source;

	EnsureVersion();

	#ifdef EE_SHADERS_SUPPORTED
	const char * src = reinterpret_cast<const char *> ( &mSource[0] );

	glShaderSource( mGLId, 1, &src, NULL );
	#endif
}

void cShader::SetSource( const char** Data, const Uint32& NumLines ) {
	std::string tstr;

	for ( Uint32 i = 0; i < NumLines; i++ ) {
		tstr += std::string( Data[i] );
	}

	SetSource( tstr );
}

void cShader::SetSource( const char * Data, const Uint32& DataSize ) {
	std::string _dst( DataSize, 0 );

	memcpy( reinterpret_cast<void*>( &_dst[0] ), reinterpret_cast<const void*>( &Data[0] ), DataSize );

	SetSource( _dst );
}

void cShader::SetSource( const std::vector<Uint8>& Source ) {
	std::string _dst( Source.size(), 0 );

    memcpy( reinterpret_cast<void*>( &_dst[0] ), reinterpret_cast<const void*>( &Source[0] ), Source.size() );

    SetSource( _dst );
}

bool cShader::Compile() {
	if ( IsCompiled() ) {
		eePRINTL( "Shader %s report: can't compile a shader twice", GetName().c_str() );
		return false;
	}

	#ifdef EE_SHADERS_SUPPORTED

	glCompileShader( GetId() );
	mCompiled = true;

	int Compiled;
	glGetShaderiv( GetId(), GL_COMPILE_STATUS, &Compiled );
	mValid = 0 != Compiled;

	if ( !mValid ) {
		GLsizei logsize = 0, logarraysize = 0;
		glGetShaderiv( GetId(), GL_INFO_LOG_LENGTH, &logarraysize );

		if ( logarraysize > 0 ) {
			mCompileLog.resize( logarraysize - 1 );

			glGetShaderInfoLog( GetId(), logarraysize, &logsize, reinterpret_cast<GLchar*>( &mCompileLog[0] ) );
		}

		eePRINTL( "Couldn't compile shader %s. Log follows:\n", GetName().c_str() );
		eePRINTL( mCompileLog.c_str() );
		eePRINTL( mSource.c_str() );
	} else {
		eePRINTL( "Shader %s compiled succesfully", GetName().c_str() );
	}

	#endif

	return mValid;
}

bool cShader::IsValid() const {
	return mValid;
}

bool cShader::IsCompiled() const {
	return mCompiled;
}

std::string cShader::CompileLog() const {
	return mCompileLog;
}

Uint32 cShader::GetType() const {
	return mType;
}

Uint32 cShader::GetId() const {
	return mGLId;
}

cVertexShader::cVertexShader() :
	cShader( GL_VERTEX_SHADER )
{
}

cVertexShader::cVertexShader( const std::string& Filename ) :
	cShader( GL_VERTEX_SHADER, Filename )
{
}

cVertexShader::cVertexShader( const char * Data, const Uint32& DataSize ) :
	cShader( GL_VERTEX_SHADER, Data, DataSize )
{
}

cVertexShader::cVertexShader( cPack * Pack, const std::string& Filename ) :
	cShader( GL_VERTEX_SHADER, Pack, Filename )
{
}

cVertexShader::cVertexShader( const char ** Data, const Uint32& NumLines ) :
	cShader( GL_VERTEX_SHADER, Data, NumLines )
{
}

cFragmentShader::cFragmentShader() :
	cShader( GL_FRAGMENT_SHADER )
{
}

cFragmentShader::cFragmentShader( const std::string& Filename ) :
	cShader( GL_FRAGMENT_SHADER, Filename )
{
}

cFragmentShader::cFragmentShader( const char * Data, const Uint32& DataSize ) :
	cShader( GL_FRAGMENT_SHADER, Data, DataSize )
{
}

cFragmentShader::cFragmentShader( cPack * Pack, const std::string& Filename ) :
	cShader( GL_FRAGMENT_SHADER, Pack, Filename )
{
}

cFragmentShader::cFragmentShader( const char ** Data, const Uint32& NumLines ) :
	cShader( GL_FRAGMENT_SHADER, Data, NumLines )
{
}

}}
