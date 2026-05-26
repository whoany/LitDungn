#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "TileMap.h"
class Pathfinding {
public:
    static std::vector<sf::Vector2f> findPath(
        sf::Vector2f startPx,
        sf::Vector2f goalPx,
        const TileMap& map
    );
};