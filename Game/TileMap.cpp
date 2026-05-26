#include "TileMap.h"
#include "Assets.h"
const int TileMap::DOOR_TILES[4][3][2] = {
    {{11,0},{12,0},{13,0}},
    {{11,18},{12,18},{13,18}},
    {{24,8},{24,9},{24,10}},
    {{0,8},{0,9},{0,10}}
};
TileMap::TileMap() {
    mGrid.resize(MAP_HEIGHT,
        std::vector<TileType>(MAP_WIDTH, TileType::Floor));
}
void TileMap::loadRoom(const int grid[MAP_HEIGHT][MAP_WIDTH],
    const bool hasDoor[4], bool doorsOpen) {
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            mGrid[y][x] = grid[y][x] == 1
            ? TileType::Wall : TileType::Floor;
    for (int i = 0; i < 4; i++)
        mHasDoor[i] = hasDoor[i];
    mDoorsOpen = doorsOpen;
    applyDoors();
}
void TileMap::setDoorsOpen(bool open) {
    mDoorsOpen = open;
    applyDoors();
}
void TileMap::applyDoors() {
    for (int d = 0; d < 4; d++) {
        if (!mHasDoor[d]) continue;
        for (int i = 0; i < 3; i++) {
            int x = DOOR_TILES[d][i][0];
            int y = DOOR_TILES[d][i][1];
            mGrid[y][x] = mDoorsOpen
                ? TileType::Floor : TileType::Wall;
        }
    }
}
TileType TileMap::getTile(int x, int y) const {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return TileType::Wall;
    return mGrid[y][x];
}
bool TileMap::isFloor(float px, float py) const {
    return getTile((int)(px / TILE_SIZE),
        (int)(py / TILE_SIZE)) == TileType::Floor;
}
bool TileMap::isAreaFree(float cx, float cy,
    float halfW, float halfH) const {
    return isFloor(cx - halfW, cy - halfH) &&
        isFloor(cx + halfW, cy - halfH) &&
        isFloor(cx - halfW, cy + halfH) &&
        isFloor(cx + halfW, cy + halfH);
}

void TileMap::draw(sf::RenderWindow& window) {
    auto& assets = Assets::instance();
    sf::Sprite tileSprite;
    auto setupTile = [&](const std::string& texName) {
        tileSprite.setTexture(assets.get(texName), true);
        auto sz = tileSprite.getTexture()->getSize();
        if (sz.x != TILE_SIZE || sz.y != TILE_SIZE)
            tileSprite.setScale((float)TILE_SIZE / sz.x,
                (float)TILE_SIZE / sz.y);
        else
            tileSprite.setScale(1.f, 1.f);
        };
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            bool isDoor = false;
            for (int d = 0; d < 4 && !isDoor; d++) {
                if (!mHasDoor[d]) continue;
                for (int i = 0; i < 3; i++) {
                    if (DOOR_TILES[d][i][0] == x &&
                        DOOR_TILES[d][i][1] == y) {
                        isDoor = true;
                        break;
                    }
                }
            }
            float px = (float)(x * TILE_SIZE);
            float py = (float)(y * TILE_SIZE);
            if (isDoor || mGrid[y][x] == TileType::Floor) {
                setupTile("floor");
                tileSprite.setPosition(px, py);
                window.draw(tileSprite);
            }
            if (isDoor) {
                setupTile(mDoorsOpen ? "door_open" : "door_closed");
                tileSprite.setPosition(px, py);
                window.draw(tileSprite);
            }
            else if (mGrid[y][x] == TileType::Wall) {
                setupTile("wall");
                tileSprite.setPosition(px, py);
                window.draw(tileSprite);
            }
        }
    }
}