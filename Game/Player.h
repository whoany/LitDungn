#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "TileMap.h"
#include "Projectile.h"
#include "Weapon.h"
class Player : public Entity {
public:
    Player();
    void update(float dt, const TileMap& map);
    void draw(sf::RenderWindow& window);
    void shoot(sf::Vector2f target);
    void takeDamage(float dmg);
    void setPosition(sf::Vector2f pos) { mSprite.setPosition(pos); }
    void clearBullets() { mBullets.clear(); }
    void switchWeapon(WeaponType type);
    void addWeapon(WeaponType type);
    const Weapon& currentWeapon() const { return mWeapons[mCurrentWeaponIdx]; }
    Weapon& currentWeapon() { return mWeapons[mCurrentWeaponIdx]; }
    const std::vector<Weapon>& weapons() const { return mWeapons; }
    int           currentWeaponIdx() const { return mCurrentWeaponIdx; }
    sf::Vector2f  getPosition() const { return mSprite.getPosition(); }
    sf::FloatRect getBounds()   const {
        auto p = mSprite.getPosition();
        return sf::FloatRect(p.x - HALF_SIZE, p.y - HALF_SIZE, 28.f, 28.f);
    }
    float getHP()    const { return mHP; }
    float getMaxHP() const { return mMaxHP; }
    bool  isAlive()  const { return mHP > 0.f; }
    std::vector<Projectile>& getProjectiles() { return mBullets; }
private:
    void handleInput();
    bool collidesWithWall(sf::Vector2f pos, const TileMap& map);
    void updateSpriteScale();
    sf::Sprite   mSprite;
    sf::Vector2f mVelocity;
    bool         mFacingLeft = false;
    static constexpr float SPEED = 200.f;
    static constexpr float MAX_HP = 100.f;
    static constexpr float INVULN_TIME = 1.0f;
    static constexpr float HALF_SIZE = 14.f;
    float mHP = MAX_HP;
    float mMaxHP = MAX_HP;
    float mInvuln = 0.f;
    std::vector<Weapon>     mWeapons;
    int                     mCurrentWeaponIdx = 0;
    std::vector<Projectile> mBullets;
};