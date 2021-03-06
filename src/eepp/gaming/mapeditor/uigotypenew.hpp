#ifndef EE_GAMINGCUIGOTYPENEW_HPP
#define EE_GAMINGCUIGOTYPENEW_HPP

#include <eepp/gaming/base.hpp>
#include <eepp/ui/uiwindow.hpp>

using namespace EE::UI;

namespace EE { namespace Gaming { namespace Private {

class EE_API UIGOTypeNew {
	public:
		UIGOTypeNew( cb::Callback2<void, std::string, Uint32> Cb );

		virtual ~UIGOTypeNew();
	protected:
		UITheme *			mUITheme;
		UIWindow *			mUIWindow;
		UITextInput *		mUIInput;
		cb::Callback2<void, std::string, Uint32>	mCb;

		void WindowClose( const UIEvent * Event );

		void CancelClick( const UIEvent * Event );

		void OKClick( const UIEvent * Event );
};

}}}

#endif
