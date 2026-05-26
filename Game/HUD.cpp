#include "HUD.h"
#include "Dungeon.h"
#include "Player.h"
#include "Weapon.h"
#include "Assets.h"

void HUD::load(sf::Font& font, const Dungeon* dungeon) {
    mFont = &font;
    mDungeon = dungeon;
    auto setup = [&](sf::Text& t, int size, sf::Color color,
        float x, float y) {
            t.setFont(font);
            t.setCharacterSize(size);
            t.setFillColor(color);
            t.setPosition(x, y);
        };
    setup(mHpText, 12, sf::Color::White, 15.f, 13.f);
    setup(mScoreText, 18, sf::Color::White, 10.f, 38.f);
    setup(mEnemiesText, 18, sf::Color(255, 0, 0), 10.f, 60.f);
    setup(mCoinsText, 18, sf::Color(255, 220, 50), 10.f, 82.f);
    setup(mWeaponText, 16, sf::Color::White, 10.f, 545.f);
}
void HUD::drawMinimap(sf::RenderWindow& window) {
    if (!mDungeon) return;
    const float cellSize = 14.f;
    const float gap = 3.f;
    const float startX = 670.f;
    const float startY = 40.f;
    auto& rooms = mDungeon->rooms();
    int cur = mDungeon->currentIndex();
    for (int i = 0; i < (int)rooms.size(); i++) {
        auto& r = rooms[i];
        if (!r.visited) continue;
        float x = startX + r.gridX * (cellSize + gap);
        float y = startY + r.gridY * (cellSize + gap);
        sf::RectangleShape cell({ cellSize, cellSize });
        cell.setPosition(x, y);
        if (i == cur)
            cell.setFillColor(sf::Color(80, 160, 255));
        else if (r.type == RoomType::Boss)
            cell.setFillColor(sf::Color(200, 50, 50));
        else if (r.cleared)
            cell.setFillColor(sf::Color(50, 180, 50));
        else
            cell.setFillColor(sf::Color(100, 100, 100));
        cell.setOutlineThickness(1.f);
        cell.setOutlineColor(sf::Color(60, 60, 60));
        window.draw(cell);
        for (int d = 0; d < 4; d++) {
            if (!r.doors[d]) continue;
            static const int DX[] = { 0,0,1,-1 };
            static const int DY[] = { -1,1,0,0 };
            int nx = r.gridX + DX[d];
            int ny = r.gridY + DY[d];
            for (int j = 0; j < (int)rooms.size(); j++) {
                if (rooms[j].gridX == nx && rooms[j].gridY == ny
                    && rooms[j].visited)
                {
                    float x2 = startX + nx * (cellSize + gap)
                        + cellSize * 0.5f;
                    float y2 = startY + ny * (cellSize + gap)
                        + cellSize * 0.5f;
                    float x1 = x + cellSize * 0.5f;
                    float y1 = y + cellSize * 0.5f;
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(x1, y1),
                                   sf::Color(80, 80, 80)),
                        sf::Vertex(sf::Vector2f(x2, y2),
                                   sf::Color(80, 80, 80))
                    };
                    window.draw(line, 2, sf::Lines);
                    break;
                }
            }
        }
    }
}

static std::string weaponTexName(WeaponType t) {
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

void HUD::drawWeaponBar(sf::RenderWindow& window, const Player& player) {
    const auto& weapons = player.weapons();
    int curIdx = player.currentWeaponIdx();
    auto& assets = Assets::instance();
    const float slotSize = 44.f;
    const float gap = 6.f;
    const float startX = 10.f;
    const float startY = 565.f;
    for (int i = 0; i < (int)weapons.size(); i++) {
        float x = startX + i * (slotSize + gap);
        sf::RectangleShape slot({ slotSize, slotSize });
        slot.setPosition(x, startY);
        slot.setFillColor(i == curIdx
            ? sf::Color(40, 60, 100)
            : sf::Color(30, 30, 40));
        slot.setOutlineThickness(2.f);
        slot.setOutlineColor(i == curIdx
            ? sf::Color(120, 200, 255)
            : sf::Color(60, 60, 80));
        window.draw(slot);
        sf::Sprite icon;
        icon.setTexture(
            assets.get(weaponTexName(weapons[i].getType())), true);
        auto sz = icon.getTexture()->getSize();
        if (sz.x > 0 && sz.y > 0) {
            icon.setOrigin(sz.x * 0.5f, sz.y * 0.5f);
            icon.setScale(32.f / sz.x, 32.f / sz.y);
            icon.setPosition(x + slotSize * 0.5f, startY + slotSize * 0.5f);
            window.draw(icon);
        }

        if (mFont) {
            sf::Text num;
            num.setFont(*mFont);
            num.setCharacterSize(11);
            num.setFillColor(sf::Color(200, 200, 200));
            num.setString(std::to_string(i + 1));
            num.setPosition(x + 4.f, startY + 2.f);
            window.draw(num);
        }
        float cd = weapons[i].getCooldownProgress();
        if (cd < 1.f) {
            sf::RectangleShape cdBar({ slotSize * cd, 3.f });
            cdBar.setPosition(x, startY + slotSize - 3.f);
            cdBar.setFillColor(sf::Color(255, 200, 50));
            window.draw(cdBar);
        }
    }
    if (mFont) {
        mWeaponText.setString(player.currentWeapon().getStats().name);
        window.draw(mWeaponText);
    }
}



void HUD::draw(sf::RenderWindow& window,
    float hp, float maxHP,
    int score, int enemiesLeft,
    const Player& player, int coins)
{
    sf::RectangleShape hpBg({ 200.f, 22.f });
    hpBg.setFillColor(sf::Color(80, 0, 0));
    hpBg.setPosition(10.f, 10.f);
    float ratio = (maxHP > 0.f) ? hp / maxHP : 0.f;
    sf::RectangleShape hpFill({ 200.f * ratio, 22.f });
    hpFill.setPosition(10.f, 10.f);
    if (ratio > 0.5f)  hpFill.setFillColor(sf::Color(30, 200, 30));
    else if (ratio > 0.25f) hpFill.setFillColor(sf::Color(220, 180, 0));
    else                    hpFill.setFillColor(sf::Color(220, 30, 30));
    mHpText.setString(
        "HP: " + std::to_string((int)hp) +
        " / " + std::to_string((int)maxHP)
    );
    mScoreText.setString("Score: " + std::to_string(score));
    mEnemiesText.setString("Enemies: " + std::to_string(enemiesLeft));
    mCoinsText.setString("Coins: " + std::to_string(coins));
    window.draw(hpBg);
    window.draw(hpFill);
    if (mFont) {
        window.draw(mHpText);
        window.draw(mScoreText);
        window.draw(mEnemiesText);
        window.draw(mCoinsText);
    }
    drawMinimap(window);
    drawWeaponBar(window, player);
}