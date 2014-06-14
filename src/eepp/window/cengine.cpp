#include <eepp/window/cengine.hpp>
#include <eepp/system/packmanager.hpp>
#include <eepp/system/inifile.hpp>
#include <eepp/graphics/ctexturefactory.hpp>
#include <eepp/graphics/cfontmanager.hpp>
#include <eepp/graphics/cglobalbatchrenderer.hpp>
#include <eepp/graphics/cshaderprogrammanager.hpp>
#include <eepp/graphics/ctextureatlasmanager.hpp>
#include <eepp/graphics/cframebuffermanager.hpp>
#include <eepp/graphics/cvertexbuffermanager.hpp>
#include <eepp/ui/cuimanager.hpp>
#include <eepp/audio/audiolistener.hpp>
#include <eepp/helper/haikuttf/hkfontmanager.hpp>
#include <eepp/physics/cphysicsmanager.hpp>
#include <eepp/network/ssl/sslsocket.hpp>
#include <eepp/window/cbackend.hpp>
#include <eepp/window/backend/SDL/cbackendsdl.hpp>
#include <eepp/window/backend/SDL2/cbackendsdl2.hpp>
#include <eepp/window/backend/SFML/cbackendsfml.hpp>
#include <eepp/graphics/renderer/cgl.hpp>

#define BACKEND_SDL			1
#define BACKEND_SDL2		2
#define BACKEND_SFML		3

#ifndef DEFAULT_BACKEND

#if defined( EE_BACKEND_SDL2 )
#define DEFAULT_BACKEND		BACKEND_SDL2
#elif defined( EE_BACKEND_SDL_1_2 )
#define DEFAULT_BACKEND		BACKEND_SDL
#elif defined( EE_BACKEND_SFML_ACTIVE )
#define DEFAULT_BACKEND		BACKEND_SFML
#endif

#endif

namespace EE { namespace Window {

SINGLETON_DECLARE_IMPLEMENTATION(cEngine)

cEngine::cEngine() :
	mBackend( NULL ),
	mWindow( NULL ),
	mSharedGLContext( false ),
	mMainThreadId( 0 )
{
	cTextureAtlasManager::CreateSingleton();
}

cEngine::~cEngine() {
	Physics::cPhysicsManager::DestroySingleton();

	Graphics::Private::cFrameBufferManager::DestroySingleton();

	Graphics::Private::cVertexBufferManager::DestroySingleton();

	cGlobalBatchRenderer::DestroySingleton();

	cTextureFactory::DestroySingleton();

	cTextureAtlasManager::DestroySingleton();

	cFontManager::DestroySingleton();

	UI::cUIManager::DestroySingleton();

	Graphics::cGL::DestroySingleton();

	cShaderProgramManager::DestroySingleton();

	PackManager::DestroySingleton();

	Log::DestroySingleton();

	HaikuTTF::hkFontManager::DestroySingleton();

	#ifdef EE_SSL_SUPPORT
	Network::SSL::SSLSocket::End();
	#endif

	Destroy();

	eeSAFE_DELETE( mBackend );
}

void cEngine::Destroy() {
	std::list<cWindow*>::iterator it;

	for ( it = mWindows.begin(); it != mWindows.end(); it++ ) {
		eeSAFE_DELETE( *it );
	}

	mWindow = NULL;
}

Backend::cBackend * cEngine::CreateSDLBackend( const WindowSettings &Settings ) {
#if defined( EE_SDL_VERSION_1_2 )
	return eeNew( Backend::SDL::cBackendSDL, () );
#else
	return NULL;
#endif
}

Backend::cBackend * cEngine::CreateSDL2Backend( const WindowSettings &Settings ) {
#if defined( EE_SDL_VERSION_2 )
	return eeNew( Backend::SDL2::cBackendSDL2, () );
#else
	return NULL;
#endif
}

Backend::cBackend * cEngine::CreateSFMLBackend( const WindowSettings &Settings ) {
#if defined( EE_BACKEND_SFML_ACTIVE )
	return eeNew( Backend::SFML::cBackendSFML, () );
#else
	return NULL;
#endif
}

cWindow * cEngine::CreateSDLWindow( const WindowSettings& Settings, const ContextSettings& Context ) {
#if defined( EE_SDL_VERSION_1_2 )
	if ( NULL == mBackend ) {
		mBackend	= CreateSDLBackend( Settings );
	}

	return eeNew( Backend::SDL::cWindowSDL, ( Settings, Context ) );
#else
	return NULL;
#endif
}

cWindow * cEngine::CreateSDL2Window( const WindowSettings& Settings, const ContextSettings& Context ) {
#if defined( EE_SDL_VERSION_2 )
	if ( NULL == mBackend ) {
		mBackend	= CreateSDL2Backend( Settings );
	}

	return eeNew( Backend::SDL2::cWindowSDL, ( Settings, Context ) );
#else
	return NULL;
#endif
}

cWindow * cEngine::CreateSFMLWindow( const WindowSettings& Settings, const ContextSettings& Context ) {
#if defined( EE_BACKEND_SFML_ACTIVE )

	if ( NULL == mBackend ) {
		mBackend	= CreateSFMLBackend( Settings );
	}

	return eeNew( Backend::SFML::cWindowSFML, ( Settings, Context ) );
#else
	return NULL;
#endif
}

cWindow * cEngine::CreateDefaultWindow( const WindowSettings& Settings, const ContextSettings& Context ) {
#if DEFAULT_BACKEND == BACKEND_SDL
	return CreateSDLWindow( Settings, Context );
#elif DEFAULT_BACKEND == BACKEND_SDL2
	return CreateSDL2Window( Settings, Context );
#elif DEFAULT_BACKEND == BACKEND_SFML
	return CreateSFMLWindow( Settings, Context );
#endif
}

cWindow * cEngine::CreateWindow( WindowSettings Settings, ContextSettings Context ) {
	cWindow * window = NULL;

	if ( NULL != mWindow ) {
		Settings.Backend	= mWindow->GetWindowInfo()->WindowConfig.Backend;
	} else {
		mMainThreadId	= Thread::GetCurrentThreadId();
	}

	switch ( Settings.Backend ) {
		case WindowBackend::SDL:		window = CreateSDLWindow( Settings, Context );		break;
		case WindowBackend::SDL2:		window = CreateSDL2Window( Settings, Context );		break;
		case WindowBackend::SFML:		window = CreateSFMLWindow( Settings, Context );		break;
		case WindowBackend::Default:
		default:						window = CreateDefaultWindow( Settings, Context );	break;
	}

	if ( NULL == window ) {
		window = CreateDefaultWindow( Settings, Context );
	}

	if ( NULL == mWindow ) {
		mWindow = window;
	}

	mWindows.push_back( mWindow );

	return window;
}

void cEngine::DestroyWindow( cWindow * window ) {
	mWindows.remove( window );

	if ( window == mWindow ) {
		if ( mWindows.size() > 0 ) {
			mWindow = mWindows.back();
		} else {
			mWindow = NULL;
		}
	}

	eeSAFE_DELETE( window );
}

bool cEngine::ExistsWindow( cWindow * window ) {
	std::list<cWindow*>::iterator it;

	for ( it = mWindows.begin(); it != mWindows.end(); it++ ) {
		if ( (*it) == window )
			return true;
	}

	return false;
}

cWindow * cEngine::GetCurrentWindow() const {
	return mWindow;
}

void cEngine::SetCurrentWindow( cWindow * window ) {
	if ( NULL != window && window != mWindow ) {
		mWindow = window;

		mWindow->SetCurrent();
	}
}

Uint32 cEngine::GetWindowCount() const {
	return mWindows.size();
}

bool cEngine::Running() const {
	return NULL != mWindow;
}

Time cEngine::Elapsed() const {
	eeASSERT( Running() );

	return mWindow->Elapsed();
}

const Uint32& cEngine::GetWidth() const {
	eeASSERT( Running() );

	return mWindow->GetWidth();
}

const Uint32& cEngine::GetHeight() const {
	eeASSERT( Running() );

	return mWindow->GetHeight();
}

Uint32 cEngine::GetDefaultBackend() const {
#if DEFAULT_BACKEND == BACKEND_SDL
	return WindowBackend::SDL;
#elif DEFAULT_BACKEND == BACKEND_SDL2
	return WindowBackend::SDL2;
#elif DEFAULT_BACKEND == BACKEND_SFML
	return WindowBackend::SFML;
#endif
}

WindowSettings cEngine::CreateWindowSettings( IniFile * ini, std::string iniKeyName ) {
	eeASSERT ( NULL != ini );

	ini->ReadFile();

	int Width 			= ini->GetValueI( iniKeyName, "Width", 800 );
	int Height			= ini->GetValueI( iniKeyName, "Height", 600 );
	int BitColor		= ini->GetValueI( iniKeyName, "BitColor", 32);
	bool Windowed		= ini->GetValueB( iniKeyName, "Windowed", true );
	bool Resizeable		= ini->GetValueB( iniKeyName, "Resizeable", true );

	std::string Backend = ini->GetValue( iniKeyName, "Backend", "" );
	Uint32 WinBackend	= GetDefaultBackend();

	String::ToLower( Backend );

	if ( "sdl2" == Backend )		WinBackend	= WindowBackend::SDL2;
	else if ( "sdl" == Backend )	WinBackend	= WindowBackend::SDL;
	else if ( "sfml" == Backend )	WinBackend	= WindowBackend::SFML;

	Uint32 Style = WindowStyle::Titlebar;

	if ( !Windowed )
		Style |= WindowStyle::Fullscreen;

	if ( Resizeable )
		Style |= WindowStyle::Resize;

	std::string Icon	= ini->GetValue( iniKeyName, "WinIcon", "" );
	std::string Caption	= ini->GetValue( iniKeyName, "WinCaption", "" );

	WindowSettings WinSettings( Width, Height, Caption, Style, WinBackend, BitColor, Icon );

	#if EE_PLATFORM == EE_PLATFORM_IOS
	//! @TODO: Check if SDL2 default win settings are being forced ( it wasn't working fine some time ago )
	WinSettings.Width	= 960;
	WinSettings.Height	= 640;
	WinSettings.Style	= WindowStyle::NoBorder;
	#endif

	return WinSettings;
}

WindowSettings cEngine::CreateWindowSettings( std::string iniPath, std::string iniKeyName ) {
	IniFile Ini( iniPath );

	return CreateWindowSettings( &Ini, iniKeyName );
}

ContextSettings cEngine::CreateContextSettings( IniFile * ini, std::string iniKeyName ) {
	eeASSERT ( NULL != ini );

	ini->ReadFile();

	bool VSync					= ini->GetValueB( iniKeyName, "VSync", true );
	std::string GLVersion		= ini->GetValue( iniKeyName, "GLVersion", "0" );

	String::ToLower( GLVersion );

	EEGL_version GLVer;
	if (		"3" == GLVersion || "opengl 3" == GLVersion || "gl3" == GLVersion || "opengl3" == GLVersion )									GLVer = GLv_3;
	else if (	"4" == GLVersion || "opengl es 2" == GLVersion || "gles2" == GLVersion || "opengles2" == GLVersion || "es2" == GLVersion )		GLVer = GLv_ES2;
	else if (	"5" == GLVersion || "opengl 3 core profile" == GLVersion ||
				"gl3cp" == GLVersion || "opengl3cp" == GLVersion || "opengl core profile" == GLVersion ||
				"core profile" == GLVersion || "cp" == GLVersion )																				GLVer = GLv_3CP;
	else if (	"opengl es 1" == GLVersion || "gles1" == GLVersion || "gl es 1" == GLVersion || "opengl es1" == GLVersion ||
				"opengles1" == GLVersion || "es1" == GLVersion || "gles 1" == GLVersion )														GLVer = GLv_ES1;
	else if (	"2" == GLVersion || "opengl 2" == GLVersion || "gl2" == GLVersion || "gl 2" == GLVersion )										GLVer = GLv_2;
	else																																		GLVer = GLv_default;

	bool doubleBuffering 		= ini->GetValueB( iniKeyName, "DoubleBuffering", true );
	int depthBufferSize 		= ini->GetValueI( iniKeyName, "DepthBufferSize", 24 );
	int stencilBufferSize 		= ini->GetValueI( iniKeyName, "StencilBufferSize", 1 );

	return ContextSettings( VSync, GLVer, doubleBuffering, depthBufferSize, stencilBufferSize );
}

ContextSettings cEngine::CreateContextSettings( std::string iniPath, std::string iniKeyName ) {
	IniFile Ini( iniPath );

	return CreateContextSettings( &Ini );
}

void cEngine::EnableSharedGLContext() {
	mSharedGLContext = true;
}

void cEngine::DisableSharedGLContext() {
	mSharedGLContext = false;
}

bool cEngine::IsSharedGLContextEnabled() {
	return mSharedGLContext;
}

Uint32 cEngine::GetMainThreadId() {
	return mMainThreadId;
}

}}
