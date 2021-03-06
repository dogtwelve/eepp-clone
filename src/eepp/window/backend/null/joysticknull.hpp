#ifndef EE_WINDOWCJOYSTICKNULL_HPP
#define EE_WINDOWCJOYSTICKNULL_HPP

#include <eepp/window/joystick.hpp>

namespace EE { namespace Window { namespace Backend { namespace Null {

class EE_API JoystickNull : public Joystick {
	public:
		JoystickNull( const Uint32& index );

		virtual ~JoystickNull();

		void 		Close();

		void 		Open();

		void		Update();

		Uint8		GetHat( const Int32& index );

		Float		GetAxis( const Int32& axis );

		Vector2i	GetBallMotion( const Int32& ball );

		bool		Plugged() const;
	protected:
};

}}}}

#endif
