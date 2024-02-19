#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
  : x(0) 
  , y(0) {}

Vec2::Vec2(double aX, double aY)
  : x(aX)
  , y(aY) {}


bool Vec2::operator == (const Vec2& rhs) const {
  return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const {
  return (x != rhs.x || y != rhs.y);
}


Vec2 Vec2::operator + (const Vec2& rhs) const {
  return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const {
  return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const double val) const {
  return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const double val) const {
  return Vec2(x * val, y * val);
}


void Vec2::operator += (const Vec2& rhs) {
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs) {
  x -= rhs.x;
  y -= rhs.y;
}

void Vec2::operator /= (const double val) {
  x /= val;
  y /= val;
}

void Vec2::operator *= (const double val) {
  x *= val;
  y *= val;
}


double Vec2::dist(const Vec2& rhs) const {
  Vec2 dVec = rhs - Vec2(x, y);
  return sqrtf(dVec.x * dVec.x + dVec.y * dVec.y);
}

void Vec2::invertX() {
  x *= -1;
}

void Vec2::invertY() {
  y *= -1;
}

void Vec2::change(double aLen, double aAngle) {
  x = cos(aAngle) * aLen;
  y = sin(aAngle) * aLen;
}

void Vec2::normalize() {
  double Len = dist(Vec2(0.0f, 0.0f));
  x /= Len;
  y /= Len;
}
