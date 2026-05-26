#include "Projectile.h"
Projectile::Projectile(sf::Vector2f startPos,
    sf::Vector2f direction) {
    mShape.setRadius(RADIUS);
    mShape.setOrigin(RADIUS, RADIUS);
    mShape.setFillColor(sf::Color(255, 220, 50));
    mShape.setPosition(startPos);
    mDirection = direction;
    mVelocity = direction * 450.f;
}
void Projectile::configure(float speed, float damage, sf::Color color,
    bool piercing, bool explosive, float explRadius) {
    mVelocity = mDirection * speed;
    mDamage = damage;
    mPiercing = piercing;
    mExplosive = explosive;
    mExplRadius = explRadius;
    mShape.setFillColor(color);
    if (explosive) {
        mShape.setRadius(RADIUS * 1.6f);
        mShape.setOrigin(RADIUS * 1.6f, RADIUS * 1.6f);
    }
}
void Projectile::update(float dt, const TileMap& map) {
    if (!mAlive) return;
    mShape.move(mVelocity * dt);
    sf::Vector2f pos = mShape.getPosition();
    if (!map.isFloor(pos.x, pos.y)) {
        mAlive = false;
        return;
    }
    mLifetime -= dt;
    if (mLifetime <= 0.f)
        mAlive = false;
}
void Projectile::draw(sf::RenderWindow& window) {
    if (!mAlive) return;
    window.draw(mShape);
}