#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Projectile.h"
enum class WeaponType {
    Pistol = 0,
    Shotgun,
    SMG,
    Sniper,
    Laser,
    Rocket
};
struct WeaponStats {
    std::string name;
    float       damage;
    float       cooldown;
    float       bulletSpeed;
    int         bulletsPerShot;
    float       spreadDegrees;
    sf::Color   bulletColor;
    bool        piercing;
    bool        explosive;
    float       explosionRadius;
};
class Weapon {
public:
    Weapon(WeaponType type = WeaponType::Pistol);
    std::vector<Projectile> fire(sf::Vector2f from, sf::Vector2f target);
    void update(float dt);
    bool canFire() const { return mCooldown <= 0.f; }
    WeaponType         getType()  const { return mType; }
    const WeaponStats& getStats() const { return mStats; }
    float              getCooldownProgress() const;
    static WeaponStats getStats(WeaponType type);
    static std::string getName(WeaponType type) { return getStats(type).name; }
    static sf::Color   getColor(WeaponType type) { return getStats(type).bulletColor; }
private:
    WeaponType  mType;
    WeaponStats mStats;
    float       mCooldown = 0.f;
};