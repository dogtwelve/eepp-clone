#ifndef EE_WINDOWCCLIPBOARD_HPP
#define EE_WINDOWCCLIPBOARD_HPP

#include <eepp/window/window.hpp>

namespace EE { namespace Window {

class EE_API Clipboard {
	public:
		virtual ~Clipboard();

		/** @return The Clipboard Text if available */
		virtual std::string GetText() = 0;

		/** @return The Clipboard Text if available ( as String ) */
		virtual String GetWideText() = 0;

		/** Set the current clipboard text */
		virtual void SetText( const std::string& Text ) = 0;

		/** @return The parent window of the clipboard */
		EE::Window::Window * GetWindow() const;
	protected:
		friend class Window;

		Clipboard( EE::Window::Window * window );

		/** Initialize the clipboard manager. This is needed because the backends first create the instance of the clipboard and then initialize the window context. */
		virtual void Init() = 0;

		EE::Window::Window * mWindow;
};

}}

#endif
