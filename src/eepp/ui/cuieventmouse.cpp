#include <eepp/ui/cuieventmouse.hpp>
#include <eepp/ui/cuicontrol.hpp>

namespace EE { namespace UI {

cUIEventMouse::cUIEventMouse( cUIControl * Ctrl, const Uint32& EventNum, const Vector2i& Pos, const Uint32& Flags ) :
	cUIEvent( Ctrl, EventNum ),
	mPos( Pos ),
	mFlags( Flags )
{
}

cUIEventMouse::~cUIEventMouse()
{
}

const Vector2i& cUIEventMouse::Pos() const {
	return mPos;
}

const Uint32& cUIEventMouse::Flags() const {
	return mFlags;
}

}}

