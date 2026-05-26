#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "TileMap.h"
#include "Pathfinding.h"
class Enemy : public Entity {
public:
    Enemy(float x, float y);
    void update(float dt, sf::Vector2f playerPos, const TileMap& map);
    void draw(sf::RenderWindow& window);
    bool isAlive() const { return mHP > 0.f; }
    void takeDamage(float dmg);
    sf::Vector2f  getPosition() const { return mSprite.getPosition(); }
    sf::FloatRect getBounds()   const {
        auto p = mSprite.getPosition();
        return sf::FloatRect(p.x - HALF_SIZE, p.y - HALF_SIZE, 28.f, 28.f);
    }
private:
    bool collidesWithWall(sf::Vector2f pos, const TileMap& map);
    void moveWithCollision(float dx, float dy, const TileMap& map);
    void updateSpriteScale();
    sf::Sprite   mSprite;
    sf::Vector2f mVelocity;
    bool         mFacingLeft = false;
    static constexpr float HALF_SIZE = 14.f;
    static constexpr float MAX_HP = 100.f;
    static constexpr float SPEED = 90.f;
    static constexpr float DETECT_DIST = 280.f;
    static constexpr float PATH_RATE = 0.3f;
    static constexpr float PATROL_TIME = 2.f;
    float mHP = MAX_HP;
    enum class State { Patrol, Chase };
    State mState = State::Patrol;
    sf::Vector2f mPatrolDir = { 1.f, 0.f };
    float        mPatrolTimer = PATROL_TIME;
    std::vector<sf::Vector2f> mPath;
    int   mPathIdx = 0;
    float mPathTimer = 0.f;
    float mHitFlash = 0.f;
};