#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Dungeon;
class Player;

class HUD {
public:
    void load(sf::Font& font, const Dungeon* dungeon);
    void draw(sf::RenderWindow& window,
              float hp, float maxHP,
              int score, int enemiesLeft,
              const Player& player);

private:
    sf::Font* mFont    = nullptr;
    const Dungeon* mDungeon = nullptr;

    sf::Text mScoreText;
    sf::Text mEnemiesText;
    sf::Text mHpText;
    sf::Text mWeaponText;

    void drawMinimap(sf::RenderWindow& window);
    void drawWeaponBar(sf::RenderWindow& window, const Player& player);
};
