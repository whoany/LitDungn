#include "Weapon.h"
#include <cmath>
Weapon::Weapon(WeaponType type) : mType(type), mStats(getStats(type)) {}
WeaponStats Weapon::getStats(WeaponType type) {
    WeaponStats s{};
    s.bulletColor = sf::Color(255, 220, 50);
    s.bulletsPerShot = 1;
    s.spreadDegrees = 0.f;
    s.bulletSpeed = 450.f;
    s.piercing = false;
    s.explosive = false;
    s.explosionRadius = 0.f;
    switch (type) {
    case WeaponType::Pistol:
        s.name = "Pistol";
        s.damage = 25.f;
        s.cooldown = 0.25f;
        break;
    case WeaponType::Shotgun:
        s.name = "Shotgun";
        s.damage = 18.f;
        s.cooldown = 0.7f;
        s.bulletsPerShot = 3;
        s.spreadDegrees = 18.f;
        s.bulletSpeed = 400.f;
        s.bulletColor = sf::Color(255, 140, 50);
        break;
    case WeaponType::SMG:
        s.name = "SMG";
        s.damage = 12.f;
        s.cooldown = 0.08f;
        s.bulletColor = sf::Color(120, 220, 255);
        break;
    case WeaponType::Sniper:
        s.name = "Sniper";
        s.damage = 100.f;
        s.cooldown = 1.5f;
        s.bulletSpeed = 800.f;
        s.bulletColor = sf::Color(255, 60, 60);
        s.piercing = true;
        break;
    case WeaponType::Laser:
        s.name = "Laser";
        s.damage = 15.f;
        s.cooldown = 0.05f;
        s.bulletSpeed = 900.f;
        s.bulletColor = sf::Color(180, 120, 255);
        s.piercing = true;
        break;
    case WeaponType::Rocket:
        s.name = "Rocket";
        s.damage = 60.f;
        s.cooldown = 1.2f;
        s.bulletSpeed = 350.f;
        s.bulletColor = sf::Color(255, 100, 0);
        s.explosive = true;
        s.explosionRadius = 80.f;
        break;
    }
    return s;
}
void Weapon::update(float dt) {
    if (mCooldown > 0.f) mCooldown -= dt;
}
float Weapon::getCooldownProgress() const {
    if (mStats.cooldown <= 0.f) return 1.f;
    float remaining = mCooldown / mStats.cooldown;
    return 1.f - std::max(0.f, std::min(1.f, remaining));
}
std::vector<Projectile> Weapon::fire(sf::Vector2f from,
    sf::Vector2f target) {
    std::vector<Projectile> bullets;
    if (mCooldown > 0.f) return bullets;
    mCooldown = mStats.cooldown;
    sf::Vector2f baseDir = target - from;
    float len = std::sqrt(baseDir.x * baseDir.x + baseDir.y * baseDir.y);
    if (len < 1.f) return bullets;
    baseDir /= len;
    float baseAngle = std::atan2(baseDir.y, baseDir.x);
    float spreadRad = mStats.spreadDegrees * 3.14159f / 180.f;
    for (int i = 0; i < mStats.bulletsPerShot; i++) {
        float angle = baseAngle;
        if (mStats.bulletsPerShot > 1) {
            float step = spreadRad / (mStats.bulletsPerShot - 1);
            angle = baseAngle - spreadRad * 0.5f + step * i;
        }
        sf::Vector2f dir = { std::cos(angle), std::sin(angle) };
        Projectile p(from, dir);
        p.configure(mStats.bulletSpeed, mStats.damage,
            mStats.bulletColor, mStats.piercing,
            mStats.explosive, mStats.explosionRadius);
        bullets.push_back(p);
    }
    return bullets;
}