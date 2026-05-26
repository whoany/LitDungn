#include "Chest.h"
#include "Assets.h"
#include <cmath>
#include <cstdlib>
Chest::Chest(sf::Vector2f pos) : mPos(pos) {}
sf::FloatRect Chest::getBounds() const {
    return sf::FloatRect(mPos.x - 16.f, mPos.y - 14.f, 32.f, 28.f);
}
void Chest::update(float dt) {
    if (!mOpened) mGlowTime += dt;
}
std::vector<Pickup> Chest::open() {
    std::vector<Pickup> drops;
    if (mOpened) return drops;
    mOpened = true;
    WeaponType weapons[] = {
        WeaponType::Shotgun, WeaponType::SMG, WeaponType::Sniper,
        WeaponType::Laser,   WeaponType::Rocket
    };
    int wIdx = std::rand() % 5;
    drops.emplace_back(
        sf::Vector2f(mPos.x - 25.f, mPos.y),
        PickupType::WeaponDrop,
        weapons[wIdx]
    );
    int coins = 1 + std::rand() % 2;
    for (int i = 0; i < coins; i++) {
        float ox = (float)((std::rand() % 60) - 30);
        float oy = (float)((std::rand() % 40) - 20);
        bool large = (std::rand() % 3 == 0);
        drops.emplace_back(
            sf::Vector2f(mPos.x + ox, mPos.y + oy),
            large ? PickupType::CoinLarge : PickupType::CoinSmall
        );
    }
    if (std::rand() % 2 == 0) {
        bool big = (std::rand() % 3 == 0);
        drops.emplace_back(
            sf::Vector2f(mPos.x + 25.f, mPos.y + 10.f),
            big ? PickupType::HealthLarge : PickupType::HealthSmall
        );
    }
    return drops;
}
void Chest::draw(sf::RenderWindow& window) {
    auto& assets = Assets::instance();
    // Свечение под закрытым сундуком (анимация пульсирования)
    if (!mOpened) {
        float pulse = 0.5f + 0.5f * std::sin(mGlowTime * 3.f);
        sf::CircleShape glow(22.f);
        glow.setOrigin(22.f, 22.f);
        glow.setPosition(mPos);
        glow.setFillColor(sf::Color(255, 200, 50,
            (sf::Uint8)(40 + 40 * pulse)));
        window.draw(glow);
    }
    // Спрайт сундука (закрытый или открытый)
    sf::Sprite spr;
    spr.setTexture(assets.get(mOpened ? "chest_open" : "chest_closed"), true);
    auto sz = spr.getTexture()->getSize();
    if (sz.x == 0 || sz.y == 0) return;
    spr.setOrigin(sz.x * 0.5f, sz.y * 0.5f);
    // Масштабируем под 36x32 пикселя
    spr.setScale(36.f / sz.x, 32.f / sz.y);
    spr.setPosition(mPos);
    window.draw(spr);
}