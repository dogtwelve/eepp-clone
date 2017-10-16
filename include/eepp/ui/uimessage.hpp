#ifndef EE_UICUIMESSAGE_HPP
#define EE_UICUIMESSAGE_HPP

#include <eepp/ui/base.hpp>

namespace EE { namespace UI {

class UIControl;

class EE_API UIMessage {
	public:
		enum Message
		{
			Click = 0,
			DoubleClick,
			MouseEnter,
			MouseExit,
			MouseDown,
			MouseUp,
			WindowResize,
			Focus,
			FocusLoss,
			CellClicked,
			Selected,
			DragStart,
			DragStop,
			LayoutAttributeChange,
			UserMessage,
			NoMessage = eeINDEX_NOT_FOUND
		};

		UIMessage( UIControl * Ctrl, const Uint32& getMsg, const Uint32& getFlags = NoMessage );

		~UIMessage();

		UIControl * getSender() const;

		const Uint32& getMsg() const;

		const Uint32& getFlags() const;
	private:
		UIControl *	mCtrl;
		Uint32			mMsg;
		Uint32			mFlags;
};

}}

#endif
