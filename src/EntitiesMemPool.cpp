#include "EntitiesMemPool.h"


EntitiesMemPool::EntitiesMemPool(size_t maxEntities) {
  std::get<std::vector<CTransform>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CAABB>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CAnimation>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CLifespan>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CInput>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CDamage>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CInvincibility>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CHP>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CFollowAI>>(componentspool).reserve(MAX_ENTITIES);
  std::get<std::vector<CPatrolAI>>(componentspool).reserve(MAX_ENTITIES);
  tags.reserve(MAX_ENTITIES);
  isAlive.reserve(MAX_ENTITIES);
}

void EntitiesMemPool::stripTag(size_t aEntityId) {
  tags[aEntityId] = EDEFAULT;
}

void EntitiesMemPool::destroy(size_t aEntityId) {
  isAlive[aEntityId] = false;
}

size_t EntitiesMemPool::addEntity(EntityTag tag) {
  for (size_t i = 0; i < isAlive.size(); i++) {
    if (isAlive[i] == false) {
      if (tags[i] == EDEFAULT) {
        std::get<std::vector<CTransform>>(componentspool)[i] = CTransform();
        std::get<std::vector<CAABB>>(componentspool)[i] = CAABB();
        std::get<std::vector<CAnimation>>(componentspool)[i] = CAnimation();
        std::get<std::vector<CLifespan>>(componentspool)[i] = CLifespan();
        std::get<std::vector<CInput>>(componentspool)[i] = CInput();
        std::get<std::vector<CDamage>>(componentspool)[i] = CDamage();
        std::get<std::vector<CInvincibility>>(componentspool)[i] = CInvincibility();
        std::get<std::vector<CHP>>(componentspool)[i] = CHP();
        std::get<std::vector<CFollowAI>>(componentspool)[i] = CFollowAI();
        std::get<std::vector<CPatrolAI>>(componentspool)[i] = CPatrolAI();
        tags[i] = tag;
        isAlive[i] = true;
        return i;
      }
    }
  }
  std::get<std::vector<CTransform>>(componentspool).push_back(CTransform());
  std::get<std::vector<CAABB>>(componentspool).push_back(CAABB());
  std::get<std::vector<CAnimation>>(componentspool).push_back(CAnimation());
  std::get<std::vector<CLifespan>>(componentspool).push_back(CLifespan());
  std::get<std::vector<CInput>>(componentspool).push_back(CInput());
  std::get<std::vector<CDamage>>(componentspool).push_back(CDamage());
  std::get<std::vector<CInvincibility>>(componentspool).push_back(CInvincibility());
  std::get<std::vector<CHP>>(componentspool).push_back(CHP());
  std::get<std::vector<CFollowAI>>(componentspool).push_back(CFollowAI());
  std::get<std::vector<CPatrolAI>>(componentspool).push_back(CPatrolAI());
  tags.push_back(tag);
  isAlive.push_back(true);
  std::cout << "Didnt fit addr: " << isAlive.size() - 1 << "\n";
  return isAlive.size() - 1;
}

bool EntitiesMemPool::alive(size_t aEntityId) {
  return isAlive[aEntityId];
}

EntityTag EntitiesMemPool::tag(size_t aEntityId) {
  return tags[aEntityId];
}
