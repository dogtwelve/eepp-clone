#ifndef EE_UICUITEXTEDIT_HPP
#define EE_UICUITEXTEDIT_HPP

#include <eepp/ui/uicontrolanim.hpp>
#include <eepp/ui/uitextinput.hpp>
#include <eepp/ui/uiscrollbar.hpp>

namespace EE { namespace UI {

class EE_API UITextEdit : public UIComplexControl {
	public:			
		class CreateParams : public UITextBox::CreateParams {
			public:
				inline CreateParams() : UITextBox::CreateParams(),
					HScrollBar( UI_SCROLLBAR_AUTO ),
					VScrollBar( UI_SCROLLBAR_AUTO ),
					WordWrap( true )
				{
				}

				inline ~CreateParams() {}

				UI_SCROLLBAR_MODE	HScrollBar;
				UI_SCROLLBAR_MODE	VScrollBar;
				bool				WordWrap;
		};

		UITextEdit( UITextEdit::CreateParams& Params );

		virtual ~UITextEdit();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type ) const;

		virtual void setTheme( UITheme * Theme );

		const String& text() const;

		void text( const String& Txt );

		UITextInput * getTextInput() const;

		UIScrollBar * getHScrollBar() const;

		UIScrollBar * getVScrollBar() const;

		virtual void update();

		void allowEditing( const bool& allow );

		const bool& allowEditing() const;
	protected:
		UITextInput *		mTextInput;
		UIScrollBar *		mHScrollBar;
		UIScrollBar *		mVScrollBar;
		UI_SCROLLBAR_MODE	mHScrollBarMode;
		UI_SCROLLBAR_MODE	mVScrollBarMode;
		Recti				mPadding;
		String				mText;
		bool				mSkipValueChange;

		virtual void onSizeChange();

		virtual void onAlphaChange();

		virtual void onParentSizeChange( const Vector2i& SizeChange );

		void onVScrollValueChange( const UIEvent * Event );

		void onHScrollValueChange( const UIEvent * Event );

		void onInputSizeChange( const UIEvent * Event = NULL );

		void onCursorPosChange( const UIEvent * Event );

		void autoPadding();

		void scrollbarsSet();

		void fixScroll();

		void fixScrollToCursor();

		void shrinkText( const Uint32& Width );
};

}}

#endif
