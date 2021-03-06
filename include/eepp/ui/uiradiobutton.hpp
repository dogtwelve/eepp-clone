#ifndef EE_UICUIRADIOBUTTON_H
#define EE_UICUIRADIOBUTTON_H

#include <eepp/ui/uitextbox.hpp>
#include <eepp/ui/uipushbutton.hpp>

namespace EE { namespace UI {

class EE_API UIRadioButton : public UITextBox {
	public:
		UIRadioButton( const UITextBox::CreateParams& Params );

		virtual ~UIRadioButton();

		virtual Uint32 Type() const;

		virtual bool IsType( const Uint32& type ) const;

		virtual void SetTheme( UITheme * Theme );

		const bool& IsActive() const;

		void Active( const bool& active );

		const bool& Active() const;

		virtual void Padding( const Recti& padding );

		UIControlAnim * ActiveButton() const;

		UIControlAnim * InactiveButton() const;
	protected:
		UIControlAnim *	mActiveButton;
		UIControlAnim *	mInactiveButton;
		bool				mActive;
		Uint32				mLastTick;

		virtual void OnSizeChange();

		void SwitchState();

		void AutoActivate();

		bool CheckActives();

		virtual void OnAlphaChange();

		virtual Uint32 OnKeyDown( const UIEventKey& Event );

		virtual Uint32 OnMessage( const UIMessage * Msg );

		virtual void AutoSize();
};

}}

#endif


