#ifndef EE_WINDOWCJOYSTICKSDL_HPP
#define EE_WINDOWCJOYSTICKSDL_HPP

#include <eepp/window/backend.hpp>
#include <eepp/window/backend/SDL/base.hpp>

#ifdef EE_BACKEND_SDL_1_2

#include <eepp/window/joystick.hpp>
#if !defined( EE_COMPILER_MSVC )
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

namespace EE { namespace Window { namespace Backend { namespace SDL {

class EE_API JoystickSDL : public Joystick {
	public:
		JoystickSDL( const Uint32& index );

		virtual ~JoystickSDL();

		void 		Close();

		void 		Open();

		void		Update();

		Uint8		GetHat( const Int32& index );

		Float		GetAxis( const Int32& axis );

		Vector2i	GetBallMotion( const Int32& ball );

		bool		Plugged() const;
	protected:
		SDL_Joystick * mJoystick;
};

}}}}

#endif

#endif
