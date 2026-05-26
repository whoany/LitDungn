#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Pickup.h"
class Chest {
public:
    Chest(sf::Vector2f pos);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    std::vector<Pickup> open();
    bool isOpened() const { return mOpened; }
    sf::FloatRect getBounds() const;
    sf::Vector2f  getPosition() const { return mPos; }
private:
    sf::Vector2f mPos;
    bool         mOpened = false;
    float        mGlowTime = 0.f;
};