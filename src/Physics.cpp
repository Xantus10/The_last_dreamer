#include "Physics.h"

Vec2 getOverlap(Entity e1, Entity e2) {
  Vec2 overlapVec;
  if (!(e1.hasComponent<CTransform>() && e2.hasComponent<CTransform>() && e1.hasComponent<CAABB>() && e2.hasComponent<CAABB>())) {
    overlapVec = Vec2(0.0, 0.0);
  } else {
    double xDist = abs(e1.getComponent<CTransform>().pos.x - e2.getComponent<CTransform>().pos.x);
    double yDist = abs(e1.getComponent<CTransform>().pos.y - e2.getComponent<CTransform>().pos.y);
    double overlapX = ((e1.getComponent<CAABB>().halfSize.x) + (e2.getComponent<CAABB>().halfSize.x)) - xDist;
    double overlapY = ((e1.getComponent<CAABB>().halfSize.y) + (e2.getComponent<CAABB>().halfSize.y)) - yDist;
    overlapVec = Vec2(overlapX, overlapY);
  }
  return overlapVec;
}

Vec2 getPreviousOverlap(Entity e1, Entity e2) {
  Vec2 overlapVec;
  if (!(e1.hasComponent<CTransform>() && e2.hasComponent<CTransform>() && e1.hasComponent<CAABB>() && e2.hasComponent<CAABB>())) {
    overlapVec = Vec2(0.0, 0.0);
  }
  else {
    double xDist = abs(e1.getComponent<CTransform>().previousPos.x - e2.getComponent<CTransform>().previousPos.x);
    double yDist = abs(e1.getComponent<CTransform>().previousPos.y - e2.getComponent<CTransform>().previousPos.y);
    double overlapX = ((e1.getComponent<CAABB>().halfSize.x) + (e2.getComponent<CAABB>().halfSize.x)) - xDist;
    double overlapY = ((e1.getComponent<CAABB>().halfSize.y) + (e2.getComponent<CAABB>().halfSize.y)) - yDist;
    overlapVec = Vec2(overlapX, overlapY);
  }
  return overlapVec;
}

// Is a point inside entity's AABB
bool isInside(const Vec2& pos, Entity e) {
  CTransform& eTransform = e.getComponent<CTransform>();
  CAABB& eAABB = e.getComponent<CAABB>();
  return (pos.x > (eTransform.pos.x - eAABB.halfSize.x) && pos.x < (eTransform.pos.x + eAABB.halfSize.x)) &&
    (pos.y > (eTransform.pos.y - eAABB.halfSize.y) && pos.y < (eTransform.pos.y + eAABB.halfSize.y));
}

bool isBetween(const Vec2& pos, Vec2 startpos, Vec2 endpos) {
  return (pos.x > startpos.x && pos.x < endpos.x) && (pos.y > startpos.y && pos.y < endpos.y);
}

// Line intersection
Intersect lineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d) {
  Vec2 r = b - a;
  Vec2 s = d - c;
  double rxs = r.x * s.y - r.y * s.x;
  Vec2 cMinusA = c - a;
  double t = (cMinusA.x * s.y - cMinusA.y * s.x) / rxs;
  double u = (cMinusA.x * r.y - cMinusA.y * r.x) / rxs;
  return { (t >= 0 && t <= 1 && u >= 0 && u <= 1), Vec2(a.x + t*r.x, a.y + t*r.y) };
}

// Entity intersects a line
bool entityIntersect(const Vec2& a, const Vec2& b, Entity e) {
  CTransform& eTransform = e.getComponent<CTransform>();
  CAABB& eAABB = e.getComponent<CAABB>();
  Vec2 eCornerTL = Vec2(eTransform.pos.x - eAABB.halfSize.x, eTransform.pos.y - eAABB.halfSize.y);
  Vec2 eCornerTR = Vec2(eTransform.pos.x + eAABB.halfSize.x, eTransform.pos.y - eAABB.halfSize.y);
  Vec2 eCornerBR = Vec2(eTransform.pos.x + eAABB.halfSize.x, eTransform.pos.y + eAABB.halfSize.y);
  Vec2 eCornerBL = Vec2(eTransform.pos.x - eAABB.halfSize.x, eTransform.pos.y + eAABB.halfSize.y);
  
  return lineIntersect(a, b, eCornerTL, eCornerTR).isIntersect ||
    lineIntersect(a, b, eCornerTR, eCornerBR).isIntersect ||
    lineIntersect(a, b, eCornerBR, eCornerBL).isIntersect ||
    lineIntersect(a, b, eCornerBL, eCornerTL).isIntersect;
}
