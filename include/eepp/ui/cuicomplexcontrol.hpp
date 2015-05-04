#ifndef EE_UICUICOMPLEXCONTROL_HPP
#define EE_UICUICOMPLEXCONTROL_HPP

#include <eepp/ui/cuicontrolanim.hpp>
#include <eepp/ui/cuitooltip.hpp>

namespace EE { namespace UI {

class EE_API cUIComplexControl : public cUIControlAnim {
	public:
		class CreateParams : public cUIControlAnim::CreateParams {
			public:
				inline CreateParams() :
					cUIControlAnim::CreateParams(),
					MinControlSize( 0, 0 )
				{
				}

				inline CreateParams(
					cUIControl * parentCtrl,
					const eeVector2i& pos = eeVector2i( 0, 0 ),
					const eeSize& size = eeSize( -1, -1 ),
					const Uint32& flags = UI_CONTROL_DEFAULT_FLAGS,
					const EE_BLEND_MODE& blend = ALPHA_NORMAL,
					const cUIBackground& Back = cUIBackground(),
					const cUIBorder& Bord = cUIBorder(),
					const eeSize& MinCtrlSize = eeSize(0,0)
				) :
					cUIControlAnim::CreateParams( parentCtrl, pos, size, flags, blend, Back, Bord ),
					MinControlSize( MinCtrlSize )
				{
				}

				inline ~CreateParams() {}

				String	TooltipText;
				eeSize	MinControlSize;
		};

		cUIComplexControl( const cUIComplexControl::CreateParams& Params );

		virtual ~cUIComplexControl();

		virtual Uint32 Type() const;

		virtual bool IsType( const Uint32& type ) const;

		virtual void Update();

		virtual void Size( const eeSize &Size );

		void Size( const Int32& Width, const Int32& Height );

		const eeSize& Size();

		cUITooltip * Tooltip();

		void TooltipRemove();

		void TooltipText( const String& Text );

		String TooltipText();

		void UpdateAnchorsDistances();
	protected:
		cUITooltip *	mTooltip;
		eeSize			mMinControlSize;
		eeRecti			mDistToBorder;

		void CreateTooltip();

		virtual void OnParentSizeChange( const eeVector2i& SizeChange );
};

}}

#endif
