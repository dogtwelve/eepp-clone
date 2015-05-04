#include <eepp/ui/cuicontrol.hpp>
#include <eepp/ui/cuitheme.hpp>
#include <eepp/ui/cuimanager.hpp>
#include <eepp/graphics/cprimitives.hpp>
#include <eepp/graphics/csubtexture.hpp>
#include <eepp/graphics/renderer/cgl.hpp>

namespace EE { namespace UI {

cUIControl::cUIControl( const CreateParams& Params ) :
	mPos( Params.Pos ),
	mSize( Params.Size ),
	mFlags( Params.Flags ),
	mData( 0 ),
	mParentCtrl( Params.ParentCtrl ),
	mChild( NULL ),
	mChildLast( NULL ),
	mNext( NULL ),
	mPrev( NULL ),
	mSkinState( NULL ),
	mBackground( NULL ),
	mBorder( NULL ),
	mControlFlags( 0 ),
	mBlend( Params.Blend ),
	mNumCallBacks( 0 ),
	mVisible( false ),
	mEnabled( false )
{
	if ( NULL == mParentCtrl && NULL != cUIManager::instance()->MainControl() ) {
		mParentCtrl = cUIManager::instance()->MainControl();
	}

	if ( NULL != mParentCtrl )
		mParentCtrl->ChildAdd( this );

	if ( mFlags & UI_FILL_BACKGROUND )
		mBackground = eeNew( cUIBackground, ( Params.Background ) );

	if ( mFlags & UI_BORDER )
		mBorder = eeNew( cUIBorder, ( Params.Border ) );

	UpdateScreenPos();
	UpdateQuad();
}

cUIControl::~cUIControl() {
	SafeDeleteSkinState();
	eeSAFE_DELETE( mBackground );
	eeSAFE_DELETE( mBorder );

	ChildDeleteAll();

	if ( NULL != mParentCtrl )
		mParentCtrl->ChildRemove( this );

	if ( cUIManager::instance()->FocusControl() == this && cUIManager::instance()->MainControl() != this ) {
		cUIManager::instance()->FocusControl( cUIManager::instance()->MainControl() );
	}

	if ( cUIManager::instance()->OverControl() == this && cUIManager::instance()->MainControl() != this ) {
		cUIManager::instance()->OverControl( cUIManager::instance()->MainControl() );
	}
}

void cUIControl::ScreenToControl( eeVector2i& Pos ) const {
	cUIControl * ParentLoop = mParentCtrl;

	Pos.x -= mPos.x;
	Pos.y -= mPos.y;

	while ( NULL != ParentLoop ) {
		const eeVector2i& ParentPos = ParentLoop->Pos();

		Pos.x -= ParentPos.x;
		Pos.y -= ParentPos.y;

		ParentLoop = ParentLoop->Parent();
	}
}

void cUIControl::ControlToScreen( eeVector2i& Pos ) const {
	cUIControl * ParentLoop = mParentCtrl;

	while ( NULL != ParentLoop ) {
		const eeVector2i& ParentPos = ParentLoop->Pos();

		Pos.x += ParentPos.x;
		Pos.y += ParentPos.y;

		ParentLoop = ParentLoop->Parent();
	}
}

Uint32 cUIControl::Type() const {
	return UI_TYPE_CONTROL;
}

bool cUIControl::IsType( const Uint32& type ) const {
	return cUIControl::Type() == type;
}

void cUIControl::MessagePost( const cUIMessage * Msg ) {
	cUIControl * Ctrl = this;

	while( NULL != Ctrl ) {
		if ( Ctrl->OnMessage( Msg ) )
			break;

		Ctrl = Ctrl->Parent();
	}
}

Uint32 cUIControl::OnMessage( const cUIMessage * Msg ) {
	return 0;
}

bool cUIControl::IsInside( const eeVector2i& Pos ) const {
	return ( Pos.x >= 0 && Pos.y >= 0 && Pos.x < mSize.Width() && Pos.y < mSize.Height() );
}

void cUIControl::Pos( const eeVector2i& Pos ) {
	mPos = Pos;
	OnPosChange();
}

void cUIControl::Pos( const Int32& x, const Int32& y ) {
	mPos = eeVector2i( x, y );
	OnPosChange();
}

const eeVector2i& cUIControl::Pos() const {
	return mPos;
}

void cUIControl::Size( const eeSize& Size ) {
	if ( Size != mSize ) {
		eeVector2i sizeChange( Size.x - mSize.x, Size.y - mSize.y );

		mSize = Size;

		OnSizeChange();

		if ( mFlags & UI_REPORT_SIZE_CHANGE_TO_CHILDS ) {
			SendParentSizeChange( sizeChange );
		}
	}
}

void cUIControl::Size( const Int32& Width, const Int32& Height ) {
	Size( eeSize( Width, Height ) );
}

eeRecti cUIControl::Rect() const {
	return eeRecti( mPos, mSize );
}

const eeSize& cUIControl::Size() {
	return mSize;
}

void cUIControl::Visible( const bool& visible ) {
	mVisible = visible;
	OnVisibleChange();
}

bool cUIControl::Visible() const {
	return mVisible;
}

bool cUIControl::Hided() const {
	return !mVisible;
}

void cUIControl::Enabled( const bool& enabled ) {
	mEnabled = enabled;
	OnEnabledChange();
}

bool cUIControl::Enabled() const {
	return mEnabled;
}

bool cUIControl::Disabled() const {
	return !mEnabled;
}

cUIControl * cUIControl::Parent() const {
	return mParentCtrl;
}

void cUIControl::Parent( cUIControl * parent ) {
	if ( parent == mParentCtrl )
		return;

	if ( NULL != mParentCtrl )
		mParentCtrl->ChildRemove( this );

	mParentCtrl = parent;

	if ( NULL != mParentCtrl )
		mParentCtrl->ChildAdd( this );
}

bool cUIControl::IsParentOf( cUIControl * Ctrl ) {
	eeASSERT( NULL != Ctrl );

	cUIControl * tParent = Ctrl->Parent();

	while ( NULL != tParent ) {
		if ( this == tParent )
			return true;

		tParent = tParent->Parent();
	}

	return false;
}

void cUIControl::CenterHorizontal() {
	cUIControl * Ctrl = Parent();

	if ( NULL != Ctrl )
		Pos( eeVector2i( ( Ctrl->Size().Width() / 2 ) - ( mSize.Width() / 2 ), mPos.y ) );
}

void cUIControl::CenterVertical(){
	cUIControl * Ctrl = Parent();

	if ( NULL != Ctrl )
		Pos( eeVector2i( mPos.x, ( Ctrl->Size().Height() / 2 ) - ( mSize.Height() / 2 ) ) );
}

void cUIControl::Center() {
	CenterHorizontal();
	CenterVertical();
}

void cUIControl::Close() {
	mControlFlags |= UI_CTRL_FLAG_CLOSE;

	cUIManager::instance()->AddToCloseQueue( this );
}

void cUIControl::Draw() {
	if ( mVisible ) {
		if ( mFlags & UI_FILL_BACKGROUND )
			BackgroundDraw();

		if ( mFlags & UI_BORDER )
			BorderDraw();

		if ( NULL != mSkinState )
			mSkinState->Draw( (eeFloat)mScreenPos.x, (eeFloat)mScreenPos.y, (eeFloat)mSize.Width(), (eeFloat)mSize.Height(), 255 );

		if ( cUIManager::instance()->HighlightFocus() && cUIManager::instance()->FocusControl() == this ) {
			cPrimitives P;
			P.FillMode( DRAW_LINE );
			P.BlendMode( Blend() );
			P.SetColor( cUIManager::instance()->HighlightFocusColor() );
			P.DrawRectangle( GetRectf() );
		}

		if ( cUIManager::instance()->HighlightOver() && cUIManager::instance()->OverControl() == this ) {
			cPrimitives P;
			P.FillMode( DRAW_LINE );
			P.BlendMode( Blend() );
			P.SetColor( cUIManager::instance()->HighlightOverColor() );
			P.DrawRectangle( GetRectf() );
		}
	}
}

void cUIControl::Update() {
	cUIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->Update();
		ChildLoop = ChildLoop->mNext;
	}

	if ( mControlFlags & UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD )
		WriteCtrlFlag( UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD, 0 );
}

void cUIControl::SendMouseEvent( const Uint32& Event, const eeVector2i& Pos, const Uint32& Flags ) {
	cUIEventMouse MouseEvent( this, Event, Pos, Flags );
	SendEvent( &MouseEvent );
}

void cUIControl::SendCommonEvent( const Uint32& Event ) {
	cUIEvent CommonEvent( this, Event );
	SendEvent( &CommonEvent );
}

Uint32 cUIControl::OnKeyDown( const cUIEventKey& Event ) {
	SendEvent( &Event );
	return 0;
}

Uint32 cUIControl::OnKeyUp( const cUIEventKey& Event ) {
	SendEvent( &Event );
	return 0;
}

Uint32 cUIControl::OnMouseMove( const eeVector2i& Pos, const Uint32 Flags ) {
	SendMouseEvent( cUIEvent::EventMouseMove, Pos, Flags );
	return 1;
}

Uint32 cUIControl::OnMouseDown( const eeVector2i& Pos, const Uint32 Flags ) {
	SendMouseEvent( cUIEvent::EventMouseDown, Pos, Flags );

	SetSkinState( cUISkinState::StateMouseDown );

	return 1;
}

Uint32 cUIControl::OnMouseUp( const eeVector2i& Pos, const Uint32 Flags ) {
	SendMouseEvent( cUIEvent::EventMouseUp, Pos, Flags );

	SetPrevSkinState();

	return 1;
}

Uint32 cUIControl::OnMouseClick( const eeVector2i& Pos, const Uint32 Flags ) {
	SendMouseEvent( cUIEvent::EventMouseClick, Pos, Flags );
	return 1;
}

bool cUIControl::IsMouseOver() {
	return 0 != ( mControlFlags & UI_CTRL_FLAG_MOUSEOVER );
}

bool cUIControl::IsMouseOverMeOrChilds() {
	return 0 != ( mControlFlags & UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD );
}

Uint32 cUIControl::OnMouseDoubleClick( const eeVector2i& Pos, const Uint32 Flags ) {
	SendMouseEvent( cUIEvent::EventMouseDoubleClick, Pos, Flags );
	return 1;
}

Uint32 cUIControl::OnMouseEnter( const eeVector2i& Pos, const Uint32 Flags ) {
	WriteCtrlFlag( UI_CTRL_FLAG_MOUSEOVER, 1 );

	SendMouseEvent( cUIEvent::EventMouseEnter, Pos, Flags );

	SetSkinState( cUISkinState::StateMouseEnter );

	return 1;
}

Uint32 cUIControl::OnMouseExit( const eeVector2i& Pos, const Uint32 Flags ) {
	WriteCtrlFlag( UI_CTRL_FLAG_MOUSEOVER, 0 );

	SendMouseEvent( cUIEvent::EventMouseExit, Pos, Flags );

	SetSkinState( cUISkinState::StateMouseExit );

	return 1;
}

Uint32 cUIControl::OnFocus() {
	mControlFlags |= UI_CTRL_FLAG_HAS_FOCUS;

	SendCommonEvent( cUIEvent::EventOnFocus );

	SetSkinState( cUISkinState::StateFocus );

	return 1;
}

Uint32 cUIControl::OnFocusLoss() {
	mControlFlags &= ~UI_CTRL_FLAG_HAS_FOCUS;

	SendCommonEvent( cUIEvent::EventOnFocusLoss );

	return 1;
}

void cUIControl::OnComplexControlFocusLoss() {
	SendCommonEvent( cUIEvent::EventOnComplexControlFocusLoss );
}

bool cUIControl::HasFocus() const {
	return 0 != ( mControlFlags & UI_CTRL_FLAG_HAS_FOCUS );
}

Uint32 cUIControl::OnValueChange() {
	SendCommonEvent( cUIEvent::EventOnValueChange );

	return 1;
}

void cUIControl::OnClose() {
	SendCommonEvent( cUIEvent::EventOnClose );
}

Uint32 cUIControl::HAlign() const {
	return mFlags & UI_HALIGN_MASK;
}

void cUIControl::HAlign( Uint32 halign ) {
	mFlags |= halign & UI_HALIGN_MASK;
}

Uint32 cUIControl::VAlign() const {
	return mFlags & UI_VALIGN_MASK;
}

void cUIControl::VAlign( Uint32 valign ) {
	mFlags |= valign & UI_VALIGN_MASK;
}

void cUIControl::FillBackground( bool enabled ) {
	WriteFlag( UI_FILL_BACKGROUND, enabled ? 1 : 0 );

	if ( enabled && NULL == mBackground ) {
		mBackground = eeNew( cUIBackground, () );
	}
}

void cUIControl::Border( bool enabled ) {
	WriteFlag( UI_BORDER, enabled ? 1 : 0 );

	if ( enabled && NULL == mBorder ) {
		mBorder = eeNew( cUIBorder, () );

		if ( NULL == mBackground ) {
			mBackground = eeNew( cUIBackground, () );
		}
	}
}

cUIControl * cUIControl::NextGet() const {
	return mNext;
}

cUIControl * cUIControl::PrevGet() const {
	return mPrev;
}

cUIControl * cUIControl::NextGetLoop() const {
	if ( NULL == mNext )
		return Parent()->ChildGetFirst();
	else
		return mNext;
}

void cUIControl::Data(const UintPtr& data ) {
	mData = data;
}

const UintPtr& cUIControl::Data() const {
	return mData;
}

const Uint32& cUIControl::Flags() const {
	return mFlags;
}

void cUIControl::Flags( const Uint32& flags ) {
	mFlags |= flags;
}

void cUIControl::Blend( const EE_BLEND_MODE& blend ) {
	mBlend = static_cast<Uint16> ( blend );
}

EE_BLEND_MODE cUIControl::Blend() {
	return static_cast<EE_BLEND_MODE> ( mBlend );
}

void cUIControl::ToFront() {
	if ( NULL != mParentCtrl ) {
		mParentCtrl->ChildRemove( this );
		mParentCtrl->ChildAdd( this );
	}
}

void cUIControl::ToBack() {
	if ( NULL != mParentCtrl ) {
		mParentCtrl->ChildAddAt( this, 0 );
	}
}

void cUIControl::ToPos( const Uint32& Pos ) {
	if ( NULL != mParentCtrl ) {
		mParentCtrl->ChildAddAt( this, Pos );
	}
}

void cUIControl::OnVisibleChange() {
	SendCommonEvent( cUIEvent::EventOnVisibleChange );
}

void cUIControl::OnEnabledChange() {
	if ( !Enabled() && NULL != cUIManager::instance()->FocusControl() ) {
		if ( IsChild( cUIManager::instance()->FocusControl() ) ) {
			cUIManager::instance()->FocusControl( NULL );
		}
	}

	SendCommonEvent( cUIEvent::EventOnEnabledChange );
}

void cUIControl::OnPosChange() {
	UpdateScreenPos();

	UpdateChildsScreenPos();

	SendCommonEvent( cUIEvent::EventOnPosChange );
}

void cUIControl::OnSizeChange() {
	SendCommonEvent( cUIEvent::EventOnSizeChange );
}

eeRectf cUIControl::GetRectf() {
	return eeRectf( eeVector2f( (eeFloat)mScreenPos.x, (eeFloat)mScreenPos.y ), eeSizef( (eeFloat)mSize.Width(), (eeFloat)mSize.Height() ) );
}

void cUIControl::BackgroundDraw() {
	cPrimitives P;
	eeRectf R = GetRectf();
	P.BlendMode( mBackground->Blend() );
	P.SetColor( mBackground->Color() );

	if ( 4 == mBackground->Colors().size() ) {
		if ( mBackground->Corners() ) {
			P.DrawRoundedRectangle( R, mBackground->Colors()[0], mBackground->Colors()[1], mBackground->Colors()[2], mBackground->Colors()[3], mBackground->Corners() );
		} else {
			P.DrawRectangle( R, mBackground->Colors()[0], mBackground->Colors()[1], mBackground->Colors()[2], mBackground->Colors()[3] );
		}
	} else {
		if ( mBackground->Corners() ) {
			P.DrawRoundedRectangle( R, 0.f, eeVector2f::One, mBackground->Corners() );
		} else {
			P.DrawRectangle( R );
		}
	}
}

void cUIControl::BorderDraw() {
	cPrimitives P;
	P.FillMode( DRAW_LINE );
	P.BlendMode( Blend() );
	P.LineWidth( (eeFloat)mBorder->Width() );
	P.SetColor( mBorder->Color() );

	//! @TODO: Check why was this +0.1f -0.1f?
	if ( mFlags & UI_CLIP_ENABLE ) {
		eeRectf R( eeVector2f( (eeFloat)mScreenPos.x + 0.1f, (eeFloat)mScreenPos.y + 0.1f ), eeSizef( (eeFloat)mSize.Width() - 0.1f, (eeFloat)mSize.Height() - 0.1f ) );

		if ( mBackground->Corners() ) {
			P.DrawRoundedRectangle( GetRectf(), 0.f, eeVector2f::One, mBackground->Corners() );
		} else {
			P.DrawRectangle( R );
		}
	} else {
		if ( mBackground->Corners() ) {
			P.DrawRoundedRectangle( GetRectf(), 0.f, eeVector2f::One, mBackground->Corners() );
		} else {
			P.DrawRectangle( GetRectf() );
		}
	}
}

const Uint32& cUIControl::ControlFlags() const {
	return mControlFlags;
}

void cUIControl::ControlFlags( const Uint32& Flags ) {
	mControlFlags = Flags;
}

void cUIControl::DrawChilds() {
	cUIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		if ( ChildLoop->mVisible ) {
			ChildLoop->InternalDraw();
		}

		ChildLoop = ChildLoop->mNext;
	}
}

void cUIControl::InternalDraw() {
	if ( mVisible ) {
		MatrixSet();

		ClipMe();

		Draw();

		DrawChilds();

		ClipDisable();

		MatrixUnset();
	}
}

void cUIControl::ClipMe() {
	if ( mFlags & UI_CLIP_ENABLE ) {
		if ( mFlags & UI_BORDER )
			cUIManager::instance()->ClipEnable( mScreenPos.x, mScreenPos.y, mSize.Width(), mSize.Height() + 1 );
		else
			cUIManager::instance()->ClipEnable( mScreenPos.x, mScreenPos.y, mSize.Width(), mSize.Height() );
	}
}

void cUIControl::ClipDisable() {
	if ( mFlags & UI_CLIP_ENABLE )
		cUIManager::instance()->ClipDisable();
}

void cUIControl::MatrixSet() {
}

void cUIControl::MatrixUnset() {
}

void cUIControl::ChildDeleteAll() {
	while( NULL != mChild ) {
		eeDelete( mChild );
	}
}

void cUIControl::ChildAdd( cUIControl * ChildCtrl ) {
	if ( NULL == mChild ) {
		mChild 		= ChildCtrl;
		mChildLast 	= ChildCtrl;
	} else {
		mChildLast->mNext 		= ChildCtrl;
		ChildCtrl->mPrev		= mChildLast;
		ChildCtrl->mNext		= NULL;
		mChildLast 				= ChildCtrl;
	}
}

void cUIControl::ChildAddAt( cUIControl * ChildCtrl, Uint32 Pos ) {
	eeASSERT( NULL != ChildCtrl );

	cUIControl * ChildLoop = mChild;
	
	ChildCtrl->Parent( this );

	ChildRemove( ChildCtrl );
	ChildCtrl->mParentCtrl = this;
	
	if ( ChildLoop == NULL ) {
		mChild 				= ChildCtrl;
		mChildLast			= ChildCtrl;
		ChildCtrl->mNext 	= NULL;
		ChildCtrl->mPrev 	= NULL;
	} else {
		if( Pos == 0 ) {
			if ( NULL != mChild ) {
				mChild->mPrev		= ChildCtrl;
			}

			ChildCtrl->mNext 	= mChild;
			ChildCtrl->mPrev	= NULL;
			mChild 				= ChildCtrl;
		} else {
			Uint32 i = 0;

			while ( NULL != ChildLoop->mNext && i < Pos ) {
				ChildLoop = ChildLoop->mNext;
				i++;
			}

			cUIControl * ChildTmp = ChildLoop->mNext;
			ChildLoop->mNext 	= ChildCtrl;
			ChildCtrl->mPrev 	= ChildLoop;
			ChildCtrl->mNext 	= ChildTmp;

			if ( NULL != ChildTmp ) {
				ChildTmp->mPrev = ChildCtrl;
			} else {
				mChildLast		= ChildCtrl;
			}
		}
	}
}

void cUIControl::ChildRemove( cUIControl * ChildCtrl ) {
	if ( ChildCtrl == mChild ) {
		mChild 			= mChild->mNext;

		if ( NULL != mChild ) {
			mChild->mPrev 	= NULL;

			if ( ChildCtrl == mChildLast )
				mChildLast		= mChild;
		} else {
			mChildLast		= NULL;
		}
	} else {
		if ( mChildLast == ChildCtrl )
			mChildLast = mChildLast->mPrev;

		ChildCtrl->mPrev->mNext = ChildCtrl->mNext;

		if ( NULL != ChildCtrl->mNext ) {
			ChildCtrl->mNext->mPrev = ChildCtrl->mPrev;
			ChildCtrl->mNext = NULL;
		}

		ChildCtrl->mPrev = NULL;
	}
}

void cUIControl::ChildsCloseAll() {
	cUIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->Close();
		ChildLoop = ChildLoop->mNext;
	}
}

bool cUIControl::IsChild( cUIControl * ChildCtrl ) const {
	cUIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		if ( ChildCtrl == ChildLoop )
			return true;

		ChildLoop = ChildLoop->mNext;
	}

	return false;
}

bool cUIControl::InParentTreeOf( cUIControl * Child ) const {
	cUIControl * ParentLoop = Child->mParentCtrl;

	while ( NULL != ParentLoop ) {
		if ( ParentLoop == this )
			return true;

		ParentLoop = ParentLoop->mParentCtrl;
	}

	return false;
}

Uint32 cUIControl::ChildCount() const {
	cUIControl * ChildLoop = mChild;
	Uint32 Count = 0;

	while( NULL != ChildLoop ) {
		ChildLoop = ChildLoop->mNext;
		Count++;
	}

	return Count;
}

cUIControl * cUIControl::ChildAt( Uint32 Index ) const {
	cUIControl * ChildLoop = mChild;

	while( NULL != ChildLoop && Index ) {
		ChildLoop = ChildLoop->mNext;
		Index--;
	}

	return ChildLoop;
}

cUIControl * cUIControl::ChildPrev( cUIControl * Ctrl, bool Loop ) const {
	if ( Loop && Ctrl == mChild && NULL != mChild->mNext )
		return mChildLast;

	return Ctrl->mPrev;
}

cUIControl * cUIControl::ChildNext( cUIControl * Ctrl, bool Loop ) const {
	if ( NULL == Ctrl->mNext && Loop )
		return mChild;

	return Ctrl->mNext;
}

cUIControl * cUIControl::ChildGetFirst() const {
	return mChild;
}

cUIControl * cUIControl::ChildGetLast() const {
	return mChildLast;
}

cUIControl * cUIControl::OverFind( const eeVector2f& Point ) {
	cUIControl * pOver = NULL;

	if ( mEnabled && mVisible ) {
		UpdateQuad();

		if ( mPoly.PointInside( Point ) ) {
			WriteCtrlFlag( UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD, 1 );

			cUIControl * ChildLoop = mChildLast;

			while ( NULL != ChildLoop ) {
				cUIControl * ChildOver = ChildLoop->OverFind( Point );

				if ( NULL != ChildOver ) {
					pOver = ChildOver;

					break; // Search from top to bottom, so the first over will be the topmost
				}

				ChildLoop = ChildLoop->mPrev;
			}


			if ( NULL == pOver )
				pOver = this;
		}
	}

	return pOver;
}

cUIControl * cUIControl::ChildGetAt( eeVector2i CtrlPos, eeUint RecursiveLevel ) {
	cUIControl * Ctrl = NULL;

	for( cUIControl * pLoop = mChild; NULL != pLoop && NULL == Ctrl; pLoop = pLoop->mNext )
	{
		if ( !pLoop->Visible() )
			continue;

		if ( pLoop->Rect().Contains( CtrlPos ) ) {
			if ( RecursiveLevel )
				Ctrl = ChildGetAt( CtrlPos - pLoop->Pos(), RecursiveLevel - 1 );

			if ( NULL == Ctrl )
				Ctrl = pLoop;
		}
	}

	return Ctrl;
}

Uint32 cUIControl::IsAnimated() {
	return mControlFlags & UI_CTRL_FLAG_ANIM;
}

Uint32 cUIControl::IsDragable() {
	return mControlFlags & UI_CTRL_FLAG_DRAGABLE;
}

Uint32 cUIControl::IsComplex() {
	return mControlFlags & UI_CTRL_FLAG_COMPLEX;
}

Uint32 cUIControl::IsClipped() {
	return mFlags & UI_CLIP_ENABLE;
}

eePolygon2f& cUIControl::GetPolygon() {
	return mPoly;
}

const eeVector2f& cUIControl::GetPolygonCenter() const {
	return mCenter;
}

void cUIControl::UpdateQuad() {
	mPoly 	= eePolygon2f( eeAABB( (eeFloat)mScreenPos.x, (eeFloat)mScreenPos.y, (eeFloat)mScreenPos.x + mSize.Width(), (eeFloat)mScreenPos.y + mSize.Height() ) );
	mCenter = eeVector2f( (eeFloat)mScreenPos.x + (eeFloat)mSize.Width() * 0.5f, (eeFloat)mScreenPos.y + (eeFloat)mSize.Height() * 0.5f );

	cUIControl * tParent = Parent();

	while ( tParent ) {
		if ( tParent->IsAnimated() ) {
			cUIControlAnim * tP = reinterpret_cast<cUIControlAnim *> ( tParent );

			mPoly.Rotate( tP->Angle(), tP->GetPolygonCenter() );
			mPoly.Scale( tP->Scale(), tP->GetPolygonCenter() );
		}

		tParent = tParent->Parent();
	};
}

cTime cUIControl::Elapsed() {
	return cUIManager::instance()->Elapsed();
}

Uint32 cUIControl::AddEventListener( const Uint32& EventType, const UIEventCallback& Callback ) {
	mNumCallBacks++;

	mEvents[ EventType ][ mNumCallBacks ] = Callback;

	return mNumCallBacks;
}

void cUIControl::RemoveEventListener( const Uint32& CallbackId ) {
	UIEventsMap::iterator it;

	for ( it = mEvents.begin(); it != mEvents.end(); ++it ) {
		std::map<Uint32, UIEventCallback> event = it->second;

		if ( event.erase( CallbackId ) )
			break;
	}
}

void cUIControl::SendEvent( const cUIEvent * Event ) {
	if ( 0 != mEvents.count( Event->EventType() ) ) {
		std::map<Uint32, UIEventCallback>			event = mEvents[ Event->EventType() ];
		std::map<Uint32, UIEventCallback>::iterator it;

		if ( event.begin() != event.end() ) {
			for ( it = event.begin(); it != event.end(); ++it )
				it->second( Event );
		}
	}
}

cUIBackground * cUIControl::Background() {
	return mBackground;
}

cUIBorder * cUIControl::Border() {
	return mBorder;
}

void cUIControl::SetThemeByName( const std::string& Theme ) {
	SetTheme( cUIThemeManager::instance()->GetByName( Theme ) );
}

void cUIControl::SetTheme( cUITheme * Theme ) {
	SetThemeControl( Theme, "control" );
}

void cUIControl::SafeDeleteSkinState() {
	if ( NULL != mSkinState && ( mControlFlags & UI_CTRL_FLAG_SKIN_OWNER ) ) {
		cUISkin * tSkin = mSkinState->GetSkin();

		eeSAFE_DELETE( tSkin );
	}

	eeSAFE_DELETE( mSkinState );
}

void cUIControl::SetThemeControl( cUITheme * Theme, const std::string& ControlName ) {
	if ( NULL != Theme ) {
		cUISkin * tSkin = Theme->GetByName( Theme->Abbr() + "_" + ControlName );

		if ( NULL != tSkin ) {
			Uint32 InitialState = cUISkinState::StateNormal;

			if ( NULL != mSkinState ) {
				InitialState = mSkinState->GetState();
			}

			SafeDeleteSkinState();

			mSkinState = eeNew( cUISkinState, ( tSkin ) );
			mSkinState->SetState( InitialState );
		}
	}
}

void cUIControl::SetSkinFromTheme( cUITheme * Theme, const std::string& ControlName ) {
	SetThemeControl( Theme, ControlName );
}

void cUIControl::SetSkin( const cUISkin& Skin ) {
	SafeDeleteSkinState();

	WriteCtrlFlag( UI_CTRL_FLAG_SKIN_OWNER, 1 );

	cUISkin * SkinCopy = const_cast<cUISkin*>( &Skin )->Copy();

	mSkinState = eeNew( cUISkinState, ( SkinCopy ) );

	DoAfterSetTheme();
}

void cUIControl::OnStateChange() {
}

void cUIControl::SetSkinState( const Uint32& State ) {
	if ( NULL != mSkinState ) {
		mSkinState->SetState( State );

		OnStateChange();
	}
}

void cUIControl::SetPrevSkinState() {
	if ( NULL != mSkinState ) {
		mSkinState->SetPrevState();

		OnStateChange();
	}
}

void cUIControl::SetThemeToChilds( cUITheme * Theme ) {
	cUIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->SetThemeToChilds( Theme );
		ChildLoop->SetTheme( Theme );	// First set the theme to childs to let the father override the childs forced themes

		ChildLoop = ChildLoop->mNext;
	}
}

void cUIControl::UpdateChildsScreenPos() {
	cUIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->UpdateScreenPos();
		ChildLoop->UpdateChildsScreenPos();

		ChildLoop = ChildLoop->mNext;
	}
}

void cUIControl::UpdateScreenPos() {
	eeVector2i Pos( mPos );

	ControlToScreen( Pos );

	mScreenPos = Pos;
}

cUISkin * cUIControl::GetSkin() {
	if ( NULL != mSkinState )
		return mSkinState->GetSkin();

	return NULL;
}

void cUIControl::WriteCtrlFlag( const Uint32& Flag, const Uint32& Val ) {
	BitOp::SetBitFlagValue( &mControlFlags, Flag, Val );
}

void cUIControl::WriteFlag( const Uint32& Flag, const Uint32& Val ) {
	if ( Val )
		mFlags |= Flag;
	else {
		if ( mFlags & Flag )
			mFlags &= ~Flag;
	}
}

void cUIControl::ApplyDefaultTheme() {
	cUIThemeManager::instance()->ApplyDefaultTheme( this );
}

eeRecti cUIControl::GetScreenRect() {
	return eeRecti( mScreenPos, mSize );
}

eeRecti cUIControl::MakePadding( bool PadLeft, bool PadRight, bool PadTop, bool PadBottom, bool SkipFlags ) {
	eeRecti tPadding( 0, 0, 0, 0 );

	if ( mFlags & UI_AUTO_PADDING || SkipFlags ) {
		if ( NULL != mSkinState && NULL != mSkinState->GetSkin() ) {
			if ( mSkinState->GetSkin()->GetType() == cUISkin::UISkinComplex ) {
				cUISkinComplex * tComplex = reinterpret_cast<cUISkinComplex*> ( mSkinState->GetSkin() );

				cSubTexture * tSubTexture = NULL;

				if ( PadLeft ) {
					tSubTexture = tComplex->GetSubTextureSide( cUISkinState::StateNormal, cUISkinComplex::Left );

					if ( NULL != tSubTexture )
						tPadding.Left = tSubTexture->RealSize().Width();
				}

				if ( PadRight ) {
					tSubTexture = tComplex->GetSubTextureSide( cUISkinState::StateNormal, cUISkinComplex::Right );

					if ( NULL != tSubTexture )
						tPadding.Right = tSubTexture->RealSize().Width();
				}

				if ( PadTop ) {
					tSubTexture = tComplex->GetSubTextureSide( cUISkinState::StateNormal, cUISkinComplex::Up );

					if ( NULL != tSubTexture )
						tPadding.Top = tSubTexture->RealSize().Height();
				}

				if ( PadBottom ) {
					tSubTexture = tComplex->GetSubTextureSide( cUISkinState::StateNormal, cUISkinComplex::Down );

					if ( NULL != tSubTexture )
						tPadding.Bottom = tSubTexture->RealSize().Height();
				}
			}
		}
	}

	return tPadding;
}

void cUIControl::SetFocus() {
	cUIManager::instance()->FocusControl( this );
}

void cUIControl::SendParentSizeChange( const eeVector2i& SizeChange ) {
	if ( mFlags & UI_REPORT_SIZE_CHANGE_TO_CHILDS )	{
		cUIControl * ChildLoop = mChild;

		while( NULL != ChildLoop ) {
			ChildLoop->OnParentSizeChange( SizeChange );
			ChildLoop = ChildLoop->mNext;
		}
	}
}

void cUIControl::OnParentSizeChange( const eeVector2i& SizeChange ) {
	SendCommonEvent( cUIEvent::EventOnParentSizeChange );
}

eeSize cUIControl::GetSkinSize( cUISkin * Skin, const Uint32& State ) {
	eeSize		tSize;

	if ( NULL != Skin ) {
		cSubTexture * tSubTexture = Skin->GetSubTexture( State );

		if ( NULL != tSubTexture ) {
			tSize = tSubTexture->RealSize();
		}

		if ( Skin->GetType() == cUISkin::UISkinComplex ) {
			cUISkinComplex * SkinC = reinterpret_cast<cUISkinComplex*> ( Skin );

			tSubTexture = SkinC->GetSubTextureSide( State, cUISkinComplex::Up );

			if ( NULL != tSubTexture ) {
				tSize.y += tSubTexture->RealSize().Height();
			}

			tSubTexture = SkinC->GetSubTextureSide( State, cUISkinComplex::Down );

			if ( NULL != tSubTexture ) {
				tSize.y += tSubTexture->RealSize().Height();
			}

			tSubTexture = SkinC->GetSubTextureSide( State, cUISkinComplex::Left );

			if ( NULL != tSubTexture ) {
				tSize.x += tSubTexture->RealSize().Width();
			}

			tSubTexture = SkinC->GetSubTextureSide( State, cUISkinComplex::Right );

			if ( NULL != tSubTexture ) {
				tSize.x += tSubTexture->RealSize().Width();
			}
		}
	}

	return tSize;
}

eeSize cUIControl::GetSkinSize() {
	return GetSkinSize( GetSkin(), cUISkinState::StateNormal );
}

cUIControl * cUIControl::NextComplexControl() {
	cUIControl * Found		= NULL;
	cUIControl * ChildLoop	= mChild;

	while( NULL != ChildLoop ) {
		if ( ChildLoop->Visible() && ChildLoop->Enabled() ) {
			if ( ChildLoop->IsComplex() ) {
				return ChildLoop;
			} else {
				Found = ChildLoop->NextComplexControl();

				if ( NULL != Found ) {
					return Found;
				}
			}
		}

		ChildLoop = ChildLoop->mNext;
	}

	if ( NULL != mNext ) {
		if ( mNext->Visible() && mNext->Enabled() && mNext->IsComplex() ) {
			return mNext;
		} else {
			return mNext->NextComplexControl();
		}
	} else {
		ChildLoop = mParentCtrl;

		while ( NULL != ChildLoop ) {
			if ( NULL != ChildLoop->mNext ) {
				if ( ChildLoop->mNext->Visible() && ChildLoop->mNext->Enabled() && ChildLoop->mNext->IsComplex() ) {
					return ChildLoop->mNext;
				} else {
					return ChildLoop->mNext->NextComplexControl();
				}
			}

			ChildLoop = ChildLoop->mParentCtrl;
		}
	}

	return cUIManager::instance()->MainControl();
}

void cUIControl::DoAfterSetTheme() {
}

void cUIControl::WorldToControl( eeVector2i& pos ) const {
	eeVector2f Pos( pos.x, pos.y );

	std::list<cUIControl*> parents;

	cUIControl * ParentLoop = mParentCtrl;

	while ( NULL != ParentLoop ) {
		parents.push_front( ParentLoop );
		ParentLoop = ParentLoop->Parent();
	}

	parents.push_back( const_cast<cUIControl*>( reinterpret_cast<const cUIControl*>( this ) ) );

	eeVector2f scale(1,1);

	for ( std::list<cUIControl*>::iterator it = parents.begin(); it != parents.end(); it++ ) {
		cUIControl * tParent	= (*it);
		cUIControlAnim * tP		= tParent->IsAnimated() ? reinterpret_cast<cUIControlAnim *> ( tParent ) : NULL;
		eeVector2f pPos			( tParent->mPos.x * scale.x			, tParent->mPos.y * scale.y			);
		eeVector2f Center		( tParent->mSize.x * 0.5f * scale.x	, tParent->mSize.y * 0.5f * scale.y	);

		if ( NULL != tP && 1.f != tP->Scale() ) {
			scale *= tP->Scale();

			pPos.Scale( scale, pPos + Center );
		}

		Pos -= pPos;

		if ( NULL != tP && 0.f != tP->Angle() ) {
			Center = eeVector2f( tParent->mSize.x * 0.5f * scale.x	, tParent->mSize.y * 0.5f * scale.y	);
			Pos.Rotate( -tP->Angle(), Center );
		}
	}

	pos = eeVector2i( Pos.x / scale.x, Pos.y / scale.y );
}

void cUIControl::ControlToWorld( eeVector2i& pos ) const {
	eeVector2f Pos( pos.x, pos.y );

	std::list<cUIControl*> parents;

	cUIControl * ParentLoop = mParentCtrl;

	while ( NULL != ParentLoop ) {
		parents.push_back( ParentLoop );
		ParentLoop = ParentLoop->Parent();
	}

	parents.push_front( const_cast<cUIControl*>( reinterpret_cast<const cUIControl*>( this ) ) );

	for ( std::list<cUIControl*>::iterator it = parents.begin(); it != parents.end(); it++ ) {
		cUIControl * tParent	= (*it);
		cUIControlAnim * tP		= tParent->IsAnimated() ? reinterpret_cast<cUIControlAnim *> ( tParent ) : NULL;
		eeVector2f pPos			( tParent->mPos.x					, tParent->mPos.y					);
		eeVector2f Center		( pPos.x + tParent->mSize.x * 0.5f	, pPos.y + tParent->mSize.y	* 0.5f	);

		Pos += pPos;

		if ( NULL != tP ) {
			Pos.Rotate( tP->Angle(), Center );
			Pos.Scale( tP->Scale(), Center );
		}
	}

	pos = eeVector2i( eeceil( Pos.x ), eeceil( Pos.y ) );
}

}}
