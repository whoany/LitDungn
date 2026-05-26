#include "Player.h"
#include "Assets.h"
#include "Stats.h"
#include <cmath>
Player::Player() {
    mSprite.setTexture(Assets::instance().get("player"));
    auto sz = mSprite.getTexture()->getSize();
    mSprite.setOrigin(sz.x * 0.5f, sz.y * 0.5f);
    mSprite.setPosition(400.f, 300.f);
    updateSpriteScale();
    mWeapons.push_back(Weapon(WeaponType::Pistol));
}

void Player::updateSpriteScale() {
    auto sz = mSprite.getTexture()->getSize();
    if (sz.x == 0 || sz.y == 0) return;
    float scaleX = 28.f / sz.x;
    float scaleY = 28.f / sz.y;
    mSprite.setScale(mFacingLeft ? -scaleX : scaleX, scaleY);
}
void Player::switchWeapon(WeaponType type) {
    for (int i = 0; i < (int)mWeapons.size(); i++) {
        if (mWeapons[i].getType() == type) {
            mCurrentWeaponIdx = i;
            return;
        }
    }
}
void Player::addWeapon(WeaponType type) {
    for (auto& w : mWeapons)
        if (w.getType() == type) return;
    mWeapons.push_back(Weapon(type));
    mCurrentWeaponIdx = (int)mWeapons.size() - 1;
}
void Player::handleInput() {
    mVelocity = { 0.f, 0.f };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mVelocity.y = -SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mVelocity.y = SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        mVelocity.x = -SPEED;
        mFacingLeft = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        mVelocity.x = SPEED;
        mFacingLeft = false;
    }
    if (mVelocity.x != 0.f && mVelocity.y != 0.f) {
        mVelocity.x *= 0.7071f;
        mVelocity.y *= 0.7071f;
    }
}
bool Player::collidesWithWall(sf::Vector2f pos, const TileMap& map) {
    int ts = TileMap::TILE_SIZE;
    int l = (int)(pos.x - HALF_SIZE) / ts;
    int r = (int)(pos.x + HALF_SIZE - 1) / ts;
    int t = (int)(pos.y - HALF_SIZE) / ts;
    int b = (int)(pos.y + HALF_SIZE - 1) / ts;
    return map.getTile(l, t) == TileType::Wall ||
        map.getTile(r, t) == TileType::Wall ||
        map.getTile(l, b) == TileType::Wall ||
        map.getTile(r, b) == TileType::Wall;
}
void Player::takeDamage(float dmg) {
    if (dmg < 0.f) {
        mHP = std::min(mMaxHP, mHP - dmg);
        return;
    }
    if (mInvuln > 0.f) return;
    mHP = std::max(0.f, mHP - dmg);
    mInvuln = INVULN_TIME;
}
void Player::shoot(sf::Vector2f target) {
    Weapon& w = currentWeapon();
    auto bullets = w.fire(mSprite.getPosition(), target);
    if (!bullets.empty()) Stats::instance().addShot();
    for (auto& b : bullets)
        mBullets.push_back(b);
}
void Player::update(float dt, const TileMap& map) {
    handleInput();
    mSprite.move(mVelocity.x * dt, 0.f);
    if (collidesWithWall(mSprite.getPosition(), map))
        mSprite.move(-mVelocity.x * dt, 0.f);
    mSprite.move(0.f, mVelocity.y * dt);
    if (collidesWithWall(mSprite.getPosition(), map))
        mSprite.move(0.f, -mVelocity.y * dt);
    for (auto& w : mWeapons) w.update(dt);
    updateSpriteScale();
    if (mInvuln > 0.f) {
        mInvuln -= dt;
        bool flash = (int)(mInvuln * 10) % 2;
        mSprite.setColor(flash
            ? sf::Color(255, 255, 255, 100)
            : sf::Color::White);
    }
    else {
        mSprite.setColor(sf::Color::White);
    }
    for (auto& b : mBullets) b.update(dt, map);
    mBullets.erase(
        std::remove_if(mBullets.begin(), mBullets.end(),
            [](const Projectile& p) { return !p.isAlive(); }),
        mBullets.end()
    );
}
void Player::draw(sf::RenderWindow& window) {
    for (auto& b : mBullets) b.draw(window);
    window.draw(mSprite);
}