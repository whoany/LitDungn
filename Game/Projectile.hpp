#pragma once
#include <SFML/Graphics.hpp>
#include "TileMap.hpp"

class Projectile {
public:
    Projectile(sf::Vector2f startPos, sf::Vector2f direction);

    // РќР°СЃС‚СЂРѕРёС‚СЊ РїР°СЂР°РјРµС‚СЂС‹ РїСѓР»Рё (РІС‹Р·С‹РІР°РµС‚ Weapon)
    void configure(float speed, float damage, sf::Color color,
                   bool piercing, bool explosive, float explRadius);

    void update(float dt, const TileMap& map);
    void draw(sf::RenderWindow& window);

    bool isAlive()    const { return mAlive; }
    void kill()              { mAlive = false; }
    float getDamage() const { return mDamage; }
    bool  isPiercing()  const { return mPiercing; }
    bool  isExplosive() const { return mExplosive; }
    float getExplosionRadius() const { return mExplRadius; }

    sf::Vector2f getPosition() const { return mShape.getPosition(); }
    sf::FloatRect getBounds()  const { return mShape.getGlobalBounds(); }

private:
    sf::CircleShape mShape;
    sf::Vector2f    mVelocity;
    sf::Vector2f    mDirection;
    bool   mAlive      = true;
    float  mLifetime   = 2.f;
    float  mDamage     = 25.f;
    bool   mPiercing   = false;
    bool   mExplosive  = false;
    float  mExplRadius = 0.f;

    static constexpr float RADIUS = 5.f;
};
