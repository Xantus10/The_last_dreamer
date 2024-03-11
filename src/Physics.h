#pragma once
#include "Entity.h"

struct Intersect {
  bool isIntersect;
  Vec2 intersectPos;
};

// Get Vec2 symbolizing the overlap of two entities
Vec2 getOverlap(Entity e1, Entity e2);
// Get Vec2 symbolizing the overlap of two entities on the last frame
Vec2 getPreviousOverlap(Entity e1, Entity e2);
// Is a point inside entity's AABB
bool isInside(const Vec2& pos, Entity e);
bool isBetween(const Vec2& pos, Vec2 startpos, Vec2 endpos);
// Line intersection
Intersect lineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d);
// Entity intersects a line
bool entityIntersect(const Vec2& a, const Vec2& b, Entity e);
