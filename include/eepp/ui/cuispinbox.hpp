#ifndef EE_UICUISPINBOX_HPP
#define EE_UICUISPINBOX_HPP

#include <eepp/ui/cuitextinput.hpp>
#include <eepp/ui/cuipushbutton.hpp>

namespace EE { namespace UI {

class EE_API cUISpinBox : public cUIComplexControl {
	public:
		class CreateParams : public cUITextInput::CreateParams {
			public:
				inline CreateParams() :
					cUITextInput::CreateParams(),
					DefaultValue( 0.f ),
					AllowDotsInNumbers( false )
				{
					MaxLength = 24;
				}

				inline ~CreateParams() {}

				eeFloat DefaultValue;
				bool AllowDotsInNumbers;
		};

		cUISpinBox( const cUISpinBox::CreateParams& Params );

		virtual ~cUISpinBox();

		virtual Uint32 Type() const;

		virtual bool IsType( const Uint32& type ) const;

		virtual void SetTheme( cUITheme * Theme );

		virtual void Padding( const eeRecti& padding );

		const eeRecti& Padding() const;

		virtual void ClickStep( const eeFloat& step );

		const eeFloat& ClickStep() const;

		virtual Uint32 OnMessage( const cUIMessage * Msg );

		void AddValue( const eeFloat& value );

		virtual void MinValue( const eeFloat& MinVal );

		const eeFloat& MinValue() const;

		virtual void MaxValue( const eeFloat& MaxVal );

		const eeFloat& MaxValue() const;

		virtual void Value( const eeFloat& Val );

		const eeFloat& Value() const;

		virtual void Update();

		cUIControlAnim * ButtonPushUp() const;

		cUIControlAnim * ButtonPushDown() const;

		cUITextInput * TextInput() const;
	protected:
		cUITextInput * 		mInput;
		cUIControlAnim * 	mPushUp;
		cUIControlAnim * 	mPushDown;
		eeFloat				mMinValue;
		eeFloat				mMaxValue;
		eeFloat				mValue;
		eeFloat				mClickStep;

		void AdjustChilds();

		void InternalValue( const eeFloat& Val, const bool& Force = false );
		
		virtual void OnAlphaChange();
};

}}

#endif
