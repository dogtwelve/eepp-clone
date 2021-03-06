#ifndef EE_UICUIDEFAULTTHEME_HPP
#define EE_UICUIDEFAULTTHEME_HPP

#include <eepp/ui/uitheme.hpp>

namespace EE { namespace UI {

class EE_API UIDefaultTheme : public UITheme {
	public:
		UIDefaultTheme( const std::string& Name, const std::string& Abbr, Graphics::Font * DefaultFont = NULL );

		virtual UIPopUpMenu * CreatePopUpMenu( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS | UI_AUTO_SIZE | UI_AUTO_PADDING, Uint32 RowHeight = 0, Recti PaddingContainer = Recti(), Uint32 MinWidth = 100, Uint32 MinSpaceForIcons = 16, Uint32 MinRightMargin = 8 );

		virtual UIProgressBar * CreateProgressBar( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS, bool DisplayPercent = true, bool VerticalExpand = false, Vector2f MovementSpeed = Vector2f( -64, 0 ), Rectf FillerMargin = Rectf() );

		virtual UIWinMenu * CreateWinMenu( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS, Uint32 MarginBetweenButtons = 0, Uint32 ButtonMargin = 12, Uint32 MenuHeight = 0, Uint32 FirstButtonMargin = 1 );

		virtual UIWindow * CreateWindow( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS_CENTERED, Uint32 WinFlags = UI_WIN_DEFAULT_FLAGS, Sizei MinWindowSize = Sizei(0,0), Uint8 BaseAlpha = 255 );

		virtual UICommonDialog * CreateCommonDialog( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS_CENTERED, Uint32 WinFlags = UI_WIN_DEFAULT_FLAGS | UI_WIN_MAXIMIZE_BUTTON, Sizei MinWindowSize = Sizei(0,0), Uint8 BaseAlpha = 255, Uint32 CDLFlags = UI_CDL_DEFAULT_FLAGS, std::string DefaultFilePattern = "*", std::string DefaultDirectory = Sys::GetProcessPath() );

		virtual UIComboBox * CreateComboBox( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS | UI_CLIP_ENABLE | UI_AUTO_PADDING | UI_AUTO_SIZE, Uint32 MinNumVisibleItems = 6, bool PopUpToMainControl = false, UIListBox * ListBox = NULL );

		virtual UIDropDownList * CreateDropDownList( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS | UI_CLIP_ENABLE | UI_AUTO_PADDING | UI_AUTO_SIZE, Uint32 MinNumVisibleItems = 6, bool PopUpToMainControl = false, UIListBox * ListBox = NULL );

		virtual UIMessageBox * CreateMessageBox( UI_MSGBOX_TYPE Type = MSGBOX_OKCANCEL, const String& Message = String(), Uint32 WinFlags = UI_WIN_DEFAULT_FLAGS | UI_WIN_MODAL, UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS_CENTERED, Sizei MinWindowSize = Sizei(0,0), Uint8 BaseAlpha = 255 );

		virtual UITabWidget * CreateTabWidget( UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_HALIGN_CENTER | UI_VALIGN_BOTTOM | UI_CONTROL_DEFAULT_ANCHOR, const bool& TabsClosable = false, const bool& SpecialBorderTabs = false , const Int32& TabSeparation = 0, const Uint32& MaxTextLength = 30, const Uint32& TabWidgetHeight = 0, const Uint32& TabTextAlign = UI_HALIGN_CENTER | UI_VALIGN_CENTER, const Uint32& MinTabWidth = 32, const Uint32& MaxTabWidth = 210 );

		virtual UITooltip * CreateTooltip( UIControl * TooltipOf, UIControl * Parent = NULL, const Sizei& Size = Sizei(), const Vector2i& Pos = Vector2i(), const Uint32& Flags = UI_CONTROL_DEFAULT_FLAGS_CENTERED | UI_AUTO_PADDING | UI_AUTO_SIZE );
};

}}

#endif
