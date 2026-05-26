#include "Dungeon.h"
#include "TileMap.h"
#include <cstdlib>
static const int DX[] = { 0, 0, 1,-1 };
static const int DY[] = { -1, 1, 0, 0 };
int Dungeon::roomAt(int gx, int gy) const {
    if (gx < 0 || gx >= GRID_SIZE || gy < 0 || gy >= GRID_SIZE)
        return -1;
    return mGrid[gy][gx];
}
Room& Dungeon::currentRoom() { return mRooms[mCurrent]; }
const Room& Dungeon::currentRoom() const { return mRooms[mCurrent]; }
void Dungeon::generate(int numRooms) {
    mRooms.clear();
    mCurrent = 0;
    for (int y = 0; y < GRID_SIZE; y++)
        for (int x = 0; x < GRID_SIZE; x++)
            mGrid[y][x] = -1;
    int cx = 2, cy = 2;
    Room start;
    start.type = RoomType::Start;
    start.gridX = cx;
    start.gridY = cy;
    start.cleared = true;
    start.visited = true;
    start.generate(0);
    mRooms.push_back(start);
    mGrid[cy][cx] = 0;
    std::vector<std::pair<int, int>> path = { {cx, cy} };
    for (int i = 1; i < numRooms; i++) {
        std::vector<int> valid;
        for (int d = 0; d < 4; d++) {
            int nx = cx + DX[d], ny = cy + DY[d];
            if (nx >= 0 && nx < GRID_SIZE &&
                ny >= 0 && ny < GRID_SIZE &&
                mGrid[ny][nx] == -1)
                valid.push_back(d);
        }
        if (valid.empty()) {
            path.pop_back();
            if (path.empty()) break;
            cx = path.back().first;
            cy = path.back().second;
            i--;
            continue;
        }
        int dir = valid[std::rand() % (int)valid.size()];
        cx += DX[dir];
        cy += DY[dir];
        Room room;
        room.gridX = cx;
        room.gridY = cy;
        if (i == numRooms - 1) {
            room.type = RoomType::Boss;
            room.generate(5);
            room.placeEnemySpawns(8);
            room.placeChestSpawn();
        }
        else {
            room.type = RoomType::Normal;
            room.generate(1 + std::rand() % 4);
            room.placeEnemySpawns(2 + i);
            if (std::rand() % 100 < 30)
                room.placeChestSpawn();
        }
        int idx = (int)mRooms.size();
        mRooms.push_back(room);
        mGrid[cy][cx] = idx;
        path.push_back({ cx, cy });
    }
    for (auto& room : mRooms) {
        int gx = room.gridX, gy = room.gridY;
        if (roomAt(gx, gy - 1) < 0) room.closeDoor(0);
        if (roomAt(gx, gy + 1) < 0) room.closeDoor(1);
        if (roomAt(gx + 1, gy) < 0) room.closeDoor(2);
        if (roomAt(gx - 1, gy) < 0) room.closeDoor(3);
    }
}
bool Dungeon::tryTransition(Direction dir) {
    Room& cur = mRooms[mCurrent];
    int d = (int)dir;
    if (!cur.doors[d] || !cur.cleared)
        return false;
    int nx = cur.gridX + DX[d];
    int ny = cur.gridY + DY[d];
    int next = roomAt(nx, ny);
    if (next < 0) return false;
    mCurrent = next;
    mRooms[mCurrent].visited = true;
    return true;
}
sf::Vector2f Dungeon::getEntryPos(Direction fromDir) const {
    const float ts = 32.f;
    float midX = 12.f * ts + ts * 0.5f;
    float midY = 9.f * ts + ts * 0.5f;
    switch (fromDir) {
    case Direction::North: return { midX, 2.f * ts + ts * 0.5f };
    case Direction::South: return { midX, 16.f * ts + ts * 0.5f };
    case Direction::West:  return { 2.f * ts + ts * 0.5f, midY };
    case Direction::East:  return { 22.f * ts + ts * 0.5f, midY };
    }
    return { midX, midY };
}
void Dungeon::loadInto(TileMap& map) const {
    const Room& r = mRooms[mCurrent];
    map.loadRoom(r.grid, r.doors, r.cleared);
}