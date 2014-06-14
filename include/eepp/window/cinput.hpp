#ifndef EE_WINDOWCINPUT_H
#define EE_WINDOWCINPUT_H

#include <eepp/window/base.hpp>
#include <eepp/window/cwindow.hpp>
#include <eepp/window/inputhelper.hpp>
#include <eepp/window/inputevent.hpp>
#include <eepp/window/cjoystickmanager.hpp>
#include <eepp/window/cinputfinger.hpp>
#include <list>

namespace EE { namespace Window {

class View;

/** @brief The basic input class. For mouse and keyboard. */
class EE_API Input {
	public:
		typedef cb::Callback1<void, InputEvent*>	InputCallback;
		
		virtual ~Input();

		/** Update the Input */
		virtual void Update() = 0;

		/** @return If the mouse and keyboard are grabed. */
		virtual bool GrabInput() = 0;

		/** Grab or Ungrab the mouse and keyboard. */
		virtual void GrabInput( const bool& Grab ) = 0;

		/** Inject the mouse position given */
		virtual void InjectMousePos( const Uint16& x, const Uint16& y ) = 0;

		/** @return If keyboard key was released */
		bool IsKeyUp( const EE_KEY& Key );

		/** @return If keyboard key it's pressed */
		bool IsKeyDown( const EE_KEY& Key );

		/** Inject the key state of a key as KEY UP or RELEASE */
		void InjectKeyUp( const EE_KEY& Key );

		/** Inject the key state of a key as KEY DOWN or PRESSED */
		void InjectKeyDown( const EE_KEY& Key );

		/** Inject the mouse position given */
		void InjectMousePos( const Vector2i& Pos );

		/** Inject the mouse button as pressed */
		void InjectButtonPress( const Uint32& Button );

		/** Inject the mouse button as released */
		void InjectButtonRelease( const Uint32& Button );

		/** @return If the Control Key is pressed */
		bool ControlPressed() const;

		/** @return If the Shift Key is pressed */
		bool ShiftPressed() const;

		/** @return If the Alt Key is pressed */
		bool AltPressed() const;

		/** @return If the Meta Key is pressed */
		bool MetaPressed() const;

		/** @return If mouse left button it's pressed */
		bool MouseLeftPressed() const;

		/** @return If mouse right button it's pressed */
		bool MouseRightPressed() const;

		/** @return If mouse middle button it's pressed */
		bool MouseMiddlePressed() const;

		/** @return If mouse left click was clicked */
		bool MouseLeftClick() const;

		/** @return If mouse right click was clicked */
		bool MouseRightClick() const;

		/** @return If mouse middle button (scroll button) was clicked. */
		bool MouseMiddleClick() const;

		/** @return If mouse left click was double clicked */
		bool MouseLeftDoubleClick() const;

		/** @return If mouse right click was double clicked */
		bool MouseRightDoubleClick() const;

		/** @return If mouse middle button (scroll button) was double clicked. */
		bool MouseMiddleDoubleClick() const;

		/** @return If mouse wheel up scrolled */
		bool MouseWheelUp() const;

		/** @return If mouse wheel down scrolled */
		bool MouseWheelDown() const;

		/** Push a new input callback.
		* @return The Callback Id
		*/
		Uint32 PushCallback( const InputCallback& cb );

		/** Pop the callback id indicated. */
		void PopCallback( const Uint32& CallbackId );

		/** @return The Mouse position vector */
		Vector2i GetMousePos() const;

		/** @return The position vector converted to float */
		Vector2f GetMousePosf();

		/** This will change the value of the mouse pos, will not REALLY move the mouse ( for that is InjectMousePos ). */
		void SetMousePos( const Vector2i& Pos );

		/** @return The mouse position over the current view */
		Vector2i GetMousePosFromView( const View& View );

		/** @return The Mouse X axis position */
		Uint16 MouseX() const;

		/** @return The Mouse Y axis position */
		Uint16 MouseY() const;

		/** Set the mouse speed ( only affects grabed windows ) */
		void MouseSpeed( const Float& Speed );

		/** @return The Mouse Speed */
		const Float& MouseSpeed() const;

		/** @return The bitflags of the last pressed trigger (before the current state of press trigger) */
		const Uint32& LastPressTrigger() const;

		/** @return The current state as flags of the mouse press trigger
			@brief 	Triggers are used mostly for the UI components. They are simple to manage.
					The mouse flags are defined in keycodes.hpp
			For Example The usage is simple, to know if the left mouse click is pressed you need to check against the left mouse flag mask
					 if ( myInput->PressTrigger() & EE_BUTTON_LMASK ) ...
		*/
		const Uint32& PressTrigger() const;

		/** @return The current state as flags of the mouse release trigger */
		const Uint32& ReleaseTrigger() const;

		/** @return The current state as flags of the mouse click trigger */
		const Uint32& ClickTrigger() const;

		/** @return The current state as flags of the mouse double click trigger */
		const Uint32& DoubleClickTrigger() const;

		/** @return The double click interval in milliseconds ( default 500 ms ) */
		const Uint32& DoubleClickInterval() const;

		/** Set the double click interval in milliseconds */
		void DoubleClickInterval( const Uint32& Interval );

		/** Clean the keyboard and mouse states */
		void CleanStates();
		
		/** Send an input event to the window */
		void SendEvent( InputEvent * Event );
		
		/** @return The joystick manager */
		JoystickManager * GetJoystickManager() const;

		/** @return The maximun number of fingers */
		Uint32 GetFingerCount();

		/** @return The input finger from it's index */
		InputFinger * GetFingerIndex( const Uint32& Index );

		/** @return The Input Finder from it's id */
		InputFinger * GetFinger( const Int64& fingerId );

		/** @return A list of the input finders that are currently down */
		std::list<InputFinger *> GetFingersDown();

		/** @return A list of the input finders that were down in the last update */
		std::list<InputFinger *> GetFingersWasDown();
	protected:
		friend class cWindow;
		
		Input( Window::cWindow * window, JoystickManager * joystickmanager );
		
		virtual void Init() = 0;

		cWindow *	mWindow;
		JoystickManager * mJoystickManager;
		Uint8 mKeysDown	[EE_KEYS_SPACE];
		Uint8 mKeysUp	[EE_KEYS_SPACE];
		Uint32		mPressTrigger;
		Uint32		mReleaseTrigger;
		Uint32		mLastPressTrigger;
		Uint32		mClickTrigger;
		Uint32		mDoubleClickTrigger;
		Uint32		mInputMod;
		Uint32		mDoubleClickInterval; //!< Determine the double click inverval in milliseconds ( default 500 ms )
		Uint32		mLastButtonLeftClicked;
		Uint32		mLastButtonRightClicked;
		Uint32		mLastButtonMiddleClicked;
		Uint32		mLastButtonLeftClick;
		Uint32		mLastButtonRightClick;
		Uint32		mLastButtonMiddleClick;
		Uint32		mTClick;
		Vector2i	mMousePos;
		Uint32		mNumCallBacks;
		Float		mMouseSpeed;
		bool		mInputGrabed;
		InputFinger mFingers[ EE_MAX_FINGERS ];
		
		std::map<Uint32, InputCallback> mCallbacks;
		
		void ProcessEvent( InputEvent * Event );

		InputFinger * GetFingerId( const Int64& fingerId );

		void ResetFingerWasDown();
};

}}

#endif
 
