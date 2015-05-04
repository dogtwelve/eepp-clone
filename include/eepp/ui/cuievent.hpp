#ifndef EE_UICUIEVENT_HPP
#define EE_UICUIEVENT_HPP

#include <eepp/ui/base.hpp>

namespace EE { namespace UI {

class cUIControl;

class EE_API cUIEvent {
	public:
		enum UIEvent {
			EventKeyDown = 0,
			EventKeyUp,
			EventMouseMove,
			EventMouseDown,
			EventMouseClick,
			EventMouseDoubleClick,
			EventMouseUp,
			EventMouseEnter,
			EventMouseExit,
			EventOnFocus,
			EventOnFocusLoss,
			EventOnVisibleChange,
			EventOnEnabledChange,
			EventOnPosChange,
			EventOnSizeChange,
			EventOnAngleChange,
			EventOnScaleChange,
			EventOnAlphaChange,
			EventOnTextChanged,
			EventOnFontChanged,
			EventOnPressEnter,
			EventOnValueChange,
			EventOnComplexControlFocusLoss,
			EventOnItemClicked,
			EventOnHideByClick,
			EventOnItemKeyDown,
			EventOnItemKeyUp,
			EventOnItemSelected,
			EventOnCursorPosChange,
			EventOnParentSizeChange,
			EventOnWindowClose,
			EventOnWindowCloseClick,
			EventOnWindowMaximizeClick,
			EventOnWindowMinimizeClick,
			EventOpenFile,
			EventSaveFile,
			EventOnControlClear,
			EventMsgBoxConfirmClick,
			EventOnTabSelected,
			EventOnClose, // Warning: Only some controls will report this event.
			EventUser,
			EventForceDWord = eeINDEX_NOT_FOUND
		};

		cUIEvent( cUIControl * Ctrl, const Uint32& EventType = EventForceDWord );

		~cUIEvent();

		cUIControl * Ctrl() const;

		const Uint32& EventType() const;
	protected:
		cUIControl	* 	mCtrl;
		Uint32 			mEventType;
};

}}

#endif
