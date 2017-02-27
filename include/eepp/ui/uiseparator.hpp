#ifndef EE_UICUISEPARATOR
#define EE_UICUISEPARATOR

#include <eepp/ui/uicontrolanim.hpp>

namespace EE { namespace UI {

class EE_API UISeparator : public UIControlAnim {
	public:
		UISeparator( UIControlAnim::CreateParams Params );

		UISeparator();
		
		virtual ~UISeparator();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type ) const;

		virtual void setTheme( UITheme * Theme );
	protected:
};

}}

#endif

