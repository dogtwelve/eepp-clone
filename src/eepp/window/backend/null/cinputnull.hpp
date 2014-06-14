#ifndef EE_WINDOWCINPUTNULL_HPP 
#define EE_WINDOWCINPUTNULL_HPP

#include <eepp/window/cinput.hpp>

namespace EE { namespace Window { namespace Backend { namespace Null {

class EE_API InputNull : public Input {
	public:
		virtual ~InputNull();
		
		void Update();

		bool GrabInput();

		void GrabInput( const bool& Grab );

		void InjectMousePos( const Uint16& x, const Uint16& y );
	protected:
		friend class cWindowNull;

		InputNull( Window::cWindow * window );
		
		virtual void Init();
};

}}}}

#endif
