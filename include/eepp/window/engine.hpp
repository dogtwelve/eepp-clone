#ifndef EE_WINDOWCENGINE_H
#define EE_WINDOWCENGINE_H

#include <eepp/window/base.hpp>
#include <eepp/window/window.hpp>
#include <list>

namespace EE { namespace System { class IniFile; } }
namespace EE { namespace Window { namespace Backend { class WindowBackend; } } }

namespace EE { namespace Window {

/** @brief The window management class. Here the engine starts working. (Singleton Class). */
class EE_API Engine {
	SINGLETON_DECLARE_HEADERS(Engine)

	public:
		~Engine();

		/** Creates a new window.
		* SDL2, SFML backends support more than one window creation, SDL 1.2 backend only 1 window.
		*/
		EE::Window::Window * createWindow( WindowSettings Settings, ContextSettings Context = ContextSettings() );

		/** Destroy the window instance, and set as current other window running ( if any ).
		* This function is only useful for multi-window environment. Avoid using it with one window context.
		*/
		void destroyWindow( EE::Window::Window * window );

		/** @return The current Window context. */
		EE::Window::Window * getCurrentWindow() const;

		/** Set the window as the current. */
		void setCurrentWindow( EE::Window::Window * window );

		/** @return The number of windows created. */
		Uint32	getWindowCount() const;

		/** @return If any window is created. */
		bool isRunning() const;

		/** @return The current window elapsed time. */
		Time getElapsed() const;

		/** @return The current window width. */
		const Uint32& getWidth() const;

		/** @return The current window height */
		const Uint32& getHeight() const;

		/** @return If the window instance is inside the window list. */
		bool existsWindow( EE::Window::Window * window );

		/** Constructs WindowSettings from an ini file
		It will search for the following properties:
			Width			Window width
			Height			Window height
			BitColor		32,16,8
			Windowed		bool
			Resizeable		bool
			Backend			SDL, SDL2 or SFML
			WinIcon			The path to the window icon
			WinCaption		The window default title

			@param iniPath The ini file path
			@param iniKeyName The ini key name to search the properties
		*/
		WindowSettings createWindowSettings( std::string iniPath, std::string iniKeyName = "EEPP" );

		/** Constructs WindowSettings from an ini file instance
		It will search for the following properties:
			Width			Window width
			Height			Window height
			BitColor		32,16,8
			Windowed		bool
			Resizeable		bool
			Backend			SDL, SDL2 or SFML
			WinIcon			The path to the window icon
			WinCaption		The window default title

			@param ini The ini file instance
			@param iniKeyName The ini key name to search the properties
		*/
		WindowSettings createWindowSettings( IniFile * ini, std::string iniKeyName = "EEPP" );

		/** Constructs ContextSettings from an ini file\n
		It will search for the following properties:
			VSync				bool
			GLVersion			Selects the default renderer: 2 for OpenGL 2, 3 for OpenGL 3, 4 for OpenGL ES 2
			DoubleBuffering		bool
			DepthBufferSize		int
			StencilBufferSize	int

			@param iniPath The ini file path
			@param iniKeyName The ini key name to search the properties
		*/
		ContextSettings createContextSettings( std::string iniPath, std::string iniKeyName = "EEPP" );

		/** Constructs ContextSettings from an ini file instance\n
		It will search for the following properties:
			VSync				bool
			GLVersion			Selects the default renderer: 2 for OpenGL 2, 3 for OpenGL 3, 4 for OpenGL ES 2
			DoubleBuffering		bool
			DepthBufferSize		int
			StencilBufferSize	int

			@param ini The ini file instance
			@param iniKeyName The ini key name to search the properties
		*/
		ContextSettings createContextSettings( IniFile * ini, std::string iniKeyName = "EEPP" );

		/** Enabling Shared GL Context allows asynchronous OpenGL resource loading ( only if is supported by the backend and the OS, SDL 2 backend is the only one supported ).
		**	If the TextureLoader is threaded, will upload the texture in another thread to the GPU. So, it will not block the main rendering thread.
		**	Shared GL Context is disabled by default.
		*/
		void enableSharedGLContext();

		/** Disable the Shared GL Context
		**	@see enableSharedGLContext()
		*/
		void disableSharedGLContext();

		/** @return If the Shared GL Context is enabled and ready to use. */
		bool isSharedGLContextEnabled();

		/** @return The id of the thread that was used to initialize the OpenGL Context. */
		Uint32 getMainThreadId();
	protected:
		friend class Window;

		Backend::WindowBackend *	mBackend;
		std::list<Window*>	mWindows;
		EE::Window::Window *			mWindow;
		bool				mSharedGLContext;
		Uint32				mMainThreadId;

		Engine();

		void destroy();

		Backend::WindowBackend * createSDL2Backend( const WindowSettings& Settings );

		Backend::WindowBackend * createSFMLBackend( const WindowSettings& Settings );

		EE::Window::Window * createSDL2Window( const WindowSettings& Settings, const ContextSettings& Context );

		EE::Window::Window * createSFMLWindow( const WindowSettings& Settings, const ContextSettings& Context );

		EE::Window::Window * createDefaultWindow( const WindowSettings& Settings, const ContextSettings& Context );

		Uint32 getDefaultBackend() const;
};

}}

#endif
