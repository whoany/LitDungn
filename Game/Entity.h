#pragma once

class Entity {
public:
    Entity() = default;
    float getHP()    const { return mHP; }
    float getMaxHP() const { return mMaxHP; }
    bool  isAlive()  const { return mHP > 0.f; }
protected:
    float mHP = 100.f;
    float mMaxHP = 100.f;
};