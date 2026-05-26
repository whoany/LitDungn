#include "Pickup.h"
#include "Assets.h"
#include <cmath>
Pickup::Pickup(sf::Vector2f pos, PickupType type,
    WeaponType weaponType)
    : mPos(pos), mType(type), mWeaponType(weaponType) {
}
sf::FloatRect Pickup::getBounds() const {
    return sf::FloatRect(mPos.x - 12.f, mPos.y - 12.f, 24.f, 24.f);
}
void Pickup::update(float dt) {
    if (!mAlive) return;
    mBobTimer += dt;
    mLifetime -= dt;
    if (mLifetime <= 0.f) mAlive = false;
}
// Возвращает имя текстуры для иконки оружия
static std::string weaponTextureName(WeaponType t) {
    switch (t) {
    case WeaponType::Pistol:  return "w_pistol";
    case WeaponType::Shotgun: return "w_shotgun";
    case WeaponType::SMG:     return "w_smg";
    case WeaponType::Sniper:  return "w_sniper";
    case WeaponType::Laser:   return "w_laser";
    case WeaponType::Rocket:  return "w_rocket";
    }
    return "w_pistol";
}
void Pickup::draw(sf::RenderWindow& window) {
    if (!mAlive) return;
    // Эффект покачивания
    float yOffset = std::sin(mBobTimer * 3.f) * 3.f;
    // Мигание в последние секунды жизни
    bool fading = (mLifetime < 5.f);
    if (fading && (int)(mLifetime * 6.f) % 2 == 0) return;
    sf::Vector2f drawPos(mPos.x, mPos.y + yOffset);
    auto& assets = Assets::instance();
    // Определяем текстуру и желаемый размер на экране
    sf::Sprite spr;
    std::string texName;
    float targetSize = 20.f;
    switch (mType) {
    case PickupType::HealthSmall:
        texName = "heart"; targetSize = 18.f; break;
    case PickupType::HealthLarge:
        texName = "heart"; targetSize = 24.f; break;
    case PickupType::CoinSmall:
        texName = "coin";  targetSize = 16.f; break;
    case PickupType::CoinLarge:
        texName = "coin";  targetSize = 22.f; break;
    case PickupType::WeaponDrop:
        texName = weaponTextureName(mWeaponType); targetSize = 24.f; break;
    }
    // Берём текстуру и масштабируем под нужный размер
    spr.setTexture(assets.get(texName), true);
    auto sz = spr.getTexture()->getSize();
    if (sz.x == 0 || sz.y == 0) return;
    spr.setOrigin(sz.x * 0.5f, sz.y * 0.5f);
    spr.setScale(targetSize / sz.x, targetSize / sz.y);
    spr.setPosition(drawPos);
    window.draw(spr);
    // Для оружия дополнительно рисуем подсветку цветом пуль
    if (mType == PickupType::WeaponDrop) {
        sf::CircleShape ring(targetSize * 0.65f);
        ring.setOrigin(targetSize * 0.65f, targetSize * 0.65f);
        ring.setPosition(drawPos);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(2.f);
        ring.setOutlineColor(Weapon::getColor(mWeaponType));
        window.draw(ring);
    }
}