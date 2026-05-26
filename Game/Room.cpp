#include "Room.h"
#include <cstdlib>
#include <cmath>
static const int DOOR_POS[4][3][2] = {
    {{11,0},{12,0},{13,0}},
    {{11,18},{12,18},{13,18}},
    {{24,8},{24,9},{24,10}},
    {{0,8},{0,9},{0,10}}
};
void Room::generate(int templateIdx) {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            grid[y][x] = (y == 0 || y == H - 1 ||
                x == 0 || x == W - 1) ? 1 : 0;
    auto block = [&](int bx, int by, int bw, int bh) {
        for (int dy = 0; dy < bh; dy++)
            for (int dx = 0; dx < bw; dx++) {
                int nx = bx + dx, ny = by + dy;
                if (nx > 0 && nx < W - 1 && ny > 0 && ny < H - 1)
                    grid[ny][nx] = 1;
            }
        };
    switch (templateIdx % 6) {
    case 0: break;
    case 1:
        block(4, 3, 2, 2);  block(19, 3, 2, 2);
        block(4, 14, 2, 2); block(19, 14, 2, 2);
        break;
    case 2:
        block(11, 4, 3, 4); block(11, 11, 3, 4);
        block(5, 8, 4, 3);  block(16, 8, 4, 3);
        break;
    case 3:
        block(3, 5, 6, 1);  block(16, 5, 6, 1);
        block(3, 13, 6, 1); block(16, 13, 6, 1);
        break;
    case 4:
        block(5, 3, 2, 1);  block(18, 3, 2, 1);
        block(10, 7, 1, 2); block(14, 7, 1, 2);
        block(5, 15, 2, 1); block(18, 15, 2, 1);
        break;
    case 5:
        block(6, 6, 2, 2);  block(17, 6, 2, 2);
        block(6, 11, 2, 2); block(17, 11, 2, 2);
        block(11, 8, 3, 3);
        break;
    }
    for (int d = 0; d < 4; d++) {
        doors[d] = true;
        for (int i = 0; i < 3; i++)
            grid[DOOR_POS[d][i][1]][DOOR_POS[d][i][0]] = 0;
    }
}
void Room::closeDoor(int dir) {
    doors[dir] = false;
    for (int i = 0; i < 3; i++)
        grid[DOOR_POS[dir][i][1]][DOOR_POS[dir][i][0]] = 1;
}
void Room::placeEnemySpawns(int count) {
    enemySpawns.clear();
    std::vector<sf::Vector2f> candidates;
    const float ts = 32.f;
    for (int y = 3; y < H - 3; y++) {
        for (int x = 3; x < W - 3; x++) {
            if (grid[y][x] != 0) continue;
            bool nearDoor = false;
            for (int d = 0; d < 4 && !nearDoor; d++) {
                if (!doors[d]) continue;
                for (int i = 0; i < 3; i++) {
                    if (std::abs(x - DOOR_POS[d][i][0]) <= 2 &&
                        std::abs(y - DOOR_POS[d][i][1]) <= 2) {
                        nearDoor = true;
                        break;
                    }
                }
            }
            if (!nearDoor)
                candidates.push_back({ x * ts + ts * 0.5f,
                                      y * ts + ts * 0.5f });
        }
    }
    for (int i = 0; i < count && !candidates.empty(); i++) {
        int idx = std::rand() % (int)candidates.size();
        enemySpawns.push_back(candidates[idx]);
        candidates.erase(candidates.begin() + idx);
    }
}
void Room::placeChestSpawn() {
    chestSpawns.clear();
    std::vector<sf::Vector2f> candidates;
    const float ts = 32.f;
    for (int y = 5; y < H - 5; y++) {
        for (int x = 5; x < W - 5; x++) {
            if (grid[y][x] != 0) continue;
            bool ok = true;
            for (int dy = -1; dy <= 1 && ok; dy++)
                for (int dx = -1; dx <= 1 && ok; dx++)
                    if (grid[y + dy][x + dx] != 0) ok = false;
            if (ok)
                candidates.push_back({ x * ts + ts * 0.5f,
                                      y * ts + ts * 0.5f });
        }
    }
    if (!candidates.empty()) {
        int idx = std::rand() % (int)candidates.size();
        chestSpawns.push_back(candidates[idx]);
    }
}