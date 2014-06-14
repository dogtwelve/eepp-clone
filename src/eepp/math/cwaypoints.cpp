#include <eepp/math/cwaypoints.hpp>
#include <eepp/math/easing.hpp>
using namespace EE::Math::easing;

namespace EE { namespace Math {

cWaypoints::cWaypoints() :
	mType(Ease::Linear),
	mEnable(false),
	mUpdate(true),
	mLoop(false),
	mEnded(false),
	mTotDist(0.f),
	mCurPoint(0),
	mCurTime(0.0f),
	mSpeed(1.3f),
	mOnPathEndCallback(NULL),
	mOnStepCallback(NULL)
{
}

cWaypoints::~cWaypoints() {
}

void cWaypoints::Start( OnPathEndCallback PathEndCallback, OnStepCallback StepCallback ) {
	mEnable				= true;
	mOnPathEndCallback	= PathEndCallback;
	mOnStepCallback		= StepCallback;

	if ( mPoints.size() ) {
		mActP = &mPoints[ 0 ];

		if ( mPoints.size() > 1 )
			mNexP = &mPoints[ 1 ];

		mCurPos = mPoints[0].p;
	} else {
		mEnable = false;
	}
}

void cWaypoints::Stop() {
	mEnable = false;
}

void cWaypoints::SetPathEndCallback( OnPathEndCallback PathEndCallback ) {
	mOnPathEndCallback = PathEndCallback;
}

void cWaypoints::SetStepCallback( OnStepCallback StepCallback ) {
	mOnStepCallback = StepCallback;
}

void cWaypoints::Reset() {
	mTotDist = 0.f;
	mEnable = false;
	mCurPoint = 0;
	mCurTime = 0;
	mUpdate = true;
	mEnded = false;

	if ( mPoints.size() )
		mCurPos = mPoints[0].p;
}

void cWaypoints::ClearWaypoints() {
	Reset();
	mPoints.clear();
}

void cWaypoints::AddWaypoint( const eeVector2f& Pos, const Float& Time ) {
	mPoints.push_back( cWaypoint(Pos, Time) );

	if ( mPoints.size() >= 2 )
	{
		mTotDist += mPoints[ mPoints.size() - 1 ].p.Distance( mPoints[ mPoints.size() - 2 ].p );
	}
}

bool cWaypoints::EditWaypoint( const unsigned int& PointNum, const eeVector2f& NewPos, const Float& NewTime  ) {
	if ( PointNum < mPoints.size() ) {
		if ( 0 == PointNum )
			mTotDist -= mPoints[ PointNum ].p.Distance( mPoints[ PointNum + 1 ].p );
		else
			mTotDist -= mPoints[ PointNum ].p.Distance( mPoints[ PointNum - 1 ].p );

		mPoints[ PointNum ] = cWaypoint( NewPos, NewTime );

		if ( 0 == PointNum ) {
			if ( PointNum + (unsigned int)1 < mPoints.size() )
				mTotDist += mPoints[ PointNum ].p.Distance( mPoints[ PointNum + 1 ].p );
		} else
			mTotDist += mPoints[ PointNum ].p.Distance( mPoints[ PointNum - 1 ].p );

		return true;
	}
	return false;
}

bool cWaypoints::EraseWaypoint( const unsigned int& PointNum ) {
	if ( PointNum < mPoints.size() && !mEnable ) {
		if ( 0 == PointNum )
			mTotDist -= mPoints[ PointNum ].p.Distance( mPoints[ PointNum + 1 ].p );
		else
			mTotDist -= mPoints[ PointNum ].p.Distance( mPoints[ PointNum - 1 ].p );

		mPoints.erase( mPoints.begin() + PointNum );

		return true;
	}
	return false;
}

void cWaypoints::Speed( const Float& Speed ) {
	Float tdist = mTotDist;
	mSpeed = Speed;
	Float CurDist;

	if ( mPoints.size() ) {
		if ( tdist == 0.0f ) {
			mPoints.clear();
			return;
		}

		Float TotTime = tdist * ( 1000.f / mSpeed );

		if ( mLoop ) {
			CurDist = mPoints[ mPoints.size() - 1 ].p.Distance( mPoints[0].p );
			tdist += CurDist;

			mPoints[ mPoints.size() - 1 ].t = CurDist * TotTime / tdist;
			TotTime = tdist * ( 1000.f / mSpeed );
		}

		for ( unsigned int i = 0; i < mPoints.size() - 1; i++) {
			CurDist = mPoints[i].p.Distance( mPoints[i + 1].p );
			mPoints[i].t = CurDist * TotTime / tdist;
		}
	}
}

const eeVector2f& cWaypoints::GetPos() {
	return mCurPos;
}

void cWaypoints::Update( const cTime& Elapsed ) {
	if ( mEnable && mPoints.size() > 1 && mCurPoint != mPoints.size() ) {
		if ( mUpdate ) {
			mCurTime = 0;
			mActP = &mPoints[ mCurPoint ];

			if ( mCurPoint + 1 < mPoints.size() ) {
				mNexP = &mPoints[ mCurPoint + 1 ];

				if ( mOnStepCallback.IsSet() )
					mOnStepCallback();
			} else {
				if ( mOnStepCallback.IsSet() )
					mOnStepCallback();

				if ( mLoop ) {
					mNexP = &mPoints[ 0 ];

					if ( mOnPathEndCallback.IsSet() )
						mOnPathEndCallback();
				} else {
					mEnable = false;
					mEnded = true;

					if ( mOnPathEndCallback.IsSet() ) {
						mOnPathEndCallback();
						mOnPathEndCallback.Reset();
					}
					return;
				}
			}
			mUpdate = false;
		}

		mCurTime += Elapsed.AsMilliseconds();

		mCurPos.x = easingCb[ mType ]( mCurTime, mActP->p.x, ( mNexP->p.x - mActP->p.x ), mActP->t );
		mCurPos.y = easingCb[ mType ]( mCurTime, mActP->p.y, ( mNexP->p.y - mActP->p.y ), mActP->t );

		if ( mCurTime >= mActP->t ) {
			mCurPos = mNexP->p;

			mUpdate = true;

			mCurPoint++;

			if ( mCurPoint == mPoints.size() && mLoop )
				mCurPoint = 0;
		}
	}
}

void cWaypoints::SetTotalTime( const cTime& TotTime ) {
	unsigned int i;
	Float tdist = mTotDist;

	if ( !mPoints.size() )
		return;

	if ( tdist == 0.0f ) {
		mPoints.clear();
		return;
	}

	if ( mLoop ) {
		tdist += mPoints[ mPoints.size() - 1 ].p.Distance( mPoints[0].p );
		mPoints[ mPoints.size() - 1 ].t = mPoints[ mPoints.size() - 1 ].p.Distance( mPoints[0].p ) * TotTime.AsMilliseconds() / tdist;
	}

	for (i = 0; i < mPoints.size() - 1; i++)
		mPoints[i].t = mPoints[i].p.Distance( mPoints[i + 1].p ) * TotTime.AsMilliseconds() / tdist;
}

void cWaypoints::Type( Ease::Interpolation InterpolationType ) {
	mType = InterpolationType;
}

const int& cWaypoints::Type() const {
	return mType;
}

bool cWaypoints::Loop() const {
	return mLoop;
}

void cWaypoints::Loop( const bool& loop ) {
	mLoop = loop;
}

bool cWaypoints::Ended() const {
	return mEnded;
}

cWaypoint * cWaypoints::GetCurrentActual() const {
	return mActP;
}

cWaypoint * cWaypoints::GetCurrentNext() const {
	return mNexP;
}

const Uint32& cWaypoints::GetCurrentPos() const {
	return mCurPoint;
}

const std::vector<cWaypoint>& cWaypoints::GetWaypoints() const {
	return mPoints;
}

const Float& cWaypoints::Speed() const {
	return mSpeed;
}

const bool& cWaypoints::Enabled() const {
	return mEnable;
}

void cWaypoints::Enabled( const bool& Enabled ) {
	mEnable = Enabled;
}

}}
