#pragma once
class Vec2 {
public:
  double x;
  double y;

  //Constructors
  Vec2();
  Vec2(double x, double y);

  bool operator == (const Vec2& rhs) const;
  bool operator != (const Vec2& rhs) const;

  Vec2 operator + (const Vec2& rhs) const;
  Vec2 operator - (const Vec2& rhs) const;
  Vec2 operator / (const double val) const;
  Vec2 operator * (const double val) const;

  void operator += (const Vec2& rhs);
  void operator -= (const Vec2& rhs);
  void operator /= (const double val);
  void operator *= (const double val);

  //Get distance between this and some other Vec2
  double dist(const Vec2& rhs) const;
  void invertX();
  void invertY();
  //Change the x and y of this Vec2 given length and angle
  void change(double aLen, double aAngle);
  //Normalize this Vec2 (Len=1)
  void normalize();
};
