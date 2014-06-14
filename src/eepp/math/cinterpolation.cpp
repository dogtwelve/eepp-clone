#include <eepp/math/cinterpolation.hpp>
#include <eepp/math/easing.hpp>
using namespace EE::Math::easing;

namespace EE { namespace Math {

cInterpolation::cInterpolation() :
	mType(Ease::Linear),
	mEnable(false),
	mUpdate(true),
	mLoop(false),
	mEnded(false),
	mTotDist(0.f),
	mCurPos(0.f),
	mCurPoint(0),
	mCurTime(0),
	mSpeed(1.3f),
	mActP(NULL),
	mNexP(NULL),
	mOnPathEndCallback(NULL),
	mOnStepCallback(NULL)
{
}

cInterpolation::~cInterpolation() {
}

void cInterpolation::Start( OnPathEndCallback PathEndCallback, OnStepCallback StepCallback) {
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

void cInterpolation::Stop() {
	mEnable	= false;
}

void cInterpolation::SetPathEndCallback( OnPathEndCallback PathEndCallback ) {
	mOnPathEndCallback = PathEndCallback;
}

void cInterpolation::SetStepCallback( OnStepCallback StepCallback ) {
	mOnStepCallback = StepCallback;
}

void cInterpolation::Reset() {
	mTotDist = 0.f;
	mActP = mNexP = NULL;
	mEnable	= false;
	mCurPoint = 0;
	mUpdate	= true;
	mEnded = false;
	mCurTime = 0;
	mOnPathEndCallback = NULL;

	if ( mPoints.size() )
		mCurPos = mPoints[0].p;
}

void cInterpolation::ClearWaypoints() {
	Reset();
	mPoints.clear();
}

void cInterpolation::AddWaypoint( const Float Pos, const Float Time ) {
	mPoints.push_back( cPoint1df( Pos, Time ) );

	if ( mPoints.size() >= 2 )
		mTotDist += eeabs( mPoints[ mPoints.size() - 1 ].p - mPoints[ mPoints.size() - 2 ].p );
}

bool cInterpolation::EditWaypoint( const unsigned int& PointNum, const Float& NewPos, const Float NewTime ) {
	if ( PointNum < mPoints.size() ) {
		if ( 0 == PointNum )
			mTotDist -= eeabs( mPoints[ PointNum ].p - mPoints[ PointNum + 1 ].p );
		else
			mTotDist -= eeabs( mPoints[ PointNum ].p - mPoints[ PointNum - 1 ].p );

		mPoints[ PointNum ] = cPoint1df( NewPos, NewTime );

		if ( 0 == PointNum ) {
			if ( PointNum + 1 < mPoints.size() )
				mTotDist += eeabs( mPoints[ PointNum ].p - mPoints[ PointNum + 1 ].p );
		}
		else
			mTotDist += eeabs( mPoints[ PointNum ].p - mPoints[ PointNum - 1 ].p );

		return true;
	}
	return false;
}

bool cInterpolation::EraseWaypoint( const unsigned int& PointNum ) {
	if ( PointNum < mPoints.size() && !mEnable ) {
		if ( 0 == PointNum )
			mTotDist -= eeabs( mPoints[ PointNum ].p - mPoints[ PointNum + 1 ].p );
		else
			mTotDist -= eeabs( mPoints[ PointNum ].p - mPoints[ PointNum - 1 ].p );

		mPoints.erase( mPoints.begin() + PointNum );

		return true;
	}
	return false;
}

const Float& cInterpolation::GetEndPos() {
	return mPoints[ mPoints.size() - 1 ].p;
}

const Float& cInterpolation::GetPos() {
	return mCurPos;
}

const Float& cInterpolation::GetRealPos() const {
	return mCurPos;
}

void cInterpolation::Update( const Time& Elapsed ) {
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

		mCurPos = easingCb[ mType ]( mCurTime, mActP->p, ( mNexP->p - mActP->p ), mActP->t );

		if ( mCurTime >= mActP->t ) {
			mCurPos = mNexP->p;

			mUpdate = true;

			mCurPoint++;

			if ( mCurPoint == mPoints.size() && mLoop )
				mCurPoint = 0;
		}
	}
}

void cInterpolation::SetTotalTime( const Time & TotTime ) {
	Float tdist = mTotDist;

	if ( tdist == 0.0f ) {
		mPoints.clear();
		return;
	}

	if ( mLoop ) {
		tdist += eeabs( mPoints[ mPoints.size() - 1 ].p - mPoints[0].p );
		mPoints[ mPoints.size() - 1 ].t = eeabs( mPoints[ mPoints.size() - 1 ].p - mPoints[0].p ) * TotTime.AsMilliseconds() / tdist;
	}

	for ( unsigned int i = 0; i < mPoints.size() - 1; i++) {
		Float CurDist = eeabs( mPoints[i].p - mPoints[i + 1].p );
		mPoints[i].t = CurDist * TotTime.AsMilliseconds() / tdist;
	}
}

void cInterpolation::Speed( const Float Speed ) {
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
			CurDist = eeabs( mPoints[ mPoints.size() - 1 ].p - mPoints[0].p );
			tdist += CurDist;

			mPoints[ mPoints.size() - 1 ].t = CurDist * TotTime / tdist;
			TotTime = tdist * ( 1000.f / mSpeed );
		}

		for ( unsigned int i = 0; i < mPoints.size() - 1; i++) {
			CurDist = eeabs( mPoints[i].p - mPoints[i + 1].p );
			mPoints[i].t = CurDist * TotTime / tdist;
		}
	}
}

void cInterpolation::Type( Ease::Interpolation InterpolationType ) {
	mType = InterpolationType;
}

const int& cInterpolation::Type() const {
	return mType;
}

const bool& cInterpolation::Loop() const {
	return mLoop;
}

void cInterpolation::Loop( const bool& loop ) {
	mLoop = loop;
}

const bool& cInterpolation::Ended() const {
	return mEnded;
}

cPoint1df * cInterpolation::GetCurrentActual() const {
	return mActP;
}

cPoint1df * cInterpolation::GetCurrentNext() const {
	return mNexP;
}

const Uint32& cInterpolation::GetCurrentPos() const {
	return mCurPoint;
}

const std::vector<cPoint1df>& cInterpolation::GetPoints() const {
	return mPoints;
}

const Float& cInterpolation::Speed() const {
	return mSpeed;
}

const bool& cInterpolation::Enabled() const {
	return mEnable;
}

void cInterpolation::Enabled( const bool& Enabled ) {
	mEnable = Enabled;
}

}}
