#ifndef EE_UIHELPER_HPP
#define EE_UIHELPER_HPP

#include <eepp/ui/base.hpp>

namespace EE { namespace UI {

enum UI_CONTROL_FLAGS_VALUES {
	UI_CTRL_FLAG_CLOSE								= (1<<0),
	UI_CTRL_FLAG_CLOSE_FO							= (1<<1),
	UI_CTRL_FLAG_ANIM								= (1<<2),
	UI_CTRL_FLAG_MOUSEOVER							= (1<<3),
	UI_CTRL_FLAG_HAS_FOCUS							= (1<<4),
	UI_CTRL_FLAG_SELECTED							= (1<<5),
	UI_CTRL_FLAG_DISABLE_FADE_OUT					= (1<<6),
	UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD				= (1<<7),
	UI_CTRL_FLAG_DRAGGING							= (1<<8),
	UI_CTRL_FLAG_DRAGABLE							= (1<<9),
	UI_CTRL_FLAG_COMPLEX							= (1<<10),
	UI_CTRL_FLAG_SKIN_OWNER							= (1<<11),
	UI_CTRL_FLAG_TOUCH_DRAGGING						= (1<<12),
	UI_CTRL_FLAG_DISABLED_BY_MODAL_WINDOW			= (1<<13),
	UI_CTRL_FLAG_SELECTING							= (1<<14),
	UI_CTRL_FLAG_ROTATED							= (1<<15),
	UI_CTRL_FLAG_SCALED								= (1<<16),
	UI_CTRL_FLAG_FREE_USE							= (1<<31)
};

#define UI_HALIGN_LEFT		FONT_DRAW_LEFT
#define UI_HALIGN_MASK		FONT_DRAW_HALIGN_MASK
#define UI_VALIGN_TOP		FONT_DRAW_TOP
#define UI_VALIGN_MASK		FONT_DRAW_VALIGN_MASK

inline Uint32 HAlignGet( Uint32 Flags ) {
	return Flags & UI_HALIGN_MASK;
}

inline Uint32 VAlignGet( Uint32 Flags ) {
	return Flags & UI_VALIGN_MASK;
}

enum UI_FLAGS {
	UI_HALIGN_RIGHT					= FONT_DRAW_RIGHT,
	UI_HALIGN_CENTER				= FONT_DRAW_CENTER,
	UI_VALIGN_BOTTOM				= FONT_DRAW_BOTTOM,
	UI_VALIGN_CENTER				= FONT_DRAW_MIDDLE,
	UI_DRAW_SHADOW					= FONT_DRAW_SHADOW,
	UI_TEXT_DRAW_VERTICAL			= FONT_DRAW_VERTICAL,
	UI_AUTO_SIZE					= (1 << 7),
	UI_FILL_BACKGROUND				= (1 << 9),
	UI_BORDER						= (1 << 10),
	UI_TAB_STOP						= (1 << 11),
	UI_FIT_TO_CONTROL				= (1 << 12),
	UI_CLIP_ENABLE					= (1 << 13),
	UI_WORD_WRAP					= (1 << 14),
	UI_MULTI_SELECT					= (1 << 15),
	UI_AUTO_PADDING					= (1 << 16),
	UI_DRAG_ENABLE					= (1 << 17),
	UI_REPORT_SIZE_CHANGE_TO_CHILDS = (1 << 18),
	UI_ANCHOR_TOP					= (1 << 19),
	UI_ANCHOR_BOTTOM				= (1 << 20),
	UI_ANCHOR_LEFT					= (1 << 21),
	UI_ANCHOR_RIGHT					= (1 << 22),
	UI_AUTO_FIT						= (1 << 23),
	UI_TOUCH_DRAG_ENABLED			= (1 << 24),
	UI_TEXT_SELECTION_ENABLED		= (1 << 25)
};

enum UI_CONTROL_TYPES {
	UI_TYPE_CONTROL	= 0,
	UI_TYPE_CONTROL_DRAGABLE,
	UI_TYPE_CONTROL_ANIM,
	UI_TYPE_WIDGET,
	UI_TYPE_IMAGE,
	UI_TYPE_SPRITE,
	UI_TYPE_TEXTVIEW,
	UI_TYPE_TEXTINPUT,
	UI_TYPE_PUSHBUTTON,
	UI_TYPE_CHECKBOX,
	UI_TYPE_RADIOBUTTON,
	UI_TYPE_SLIDER,
	UI_TYPE_SPINBOX,
	UI_TYPE_SCROLLBAR,
	UI_TYPE_PROGRESSBAR,
	UI_TYPE_LISTBOX,
	UI_TYPE_LISTBOXITEM,
	UI_TYPE_DROPDOWNLIST,
	UI_TYPE_MENU_SEPARATOR,
	UI_TYPE_COMBOBOX,
	UI_TYPE_MENU,
	UI_TYPE_MENUITEM,
	UI_TYPE_MENUCHECKBOX,
	UI_TYPE_MENUSUBMENU,
	UI_TYPE_TEXTEDIT,
	UI_TYPE_TOOLTIP,
	UI_TYPE_TABLE,
	UI_TYPE_WINDOW,
	UI_TYPE_WINMENU,
	UI_TYPE_SELECTBUTTON,
	UI_TYPE_POPUPMENU,
	UI_TYPE_COMMONDIALOG,
	UI_TYPE_TAB,
	UI_TYPE_TABWIDGET,
	UI_TYPE_LINEAR_LAYOUT,
	UI_TYPE_USER = 100
};

enum UI_SCROLLBAR_MODE {
	UI_SCROLLBAR_AUTO,
	UI_SCROLLBAR_ALWAYS_ON,
	UI_SCROLLBAR_ALWAYS_OFF
};

enum UI_MANAGER_FLAGS {
	UI_MANAGER_HIGHLIGHT_FOCUS	= ( 1 << 0 ),
	UI_MANAGER_HIGHLIGHT_OVER	= ( 1 << 1 ),
	UI_MANAGER_DRAW_DEBUG_DATA	= ( 1 << 2 ),
	UI_MANAGER_DRAW_BOXES		= ( 1 << 3 )
};

enum UI_WINDOW_FLAGS {
	UI_WIN_NO_BORDER					= ( 1 << 0 ),
	UI_WIN_CLOSE_BUTTON					= ( 1 << 1 ),
	UI_WIN_MINIMIZE_BUTTON				= ( 1 << 2 ),
	UI_WIN_MAXIMIZE_BUTTON				= ( 1 << 3 ),
	UI_WIN_USE_DEFAULT_BUTTONS_ACTIONS	= ( 1 << 4 ),
	UI_WIN_RESIZEABLE					= ( 1 << 5 ),
	UI_WIN_DRAGABLE_CONTAINER			= ( 1 << 6 ),
	UI_WIN_SHARE_ALPHA_WITH_CHILDS		= ( 1 << 7 ),
	UI_WIN_MODAL						= ( 1 << 8 ),
	UI_WIN_DRAW_SHADOW					= ( 1 << 9 )
};

enum UI_COMMON_DIALOG_FLAGS {
	CDL_FLAG_SAVE_DIALOG			= ( 1 << 0 ),
	CDL_FLAG_FOLDERS_FISRT			= ( 1 << 1 ),
	CDL_FLAG_SORT_ALPHABETICALLY	= ( 1 << 2 ),
	CDL_FLAG_ALLOW_FOLDER_SELECT	= ( 1 << 3 )
};

enum UI_MSGBOX_TYPE {
	MSGBOX_OKCANCEL,
	MSGBOX_YESNO,
	MSGBOX_RETRYCANCEL,
	MSGBOX_OK
};

enum UI_ORIENTATION {
	UI_VERTICAL,
	UI_HORIZONTAL
};

enum LayoutSizeRules {
	FIXED,
	MATCH_PARENT,
	WRAP_CONTENT
};

enum LayoutPositionRules {
	NONE,
	LEFT_OF,
	RIGHT_OF,
	TOP_OF,
	BOTTOM_OF
};

static const Uint32 UI_CONTROL_DEFAULT_ALIGN = UI_HALIGN_LEFT | UI_VALIGN_CENTER;

static const Uint32 UI_CONTROL_ALIGN_CENTER = UI_HALIGN_CENTER | UI_VALIGN_CENTER;

static const Uint32 UI_CONTROL_DEFAULT_ANCHOR = UI_ANCHOR_LEFT | UI_ANCHOR_TOP;

static const Uint32 UI_CONTROL_DEFAULT_FLAGS = UI_CONTROL_DEFAULT_ANCHOR | UI_CONTROL_DEFAULT_ALIGN;

static const Uint32 UI_CONTROL_DEFAULT_FLAGS_CENTERED = UI_ANCHOR_LEFT | UI_ANCHOR_TOP | UI_HALIGN_CENTER | UI_VALIGN_CENTER;

static const Uint32 UI_WIN_DEFAULT_FLAGS = UI_WIN_CLOSE_BUTTON | UI_WIN_USE_DEFAULT_BUTTONS_ACTIONS | UI_WIN_RESIZEABLE | UI_WIN_SHARE_ALPHA_WITH_CHILDS;

static const Uint32 UI_CDL_DEFAULT_FLAGS = CDL_FLAG_FOLDERS_FISRT | CDL_FLAG_SORT_ALPHABETICALLY;

class UIWidget;
class UIHelper {
	public:
		static UIWidget * createUIWidgetFromName( std::string name );
};

}}

#endif
