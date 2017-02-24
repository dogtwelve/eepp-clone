#include <eepp/ui/uimanager.hpp>
#include <eepp/window/engine.hpp>
#include <eepp/window/cursormanager.hpp>
#include <eepp/graphics/globalbatchrenderer.hpp>
#include <algorithm>

namespace EE { namespace UI {

SINGLETON_DECLARE_IMPLEMENTATION(UIManager)

UIManager::UIManager() :
	mWindow( NULL ),
	mKM( NULL ),
	mControl( NULL ),
	mFocusControl( NULL ),
	mOverControl( NULL ),
	mDownControl( NULL ),
	mLossFocusControl( NULL ),
	mCbId(-1),
	mResizeCb(0),
	mFlags( 0 ),
	mHighlightFocusColor( 234, 195, 123, 255 ),
	mHighlightOverColor( 195, 123, 234, 255 ),
	mInit( false ),
	mFirstPress( false ),
	mShootingDown( false ),
	mControlDragging( false ),
	mUseGlobalCursors( true )
{
}

UIManager::~UIManager() {
	shutdown();
}

void UIManager::init( Uint32 Flags, EE::Window::Window * window ) {
	if ( mInit )
		shutdown();

	mWindow		= window;
	mFlags		= Flags;

	if ( NULL == mWindow ) {
		mWindow = Engine::instance()->getCurrentWindow();
	}

	mKM				= mWindow->getInput();

	mInit			= true;

	UIWindow::CreateParams Params;
	Params.setParent( NULL );
	Params.setPosition( 0, 0 );
	Params.setSize( (Float)Engine::instance()->getWidth() / UIControl::getPixelDensity(), (Float)Engine::instance()->getHeight() / UIControl::getPixelDensity() );
	Params.Flags = UI_CONTROL_DEFAULT_FLAGS | UI_REPORT_SIZE_CHANGE_TO_CHILDS;
	Params.WinFlags = UI_WIN_NO_BORDER | UI_WIN_RESIZEABLE;
	Params.MinWindowSize = Sizei( 0, 0 );
	Params.DecorationSize = Sizei( 0, 0 );
	Params.DecorationAutoSize = false;

	mControl		= eeNew( UIWindow, ( Params ) );
	mControl->setVisible( true );
	mControl->setEnabled( true );
	mControl->getContainer()->setEnabled( false );
	mControl->getContainer()->setVisible( false );

	mFocusControl	= mControl;
	mOverControl	= mControl;

	mCbId = mKM->pushCallback( cb::Make1( this, &UIManager::inputCallback ) );
	mResizeCb = mWindow->pushResizeCallback( cb::Make1( this, &UIManager::resizeControl ) );
}

void UIManager::shutdown() {
	if ( mInit ) {
		if ( -1 != mCbId &&
			NULL != Engine::existsSingleton() &&
			Engine::instance()->existsWindow( mWindow )
		)
		{
			mKM->popCallback( mCbId );
			mWindow->popResizeCallback( mResizeCb );
		}

		mShootingDown = true;

		eeSAFE_DELETE( mControl );

		mShootingDown = false;

		mOverControl = NULL;
		mFocusControl = NULL;

		mInit = false;
	}

	UIThemeManager::destroySingleton();
}

void UIManager::inputCallback( InputEvent * Event ) {
	switch( Event->Type ) {
		case InputEvent::KeyUp:
			sendKeyUp( Event->key.keysym.sym, Event->key.keysym.unicode, Event->key.keysym.mod );
			break;
		case InputEvent::KeyDown:
			sendKeyDown( Event->key.keysym.sym, Event->key.keysym.unicode, Event->key.keysym.mod );

			checkTabPress( Event->key.keysym.sym );
			break;
	}
}

void UIManager::resizeControl( EE::Window::Window * win ) {
	mControl->setSize( (Float)mWindow->getWidth() / UIControl::getPixelDensity(), (Float)mWindow->getHeight() / UIControl::getPixelDensity() );
	sendMsg( mControl, UIMessage::MsgWindowResize );

	std::list<UIWindow*>::iterator it;

	for ( it = mWindowsList.begin(); it != mWindowsList.end(); it++ ) {
		sendMsg( *it, UIMessage::MsgWindowResize );
	}
}

void UIManager::sendKeyUp( const Uint32& KeyCode, const Uint16& Char, const Uint32& Mod ) {
	UIEventKey	KeyEvent	= UIEventKey( mFocusControl, UIEvent::EventKeyUp, KeyCode, Char, Mod );
	UIControl * CtrlLoop	= mFocusControl;

	while( NULL != CtrlLoop ) {
		if ( CtrlLoop->isEnabled() && CtrlLoop->onKeyUp( KeyEvent ) )
			break;

		CtrlLoop = CtrlLoop->getParent();
	}
}

void UIManager::sendKeyDown( const Uint32& KeyCode, const Uint16& Char, const Uint32& Mod ) {
	UIEventKey	KeyEvent	= UIEventKey( mFocusControl, UIEvent::EventKeyDown, KeyCode, Char, Mod );
	UIControl * CtrlLoop	= mFocusControl;

	while( NULL != CtrlLoop ) {
		if ( CtrlLoop->isEnabled() && CtrlLoop->onKeyDown( KeyEvent ) )
			break;

		CtrlLoop = CtrlLoop->getParent();
	}
}

UIControl * UIManager::getFocusControl() const {
	return mFocusControl;
}

UIControl * UIManager::getLossFocusControl() const {
	return mLossFocusControl;
}

void UIManager::setFocusControl( UIControl * Ctrl ) {
	if ( NULL != mFocusControl && NULL != Ctrl && Ctrl != mFocusControl ) {
		mLossFocusControl = mFocusControl;

		mFocusControl = Ctrl;

		mLossFocusControl->onFocusLoss();
		sendMsg( mLossFocusControl, UIMessage::MsgFocusLoss );

		mFocusControl->onFocus();
		sendMsg( mFocusControl, UIMessage::MsgFocus );
	}
}

UIControl * UIManager::getOverControl() const {
	return mOverControl;
}

void UIManager::setOverControl( UIControl * Ctrl ) {
	mOverControl = Ctrl;
}

void UIManager::sendMsg( UIControl * Ctrl, const Uint32& Msg, const Uint32& Flags ) {
	UIMessage tMsg( Ctrl, Msg, Flags );

	Ctrl->messagePost( &tMsg );
}

void UIManager::update() {
	mElapsed = mWindow->getElapsed();

	bool wasDraggingControl = isControlDragging();

	mControl->update();

	UIControl * pOver = mControl->overFind( mKM->getMousePosf() );

	if ( pOver != mOverControl ) {
		if ( NULL != mOverControl ) {
			sendMsg( mOverControl, UIMessage::MsgMouseExit );
			mOverControl->onMouseExit( mKM->getMousePos(), 0 );
		}

		mOverControl = pOver;

		if ( NULL != mOverControl ) {
			sendMsg( mOverControl, UIMessage::MsgMouseEnter );
			mOverControl->onMouseEnter( mKM->getMousePos(), 0 );
		}
	} else {
		if ( NULL != mOverControl )
			mOverControl->onMouseMove( mKM->getMousePos(), mKM->getPressTrigger() );
	}

	if ( mKM->getPressTrigger() ) {
		/*if ( !wasDraggingControl && mOverControl != mFocusControl )
			FocusControl( mOverControl );*/

		if ( NULL != mOverControl ) {
			mOverControl->onMouseDown( mKM->getMousePos(), mKM->getPressTrigger() );
			sendMsg( mOverControl, UIMessage::MsgMouseDown, mKM->getPressTrigger() );
		}

		if ( !mFirstPress ) {
			mDownControl = mOverControl;
			mMouseDownPos = mKM->getMousePos();

			mFirstPress = true;
		}
	}

	if ( mKM->getReleaseTrigger() ) {
		if ( NULL != mFocusControl ) {
			if ( !wasDraggingControl ) {
				if ( mOverControl != mFocusControl )
					setFocusControl( mOverControl );

				mFocusControl->onMouseUp( mKM->getMousePos(), mKM->getReleaseTrigger() );
				sendMsg( mFocusControl, UIMessage::MsgMouseUp, mKM->getReleaseTrigger() );

				if ( mKM->getClickTrigger() ) { // mDownControl == mOverControl &&
					sendMsg( mFocusControl, UIMessage::MsgClick, mKM->getClickTrigger() );
					mFocusControl->onMouseClick( mKM->getMousePos(), mKM->getClickTrigger() );

					if ( mKM->getDoubleClickTrigger() ) {
						sendMsg( mFocusControl, UIMessage::MsgDoubleClick, mKM->getDoubleClickTrigger() );
						mFocusControl->onMouseDoubleClick( mKM->getMousePos(), mKM->getDoubleClickTrigger() );
					}
				}
			}
		}

		mFirstPress = false;
	}

	checkClose();
}

UIControl * UIManager::getDownControl() const {
	return mDownControl;
}

void UIManager::draw() {
	GlobalBatchRenderer::instance()->draw();
	mControl->internalDraw();
	GlobalBatchRenderer::instance()->draw();
}

UIWindow * UIManager::getMainControl() const {
	return mControl;
}

const Time& UIManager::getElapsed() const {
	return mElapsed;
}

Vector2i UIManager::getMousePos() {
	return mKM->getMousePos();
}

Input * UIManager::getInput() const {
	return mKM;
}

const Uint32& UIManager::getPressTrigger() const {
	return mKM->getPressTrigger();
}

const Uint32& UIManager::getLastPressTrigger() const {
	return mKM->getLastPressTrigger();
}

void UIManager::clipPlaneEnable( const Int32& x, const Int32& y, const Uint32& Width, const Uint32& Height ) {
	mWindow->clipPlaneEnable( x, y, Width, Height );
}

void UIManager::clipPlaneDisable() {
	mWindow->clipPlaneDisable();
}

void UIManager::clipEnable( const Int32& x, const Int32& y, const Uint32& Width, const Uint32& Height ) {
	mWindow->clipEnable( x, y, Width, Height );
}

void UIManager::clipDisable() {
	mWindow->clipDisable();
}

void UIManager::clipSmartEnable(UIControl * ctrl, const Int32 & x, const Int32 & y, const Uint32 & Width, const Uint32 & Height) {
	if ( ctrl->isMeOrParentTreeRotated() ) {
		clipPlaneEnable( x, y, Width, Height );
	} else {
		clipEnable( x, y, Width, Height );
	}
}

void UIManager::clipSmartDisable(UIControl * ctrl) {
	if ( ctrl->isMeOrParentTreeRotated() ) {
		clipPlaneDisable();
	} else {
		clipDisable();
	}
}

void UIManager::setHighlightFocus( bool Highlight ) {
	BitOp::setBitFlagValue( &mFlags, UI_MANAGER_HIGHLIGHT_FOCUS, Highlight ? 1 : 0 );
}

bool UIManager::getHighlightFocus() const {
	return 0 != ( mFlags & UI_MANAGER_HIGHLIGHT_FOCUS );
}

void UIManager::setDrawDebugData( bool debug ) {
	BitOp::setBitFlagValue( &mFlags, UI_MANAGER_DRAW_DEBUG_DATA, debug ? 1 : 0 );
}

bool UIManager::getDrawDebugData() const {
	return 0 != ( mFlags & UI_MANAGER_DRAW_DEBUG_DATA );
}

void UIManager::setDrawBoxes( bool draw ) {
	BitOp::setBitFlagValue( &mFlags, UI_MANAGER_DRAW_BOXES, draw ? 1 : 0 );
}

bool UIManager::getDrawBoxes() const {
	return 0 != ( mFlags & UI_MANAGER_DRAW_BOXES );
}

void UIManager::setHighlightFocusColor( const ColorA& Color ) {
	mHighlightFocusColor = Color;
}

const ColorA& UIManager::getHighlightFocusColor() const {
	return mHighlightFocusColor;
}

void UIManager::setHighlightOver( bool Highlight ) {
	BitOp::setBitFlagValue( &mFlags, UI_MANAGER_HIGHLIGHT_OVER, Highlight ? 1 : 0 );
}

bool UIManager::getHighlightOver() const {
	return 0 != ( mFlags & UI_MANAGER_HIGHLIGHT_OVER );
}

void UIManager::setHighlightOverColor( const ColorA& Color ) {
	mHighlightOverColor = Color;
}

const ColorA& UIManager::getHighlightOverColor() const {
	return mHighlightOverColor;
}

void UIManager::checkTabPress( const Uint32& KeyCode ) {
	eeASSERT( NULL != mFocusControl );

	if ( KeyCode == KEY_TAB ) {
		UIControl * Ctrl = mFocusControl->getNextComplexControl();

		if ( NULL != Ctrl )
			Ctrl->setFocus();
	}
}

void UIManager::sendMouseClick( UIControl * ToCtrl, const Vector2i& Pos, const Uint32 Flags ) {
	sendMsg( ToCtrl, UIMessage::MsgClick, Flags );
	ToCtrl->onMouseClick( Pos, Flags );
}

void UIManager::sendMouseUp( UIControl * ToCtrl, const Vector2i& Pos, const Uint32 Flags ) {
	sendMsg( ToCtrl, UIMessage::MsgMouseUp, Flags );
	ToCtrl->onMouseUp( Pos, Flags );
}

void UIManager::sendMouseDown( UIControl * ToCtrl, const Vector2i& Pos, const Uint32 Flags ) {
	sendMsg( ToCtrl, UIMessage::MsgMouseDown, Flags );
	ToCtrl->onMouseDown( Pos, Flags );
}

EE::Window::Window * UIManager::getWindow() const {
	return mWindow;
}

void UIManager::setFocusLastWindow( UIWindow * window ) {
	if ( !mWindowsList.empty() && window != mWindowsList.front() ) {
		setFocusControl( mWindowsList.front() );
	}
}

void UIManager::windowAdd( UIWindow * win ) {
	if ( !windowExists( win ) ) {
		mWindowsList.push_front( win );
	} else {
		//! Send to front
		mWindowsList.remove( win );
		mWindowsList.push_front( win );
	}
}

void UIManager::windowRemove( UIWindow * win ) {
	if ( windowExists( win ) ) {
		mWindowsList.remove( win );
	}
}

bool UIManager::windowExists( UIWindow * win ) {
	return mWindowsList.end() != std::find( mWindowsList.begin(), mWindowsList.end(), win );
}

const bool& UIManager::isShootingDown() const {
	return mShootingDown;
}

const Vector2i &UIManager::getMouseDownPos() const {
	return mMouseDownPos;
}

void UIManager::addToCloseQueue( UIControl * Ctrl ) {
	eeASSERT( NULL != Ctrl );

	std::list<UIControl*>::iterator it;
	UIControl * itCtrl = NULL;

	for ( it = mCloseList.begin(); it != mCloseList.end(); it++ ) {
		itCtrl = *it;

		if ( NULL != itCtrl && itCtrl->isParentOf( Ctrl ) ) {
			// If a parent will be removed, means that the control
			// that we are trying to queue will be removed by the father
			// so we skip it
			return;
		}
	}

	std::list< std::list<UIControl*>::iterator > itEraseList;

	for ( it = mCloseList.begin(); it != mCloseList.end(); it++ ) {
		itCtrl = *it;

		if ( NULL != itCtrl && Ctrl->isParentOf( itCtrl ) ) {
			// if the control added is parent of another control already added,
			// we remove the already added control because it will be deleted
			// by its parent
			itEraseList.push_back( it );
		} else if ( NULL == itCtrl ) {
			itEraseList.push_back( it );
		}
	}

	// We delete all the controls that don't need to be deleted
	// because of the new added control to the queue
	std::list< std::list<UIControl*>::iterator >::iterator ite;

	for ( ite = itEraseList.begin(); ite != itEraseList.end(); ite++ ) {
		mCloseList.erase( *ite );
	}

	mCloseList.push_back( Ctrl );
}

void UIManager::checkClose() {
	if ( !mCloseList.empty() ) {
		for ( std::list<UIControl*>::iterator it = mCloseList.begin(); it != mCloseList.end(); it++ ) {
			eeDelete( *it );
		}

		mCloseList.clear();
	}
}

void UIManager::setControlDragging( bool dragging ) {
	mControlDragging = dragging;
}

const bool& UIManager::isControlDragging() const {
	return mControlDragging;
}

void UIManager::setUseGlobalCursors( const bool& use ) {
	mUseGlobalCursors = use;
}

const bool& UIManager::getUseGlobalCursors() {
	return mUseGlobalCursors;
}

void UIManager::setCursor( EE_CURSOR_TYPE cursor ) {
	if ( mUseGlobalCursors ) {
		mWindow->getCursorManager()->set( cursor );
	}
}

}}
