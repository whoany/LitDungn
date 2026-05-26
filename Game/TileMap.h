#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
enum class TileType { Floor = 0, Wall = 1 };
class TileMap {
public:
    static const int TILE_SIZE = 32;
    static const int MAP_WIDTH = 25;
    static const int MAP_HEIGHT = 19;
    TileMap();
    void loadRoom(const int grid[MAP_HEIGHT][MAP_WIDTH],
        const bool hasDoor[4], bool doorsOpen);
    void setDoorsOpen(bool open);
    void     draw(sf::RenderWindow& window);
    TileType getTile(int x, int y) const;
    bool     isFloor(float px, float py) const;
    bool     isAreaFree(float cx, float cy,
        float halfW, float halfH) const;
private:
    std::vector<std::vector<TileType>> mGrid;
    bool mHasDoor[4] = {};
    bool mDoorsOpen = false;
    static const int DOOR_TILES[4][3][2];
    void applyDoors();
};