#ifndef EE_WINDOWCCURSORMANAGERSFML_HPP
#define EE_WINDOWCCURSORMANAGERSFML_HPP

#ifdef EE_BACKEND_SFML_ACTIVE

#include <eepp/window/cursormanager.hpp>

using namespace EE::Window;

namespace EE { namespace Window { namespace Backend { namespace SFML {

class CursorManagerSFML : public CursorManager {
	public:
		CursorManagerSFML( EE::Window::Window * window );

		Cursor * Create( Texture * tex, const Vector2i& hotspot, const std::string& name );

		Cursor * Create( Image * img, const Vector2i& hotspot, const std::string& name );

		Cursor * Create( const std::string& path, const Vector2i& hotspot, const std::string& name );

		void Set( Cursor * cursor );

		void Set( EE_SYSTEM_CURSOR syscurid );

		void Show();

		void Hide();

		void Visible( bool visible );

		void Remove( Cursor * cursor, bool Delete = false );

		void Reload();
};

}}}}

#endif

#endif
