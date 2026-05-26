#pragma once
#include <vector>
#include "Room.h"
enum class Direction { North = 0, South = 1, East = 2, West = 3 };
class TileMap;
class Dungeon {
public:
    void generate(int numRooms = 6);
    Room& currentRoom();
    const Room& currentRoom() const;
    int  currentIndex() const { return mCurrent; }
    int  roomCount()    const { return (int)mRooms.size(); }
    bool tryTransition(Direction dir);
    sf::Vector2f getEntryPos(Direction fromDir) const;
    void loadInto(TileMap& map) const;
    const std::vector<Room>& rooms() const { return mRooms; }
private:
    std::vector<Room> mRooms;
    int mCurrent = 0;
    static const int GRID_SIZE = 5;
    int mGrid[GRID_SIZE][GRID_SIZE];
    int roomAt(int gx, int gy) const;
};