#include <eepp/window/input.hpp>
#include <eepp/window/view.hpp>

namespace EE { namespace Window {

Input::Input( EE::Window::Window * window, JoystickManager * joystickmanager ) :
	mWindow( window ),
	mJoystickManager( joystickmanager ),
	mPressTrigger(0), mReleaseTrigger(0), mLastPressTrigger(0), mClickTrigger(0), mDoubleClickTrigger(0), mInputMod(0),
	mDoubleClickInterval(400),
	mLastButtonLeftClicked(0), 		mLastButtonRightClicked(0), 	mLastButtonMiddleClicked(0),
	mLastButtonLeftClick(0), 		mLastButtonRightClick(0), 		mLastButtonMiddleClick(0),
	mTClick(0), mNumCallBacks(0),
	mMouseSpeed(1.0f),
	mInputGrabed( false )
{
	memset( mKeysDown	, 0, EE_KEYS_SPACE );
	memset( mKeysUp		, 0, EE_KEYS_SPACE );
}

Input::~Input() {
	eeSAFE_DELETE( mJoystickManager );
}

void Input::cleanStates() {
	memset( mKeysUp, 0, EE_KEYS_SPACE );

	mReleaseTrigger 	= 0;
	mLastPressTrigger 	= mPressTrigger;
	mClickTrigger 		= 0;
	mDoubleClickTrigger = 0;

	resetFingerWasDown();
}

void Input::sendEvent( InputEvent * Event ) {
	for ( std::map<Uint32, InputCallback>::iterator i = mCallbacks.begin(); i != mCallbacks.end(); i++ ) {
		i->second( Event );
	}
}

void Input::processEvent( InputEvent * Event ) {
	switch( Event->Type ) {
		case InputEvent::KeyDown:
		{
			if ( Event->key.keysym.sym > EE_KEYS_NUM )
				break;

			if ( Event->key.keysym.mod != eeINDEX_NOT_FOUND )
				mInputMod = Event->key.keysym.mod;

			BitOp::writeBitKey( &mKeysDown	[ Event->key.keysym.sym / 8 ], Event->key.keysym.sym % 8, 1 );
			break;
		}
		case InputEvent::KeyUp:
		{
			if ( Event->key.keysym.sym > EE_KEYS_NUM )
				break;

			BitOp::writeBitKey( &mKeysDown	[ Event->key.keysym.sym / 8 ], Event->key.keysym.sym % 8, 0 );
			BitOp::writeBitKey( &mKeysUp	[ Event->key.keysym.sym / 8 ], Event->key.keysym.sym % 8, 1 );
			break;
		}
		case InputEvent::MouseMotion:
		{
			if ( !mInputGrabed ) {
				mMousePos.x = Event->motion.x;
				mMousePos.y = Event->motion.y;
			} else {
				mMousePos.x += static_cast<Int32>( (Float)Event->motion.xrel * mMouseSpeed );
				mMousePos.y += static_cast<Int32>( (Float)Event->motion.yrel * mMouseSpeed );
			}

			if ( mMousePos.x >= (int)mWindow->getWidth() ) {
				mMousePos.x = mWindow->getWidth();
			} else if ( mMousePos.x < 0 ) {
				mMousePos.x = 0;
			}

			if ( mMousePos.y >= (int)mWindow->getHeight() ) {
				mMousePos.y = mWindow->getHeight();
			} else if ( mMousePos.y < 0 ) {
				mMousePos.y = 0;
			}

			break;
		}
		case InputEvent::MouseButtonDown:
		{
			mPressTrigger		|= EE_BUTTON_MASK( Event->button.button );
			break;
		}
		case InputEvent::MouseButtonUp:
		{
			mPressTrigger		&= ~EE_BUTTON_MASK( Event->button.button );
			mReleaseTrigger		|= EE_BUTTON_MASK( Event->button.button );
			mClickTrigger		|= EE_BUTTON_MASK( Event->button.button );

			// I know this is ugly, but i'm too lazy to fix it, it works...
			if ( Event->button.button == EE_BUTTON_LEFT ) {
				mLastButtonLeftClicked		= mLastButtonLeftClick;
				mLastButtonLeftClick		= Sys::getTicks();

				mTClick = mLastButtonLeftClick - mLastButtonLeftClicked;

				if ( mTClick < mDoubleClickInterval && mTClick > 0 ) {
					mDoubleClickTrigger			|= EE_BUTTON_MASK(EE_BUTTON_LEFT);
					mLastButtonLeftClick		= 0;
					mLastButtonLeftClicked		= 0;
				}
			} else if ( Event->button.button == EE_BUTTON_RIGHT ) {
				mLastButtonRightClicked		= mLastButtonRightClick;
				mLastButtonRightClick		= Sys::getTicks();

				mTClick = mLastButtonRightClick - mLastButtonRightClicked;

				if ( mTClick < mDoubleClickInterval && mTClick > 0 ) {
					mDoubleClickTrigger			|= EE_BUTTON_MASK(EE_BUTTON_RIGHT);
					mLastButtonRightClick		= 0;
					mLastButtonRightClicked		= 0;
				}
			} else if( Event->button.button == EE_BUTTON_MIDDLE ) {
				mLastButtonMiddleClicked	= mLastButtonMiddleClick;
				mLastButtonMiddleClick		= Sys::getTicks();

				mTClick = mLastButtonMiddleClick - mLastButtonMiddleClicked;

				if ( mTClick < mDoubleClickInterval && mTClick > 0 ) {
					mDoubleClickTrigger			|= EE_BUTTON_MASK(EE_BUTTON_MIDDLE);
					mLastButtonMiddleClick		= 0;
					mLastButtonMiddleClicked	= 0;
				}
			}

			break;
		}
		case InputEvent::FingerDown:
		{
			InputFinger * Finger = getFingerId( Event->finger.fingerId );

			Finger->writeLast();
			Finger->x			= (Uint16)( Event->finger.x * (Float)mWindow->getWidth() );
			Finger->y			= (Uint16)( Event->finger.y * (Float)mWindow->getHeight() );
			Finger->pressure	= Event->finger.pressure;
			Finger->down		= true;
			Finger->xdelta		= Event->finger.dx;
			Finger->ydelta		= Event->finger.dy;

			break;
		}
		case InputEvent::FingerUp:
		{
			InputFinger * Finger = getFingerId( Event->finger.fingerId );

			Finger->writeLast();
			Finger->x			= (Uint16)( Event->finger.x * (Float)mWindow->getWidth() );
			Finger->y			= (Uint16)( Event->finger.y * (Float)mWindow->getHeight() );
			Finger->pressure	= Event->finger.pressure;
			Finger->down		= false;
			Finger->wasDown		= true;
			Finger->xdelta		= Event->finger.dx;
			Finger->ydelta		= Event->finger.dy;

			break;
		}
		case InputEvent::FingerMotion:
		{
			InputFinger * Finger = getFingerId( Event->finger.fingerId );

			Finger->writeLast();
			Finger->x			= (Uint16)( Event->finger.x * (Float)mWindow->getWidth() );
			Finger->y			= (Uint16)( Event->finger.y * (Float)mWindow->getHeight() );
			Finger->pressure	= Event->finger.pressure;
			Finger->down		= true;
			Finger->xdelta		= Event->finger.dx;
			Finger->ydelta		= Event->finger.dy;

			break;
		}
		case InputEvent::VideoResize:
		{
			mWindow->setSize( Event->resize.w, Event->resize.h, mWindow->isWindowed() );
			break;
		}
		case InputEvent::Quit:
		{
			mWindow->close();
			break;
		}
	}

	sendEvent( Event );
}

InputFinger * Input::getFingerId( const Int64 &fingerId ) {
	Uint32 i;

	for ( i = 0; i < EE_MAX_FINGERS; i++ ) {
		if ( mFingers[i].id == fingerId ) {
			return &mFingers[i];
		}
	}

	for ( i = 0; i < EE_MAX_FINGERS; i++ ) {
		if ( -1 == mFingers[i].id ) {
			mFingers[i].id = fingerId;

			return &mFingers[i];
		}
	}

	//! Find first unused
	for ( i = 0; i < EE_MAX_FINGERS; i++ ) {
		if ( !mFingers[i].down ) {
			mFingers[i].id = fingerId;
			return &mFingers[i];
		}
	}

	return NULL;
}

void Input::resetFingerWasDown() {
	for ( Uint32 i = 0; i < EE_MAX_FINGERS; i++ ) {
		mFingers[i].wasDown = false;
	}
}

bool Input::isKeyDown( const EE_KEY& Key ) {
	return 0 != BitOp::readBitKey( &mKeysDown[ Key / 8 ], Key % 8 );
}

bool Input::isKeyUp( const EE_KEY& Key ) {
	return 0 != BitOp::readBitKey( &mKeysUp[ Key / 8 ], Key % 8 );
}

void Input::injectKeyDown( const EE_KEY& Key ) {
	BitOp::writeBitKey( &mKeysDown	[ Key / 8 ], Key % 8, 1 );
}

void Input::injectKeyUp( const EE_KEY& Key ) {
	BitOp::writeBitKey( &mKeysUp	[ Key / 8 ], Key % 8, 1 );
}

void Input::injectButtonPress( const Uint32& Button ) {
	if ( Button < 8 )
		if ( !( mPressTrigger & EE_BUTTON_MASK( Button )  ) )
			mPressTrigger |= EE_BUTTON_MASK( Button );
}

void Input::injectButtonRelease( const Uint32& Button ) {
	if ( Button < 8 ) {
		if ( mPressTrigger & EE_BUTTON_MASK( Button )  )
			mPressTrigger &= ~EE_BUTTON_MASK( Button );

		if ( !( mReleaseTrigger & EE_BUTTON_MASK( Button )  ) )
			mReleaseTrigger |= EE_BUTTON_MASK( Button );

		if ( !( mClickTrigger & EE_BUTTON_MASK( Button )  ) )
			mClickTrigger |= EE_BUTTON_MASK( Button );
	}
}

Vector2i Input::getMousePos() const {
	return mMousePos;
}

void Input::setMousePos( const Vector2i& Pos ) {
	mMousePos = Pos;
}

Vector2f Input::getMousePosf() {
	return Vector2f( (Float)mMousePos.x, (Float)mMousePos.y );
}

Vector2i Input::getMousePosFromView( const View& View ) {
	Vector2i RealMousePos = getMousePos();
	Rect RView = View.getView();
	return Vector2i( RealMousePos.x - RView.Left, RealMousePos.y - RView.Top );
}

Uint32 Input::pushCallback( const InputCallback& cb ) {
	mNumCallBacks++;
	mCallbacks[ mNumCallBacks ] = cb;
	return mNumCallBacks;
}

void Input::popCallback( const Uint32& CallbackId ) {
	mCallbacks[ CallbackId ] = 0;
	mCallbacks.erase( mCallbacks.find(CallbackId) );
}

void Input::injectMousePos( const Vector2i& Pos ) {
	injectMousePos( Pos.x, Pos.y );
}

bool Input::isControlPressed() const {
	return ( mInputMod & KEYMOD_CTRL ) != 0;
}

bool Input::isShiftPressed() const {
	return ( mInputMod & KEYMOD_SHIFT ) != 0;
}

bool Input::isAltPressed() const {
	return ( mInputMod & KEYMOD_ALT ) != 0;
}

bool Input::isMetaPressed() const {
	return ( mInputMod & KEYMOD_META ) != 0;
}

bool Input::isMouseLeftPressed() const {
	return ( mPressTrigger & EE_BUTTON_LMASK ) != 0;
}

bool Input::isMouseRightPressed() const {
	return ( mPressTrigger & EE_BUTTON_RMASK ) != 0;
}

bool Input::isMouseMiddlePressed() const {
	return ( mPressTrigger & EE_BUTTON_MMASK ) != 0;
}

bool Input::mouseLeftClicked() const {
	return ( mClickTrigger & EE_BUTTON_LMASK ) != 0;
}

bool Input::mouseRightClicked() const {
	return ( mClickTrigger & EE_BUTTON_RMASK ) != 0;
}

bool Input::mouseMiddleClicked() const {
	return ( mClickTrigger & EE_BUTTON_MMASK ) != 0;
}

bool Input::mouseLeftDoubleClicked() const {
	return ( mDoubleClickTrigger & EE_BUTTON_LMASK ) != 0;
}

bool Input::mouseRightDoubleClicked() const {
	return ( mDoubleClickTrigger & EE_BUTTON_RMASK ) != 0;
}

bool Input::mouseMiddleDoubleClicked() const {
	return ( mDoubleClickTrigger & EE_BUTTON_MMASK ) != 0;
}

bool Input::mouseWheelScrolledUp() const {
	return ( mReleaseTrigger & EE_BUTTON_WUMASK ) != 0;
}

bool Input::mouseWheelScrolledDown() const {
	return ( mReleaseTrigger & EE_BUTTON_WDMASK ) != 0;
}

void Input::setMouseSpeed( const Float& Speed ) {
	mMouseSpeed = Speed;
}

const Float& Input::getMouseSpeed() const {
	return mMouseSpeed;
}

const Uint32& Input::getLastPressTrigger() const {
	return mLastPressTrigger;
}

const Uint32& Input::getPressTrigger() const {
	return mPressTrigger;
}

const Uint32& Input::getReleaseTrigger() const {
	return mReleaseTrigger;
}

const Uint32& Input::getClickTrigger() const {
	return mClickTrigger;
}

const Uint32& Input::getDoubleClickTrigger() const {
	return mDoubleClickTrigger;
}

const Uint32& Input::getDoubleClickInterval() const {
	return mDoubleClickInterval;
}

void Input::setDoubleClickInterval( const Uint32& Interval ) {
	mDoubleClickInterval = Interval;
}

JoystickManager * Input::getJoystickManager() const {
	return mJoystickManager;
}

Uint32 Input::getFingerCount() {
	 return EE_MAX_FINGERS;
}

InputFinger * Input::getFingerIndex( const Uint32 &Index ) {
	eeASSERT( Index < EE_MAX_FINGERS );
	return &mFingers[Index];
}

InputFinger * Input::getFinger( const Int64 &fingerId ) {
	for ( Uint32 i = 0; i < EE_MAX_FINGERS; i++ ) {
		if ( mFingers[i].id == fingerId ) {
			return &mFingers[i];
		}
	}

	return NULL;
}

std::list<InputFinger *> Input::getFingersDown() {
	std::list<InputFinger *> fDown;

	for ( Uint32 i = 0; i < EE_MAX_FINGERS; i++ ) {
		if ( mFingers[i].down ) {
			fDown.push_back( &mFingers[i] );
		}
	}

	return fDown;
}

std::list<InputFinger *> Input::getFingersWasDown() {
	std::list<InputFinger *> fDown;

	for ( Uint32 i = 0; i < EE_MAX_FINGERS; i++ ) {
		if ( mFingers[i].wasDown ) {
			fDown.push_back( &mFingers[i] );
		}
	}

	return fDown;
}

}}
