#pragma once
#include <SFML/Graphics.hpp>
#include "Weapon.h"
enum class PickupType {
    HealthSmall,
    HealthLarge,
    CoinSmall,
    CoinLarge,
    WeaponDrop
};
class Pickup {
public:
    Pickup(sf::Vector2f pos, PickupType type,
        WeaponType weaponType = WeaponType::Pistol);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isAlive()           const { return mAlive; }
    void collect() { mAlive = false; }
    PickupType getType()     const { return mType; }
    WeaponType getWeapon()   const { return mWeaponType; }
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return mPos; }
private:
    sf::Vector2f mPos;
    PickupType   mType;
    WeaponType   mWeaponType;
    bool         mAlive = true;
    float        mLifetime = 30.f;
    float        mBobTimer = 0.f;
};