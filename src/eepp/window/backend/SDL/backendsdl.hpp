#ifndef EE_WINDOWCBACKENDSDL_HPP
#define EE_WINDOWCBACKENDSDL_HPP

#include <eepp/window/backend.hpp>
#include <eepp/window/backend/SDL/base.hpp>

#ifdef EE_BACKEND_SDL_1_2

#include <eepp/window/backend/SDL/windowsdl.hpp>

namespace EE { namespace Window { namespace Backend { namespace SDL {

class EE_API WindowBackendSDL : public WindowBackend {
	public:
		WindowBackendSDL();

		~WindowBackendSDL();
};

}}}}

#endif

#endif
