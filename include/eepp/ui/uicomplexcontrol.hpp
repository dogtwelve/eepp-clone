#ifndef EE_UICUICOMPLEXCONTROL_HPP
#define EE_UICUICOMPLEXCONTROL_HPP

#include <eepp/ui/uicontrolanim.hpp>
#include <eepp/ui/uitooltip.hpp>

namespace EE { namespace UI {

class EE_API UIComplexControl : public UIControlAnim {
	public:
		class CreateParams : public UIControlAnim::CreateParams {
			public:
				inline CreateParams() :
					UIControlAnim::CreateParams(),
					MinControlSize( 0, 0 )
				{
				}

				inline CreateParams(
					UIControl * parentCtrl,
					const Vector2i& pos = Vector2i( 0, 0 ),
					const Sizei& size = Sizei( -1, -1 ),
					const Uint32& flags = UI_CONTROL_DEFAULT_FLAGS,
					const EE_BLEND_MODE& blend = ALPHA_NORMAL,
					const UIBackground& Back = UIBackground(),
					const UIBorder& Bord = UIBorder(),
					const Sizei& MinCtrlSize = Sizei(0,0)
				) :
					UIControlAnim::CreateParams( parentCtrl, pos, size, flags, blend, Back, Bord ),
					MinControlSize( MinCtrlSize )
				{
				}

				inline ~CreateParams() {}

				String	TooltipText;
				Sizei	MinControlSize;
		};

		UIComplexControl( const UIComplexControl::CreateParams& Params );

		virtual ~UIComplexControl();

		virtual Uint32 Type() const;

		virtual bool IsType( const Uint32& type ) const;

		virtual void Update();

		virtual void Size( const Sizei &Size );

		void Size( const Int32& Width, const Int32& Height );

		const Sizei& Size();

		UITooltip * Tooltip();

		void TooltipRemove();

		void TooltipText( const String& Text );

		String TooltipText();

		void UpdateAnchorsDistances();
	protected:
		UITooltip *	mTooltip;
		Sizei			mMinControlSize;
		Recti			mDistToBorder;

		void CreateTooltip();

		virtual void OnParentSizeChange( const Vector2i& SizeChange );
};

}}

#endif
