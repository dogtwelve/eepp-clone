#ifndef EE_WINDOWCINPUTSDL_HPP
#define EE_WINDOWCINPUTSDL_HPP

#include <eepp/window/cbackend.hpp>
#include <eepp/window/backend/SDL/base.hpp>

#ifdef EE_BACKEND_SDL_1_2

#include <eepp/window/cinput.hpp>

namespace EE { namespace Window { namespace Backend { namespace SDL {

class EE_API cInputSDL : public cInput {
	public:
		virtual ~cInputSDL();

		void Update();

		bool GrabInput();

		void GrabInput( const bool& Grab );

		void InjectMousePos( const Uint16& x, const Uint16& y );
	protected:
		friend class cWindowSDL;

		cInputSDL( Window::cWindow * window );

		virtual void Init();
};

}}}}

#endif

#endif
