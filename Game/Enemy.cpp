#include "Enemy.h"
#include "Assets.h"
#include <cmath>
Enemy::Enemy(float x, float y) {
    mSprite.setTexture(Assets::instance().get("enemy"));
    auto sz = mSprite.getTexture()->getSize();
    mSprite.setOrigin(sz.x * 0.5f, sz.y * 0.5f);
    mSprite.setPosition(x, y);
    updateSpriteScale();
}
void Enemy::updateSpriteScale() {
    auto sz = mSprite.getTexture()->getSize();
    if (sz.x == 0 || sz.y == 0) return;
    float scaleX = 28.f / sz.x;
    float scaleY = 28.f / sz.y;
    mSprite.setScale(mFacingLeft ? -scaleX : scaleX, scaleY);
}
void Enemy::takeDamage(float dmg) {
    mHP -= dmg;
    mHitFlash = 0.15f;
}
void Enemy::moveWithCollision(float dx, float dy, const TileMap& map) {
    mSprite.move(dx, 0.f);
    if (collidesWithWall(mSprite.getPosition(), map))
        mSprite.move(-dx, 0.f);
    mSprite.move(0.f, dy);
    if (collidesWithWall(mSprite.getPosition(), map))
        mSprite.move(0.f, -dy);
}
void Enemy::update(float dt, sf::Vector2f playerPos,
    const TileMap& map) {
    if (!isAlive()) return;
    sf::Vector2f diff = playerPos - mSprite.getPosition();
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    mState = (dist < DETECT_DIST) ? State::Chase : State::Patrol;
    if (mState == State::Chase) {
        mPathTimer -= dt;
        if (mPathTimer <= 0.f) {
            mPathTimer = PATH_RATE;
            mPath = Pathfinding::findPath(
                mSprite.getPosition(), playerPos, map
            );
            mPathIdx = 0;
        }
        if (!mPath.empty() && mPathIdx < (int)mPath.size()) {
            sf::Vector2f target = mPath[mPathIdx];
            sf::Vector2f toTarget = target - mSprite.getPosition();
            float d = std::sqrt(toTarget.x * toTarget.x +
                toTarget.y * toTarget.y);
            if (d < 8.f)
                mPathIdx++;
            else
                mVelocity = (toTarget / d) * SPEED;
        }
        else if (mPath.empty()) {
            mVelocity = { 0.f, 0.f };
        }
    }
    else {
        mVelocity = mPatrolDir * (SPEED * 0.5f);
        mPatrolTimer -= dt;
        if (mPatrolTimer <= 0.f) {
            mPatrolDir.x *= -1.f;
            mPatrolTimer = PATROL_TIME;
        }
        sf::Vector2f nextPos = mSprite.getPosition();
        nextPos.x += mVelocity.x * dt;
        if (collidesWithWall(nextPos, map)) {
            mPatrolDir.x *= -1.f;
            mPatrolTimer = PATROL_TIME;
        }
    }
    if (mVelocity.x < -1.f) mFacingLeft = true;
    if (mVelocity.x > 1.f) mFacingLeft = false;
    moveWithCollision(mVelocity.x * dt, mVelocity.y * dt, map);
    updateSpriteScale();
    if (mHitFlash > 0.f) {
        mHitFlash -= dt;
        mSprite.setColor(sf::Color(255, 200, 200));
    }
    else {
        mSprite.setColor(sf::Color::White);
    }
}
void Enemy::draw(sf::RenderWindow& window) {
    if (!isAlive()) return;
    window.draw(mSprite);
    sf::Vector2f pos = mSprite.getPosition();
    float ratio = mHP / MAX_HP;
    sf::RectangleShape hpBg({ 28.f, 4.f });
    hpBg.setFillColor(sf::Color(80, 0, 0));
    hpBg.setOrigin(14.f, 22.f);
    hpBg.setPosition(pos);
    sf::RectangleShape hpFill({ 28.f * ratio, 4.f });
    hpFill.setFillColor(
        ratio > 0.5f ? sf::Color(0, 210, 0) : sf::Color(220, 60, 0)
    );
    hpFill.setOrigin(14.f, 22.f);
    hpFill.setPosition(pos);
    window.draw(hpBg);
    window.draw(hpFill);
}
bool Enemy::collidesWithWall(sf::Vector2f pos, const TileMap& map) {
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