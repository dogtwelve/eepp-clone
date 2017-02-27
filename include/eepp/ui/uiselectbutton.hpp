#ifndef EE_UICUISELECTBUTTON_HPP
#define EE_UICUISELECTBUTTON_HPP

#include <eepp/ui/uipushbutton.hpp>

namespace EE { namespace UI {

class EE_API UISelectButton : public UIPushButton {
	public:
		UISelectButton( const UIPushButton::CreateParams& Params );

		UISelectButton();

		virtual ~UISelectButton();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type ) const;

		virtual bool selected() const;

		virtual void unselect();

		virtual void select();

		ColorA getFontSelectedColor() const;

		void setFontSelectedColor(const ColorA & fontSelectedColor);
	protected:
		virtual void onStateChange();

		ColorA mFontSelectedColor;
};

}}

#endif
