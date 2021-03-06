#ifndef EE_WINDOWCJOYSTICKSFML_HPP
#define EE_WINDOWCJOYSTICKSFML_HPP

#ifdef EE_BACKEND_SFML_ACTIVE

#include <eepp/window/joystick.hpp>

namespace EE { namespace Window { namespace Backend { namespace SFML {

class EE_API JoystickSFML : public Joystick {
	public:
		JoystickSFML( const Uint32& index );

		virtual ~JoystickSFML();

		void 		Close();

		void 		Open();

		void		Update();

		Uint8		GetHat( const Int32& index );

		Float		GetAxis( const Int32& axis );

		Vector2i	GetBallMotion( const Int32& ball );

		bool		Plugged() const;
	protected:
		void		CalcHat();

		Uint8		mHat;
};

}}}}

#endif

#endif
