#include <eepp/ui/cuiskinstate.hpp>
#include <eepp/ui/cuiskin.hpp>

namespace EE { namespace UI {

cUISkinState::cUISkinState( cUISkin * Skin ) :
	mSkin( Skin ),
	mCurState(0),
	mLastState(0)
{
	eeASSERT( NULL != mSkin );
}

cUISkinState::~cUISkinState() {
}

void cUISkinState::Draw( const eeFloat& X, const eeFloat& Y, const eeFloat& Width, const eeFloat& Height, const Uint32& Alpha ) {
	if ( NULL != mSkin )
		mSkin->Draw( X, Y, Width, Height, Alpha, mCurState );
}

void cUISkinState::StateBack( const Uint32& State ) {
	if ( ( StateFocus == State ) && StateMouseEnter == mCurState && StateNormal == mLastState ) {
		return;
	}

	if ( mCurState == StateSelected && ( State == StateMouseDown || State == StateFocus ) ) {
		return;
	}

	if ( !( mCurState == StateFocus && ( State == StateMouseEnter || State == StateMouseExit || State == StateMouseDown ) ) ) {
		mLastState 	= mCurState;
		mCurState 	= StateNormal;
	}
}

void cUISkinState::SetPrevState() {
	if ( StateMouseDown == mCurState ) {
		Uint32 State = mCurState;
		mCurState = mLastState;
		mLastState = State;
	}
}

const Uint32& cUISkinState::GetPrevState() const {
	return mLastState;
}

const Uint32& cUISkinState::GetState() const {
	return mCurState;
}

void cUISkinState::SetState( const Uint32& State ) {
	eeASSERT ( State < cUISkinState::StateCount );

	switch ( mSkin->GetType() )
	{
		case cUISkin::UISkinSimple:
		case cUISkin::UISkinComplex:
			SetStateTypeSimple( State );
			break;
		default:
			SetStateTypeDefault( State );
	}
}

void cUISkinState::SetStateTypeSimple( const Uint32& State ) {
	eeASSERT ( State < cUISkinState::StateCount );

	if ( mCurState == State )
		return;

	if ( !mSkin->GetColorDefault( State ) || NULL != mSkin->GetSubTexture( State ) ) {
		mSkin->StateNormalToState( State );

		mLastState	= mCurState;
		mCurState	= State;
	} else
		StateBack( State );
}

void cUISkinState::SetStateTypeDefault( const Uint32& State ) {
	eeASSERT ( State < cUISkinState::StateCount );

	if ( !mSkin->GetColorDefault( State ) )
		mCurState = State;
	else
		StateBack( State );
}

cUISkin * cUISkinState::GetSkin() const {
	return mSkin;
}

bool cUISkinState::StateExists( const Uint32& State ) {
	switch ( mSkin->GetType() )
	{
		case cUISkin::UISkinSimple:
		case cUISkin::UISkinComplex:
			return NULL != mSkin->GetSubTexture( State );
	}

	return true;
}

}}

