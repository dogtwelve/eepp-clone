#ifndef EE_UICUITEXTBOX_H
#define EE_UICUITEXTBOX_H

#include <eepp/ui/uicomplexcontrol.hpp>

namespace EE { namespace UI {

class EE_API UITextBox : public UIComplexControl {
	public:
		class CreateParams : public UIComplexControl::CreateParams {
			public:
				inline CreateParams() :
					UIComplexControl::CreateParams(),
					Font( NULL ),
					FontColor( 0, 0, 0, 255 ),
					FontShadowColor( 255, 255, 255, 150 ),
					FontSelectionBackColor( 150, 150, 150, 150 )
				{
					UITheme * Theme = UIThemeManager::instance()->getDefaultTheme();

					if ( NULL != Theme ) {
						Font			= Theme->getFont();
						FontColor		= Theme->getFontColor();
						FontShadowColor	= Theme->getFontShadowColor();
					}

					if ( NULL == Font )
						Font = UIThemeManager::instance()->getDefaultFont();
				}

				inline ~CreateParams() {}

				Graphics::Font * 	Font;
				ColorA				FontColor;
				ColorA				FontShadowColor;
				ColorA				FontSelectionBackColor;
		};

		UITextBox( const UITextBox::CreateParams& Params );

		UITextBox();

		virtual ~UITextBox();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type ) const;

		virtual void draw();

		virtual void setAlpha( const Float& alpha );

		Graphics::Font * getFont() const;

		void setFont( Graphics::Font * font );

		virtual const String& getText();

		virtual void setText( const String& text );

		const ColorA& getFontColor() const;

		void setFontColor( const ColorA& color );

		const ColorA& getFontShadowColor() const;

		void setFontShadowColor( const ColorA& color );

		const ColorA& getSelectionBackColor() const;

		void setSelectionBackColor( const ColorA& color );

		virtual void setPadding( const Recti& padding );

		virtual void setPixelsPadding( const Recti& padding );

		const Recti& getPadding() const;

		virtual void setTheme( UITheme * Theme );

		TextCache * getTextCache();

		Float getTextWidth();

		Float getTextHeight();

		const int& getNumLines() const;

		const Vector2f& alignOffset() const;

		virtual void shrinkText( const Uint32& MaxWidth );

		bool isTextSelectionEnabled() const;
	protected:
		TextCache *	mTextCache;
		String			mString;
		ColorA 		mFontColor;
		ColorA 		mFontShadowColor;
		ColorA		mFontSelectionBackColor;
		Vector2f 		mAlignOffset;
		Recti			mPadding;
		Recti			mRealPadding;
		Int32			mSelCurInit;
		Int32			mSelCurEnd;

		virtual void drawSelection();

		virtual void onSizeChange();

		virtual void autoShrink();

		virtual void autoSize();

		virtual void autoAlign();

		virtual void onTextChanged();

		virtual void onFontChanged();

		virtual Uint32 onFocusLoss();

		virtual Uint32 onMouseDoubleClick( const Vector2i& position, const Uint32 flags );

		virtual Uint32 onMouseClick( const Vector2i& position, const Uint32 flags );

		virtual Uint32 onMouseDown( const Vector2i& position, const Uint32 flags );

		virtual void selCurInit( const Int32& init );

		virtual void selCurEnd( const Int32& end );

		virtual Int32 selCurInit();

		virtual Int32 selCurEnd();

};

}}

#endif
