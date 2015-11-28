#include <eepp/system/ctime.hpp>

namespace EE { namespace System {

const cTime cTime::Zero;

cTime::cTime() :
	mMicroseconds(0)
{
}

eeDouble cTime::AsSeconds() const {
	return mMicroseconds / 1000000.0;
}

eeDouble cTime::AsMilliseconds() const {
	return mMicroseconds / 1000.0;
}

Int64 cTime::AsMicroseconds() const {
	return mMicroseconds;
}

cTime::cTime(Int64 Microseconds) :
	mMicroseconds(Microseconds)
{
}

cTime Seconds(eeDouble amount) {
	return cTime(static_cast<Int64>(amount * 1000000));
}

cTime Milliseconds(eeDouble amount) {
	return cTime(static_cast<Int64>(amount) * 1000);
}

cTime Microseconds(Int64 amount) {
	return cTime(amount);
}

bool operator ==(cTime left, cTime right) {
	return left.AsMicroseconds() == right.AsMicroseconds();
}

bool operator !=(cTime left, cTime right) {
	return left.AsMicroseconds() != right.AsMicroseconds();
}

bool operator <(cTime left, cTime right) {
	return left.AsMicroseconds() < right.AsMicroseconds();
}

bool operator >(cTime left, cTime right) {
	return left.AsMicroseconds() > right.AsMicroseconds();
}

bool operator <=(cTime left, cTime right) {
	return left.AsMicroseconds() <= right.AsMicroseconds();
}

bool operator >=(cTime left, cTime right) {
	return left.AsMicroseconds() >= right.AsMicroseconds();
}

cTime operator -(cTime right) {
	return Microseconds(-right.AsMicroseconds());
}

cTime operator +(cTime left, cTime right) {
	return Microseconds(left.AsMicroseconds() + right.AsMicroseconds());
}

cTime& operator +=(cTime& left, cTime right) {
	return left = left + right;
}

cTime operator -(cTime left, cTime right) {
	return Microseconds(left.AsMicroseconds() - right.AsMicroseconds());
}

cTime& operator -=(cTime& left, cTime right) {
	return left = left - right;
}

cTime operator *(cTime left, cTime right) {
	return Microseconds(left.AsMicroseconds() * right.AsMicroseconds());
}

cTime operator *(cTime left, eeDouble right) {
	return Seconds(left.AsSeconds() * right);
}

cTime operator *(eeDouble left, cTime right) {
	return right * left;
}

cTime operator *(cTime left, Int64 right) {
	return Microseconds(left.AsMicroseconds() * right);
}

cTime operator *(Int64 left, cTime right) {
	return right * left;
}

cTime& operator *=(cTime& left, cTime right) {
	return left = left * right;
}

cTime& operator *=(cTime& left, eeDouble right) {
	return left = left * right;
}

cTime& operator *=(cTime& left, Int64 right) {
	return left = left * right;
}

cTime operator /(cTime left, cTime right) {
	return Microseconds(left.AsMicroseconds() / right.AsMicroseconds());
}

cTime operator /(cTime left, eeDouble right) {
	return Seconds(left.AsSeconds() / right);
}

cTime operator /(cTime left, Int64 right) {
	return Microseconds(left.AsMicroseconds() / right);
}

cTime& operator /=(cTime& left, cTime right) {
	return left = left / right;
}

cTime& operator /=(cTime& left, Int64 right) {
	return left = left / right;
}

cTime operator %(cTime left, cTime right) {
	return Microseconds(left.AsMicroseconds() % right.AsMicroseconds());
}

cTime& operator %=(cTime& left, cTime right) {
	return left = left % right;
}

}}
