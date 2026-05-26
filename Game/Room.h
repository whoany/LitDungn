#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
enum class RoomType { Start, Normal, Boss };
struct Room {
    static const int W = 25;
    static const int H = 19;
    int  grid[H][W];
    std::vector<sf::Vector2f> enemySpawns;
    std::vector<sf::Vector2f> chestSpawns;
    bool doors[4] = {};
    bool cleared = false;
    bool visited = false;
    bool chestsSpawned = false;
    RoomType type = RoomType::Normal;
    int  gridX = 0, gridY = 0;
    void generate(int templateIdx);
    void placeEnemySpawns(int count);
    void placeChestSpawn();
    void closeDoor(int dir);
};