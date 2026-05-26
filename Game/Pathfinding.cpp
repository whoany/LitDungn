#include "Pathfinding.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
struct Node {
    int x, y;
    float g = 0.f;
    float f = 0.f;
    bool operator>(const Node& o) const { return f > o.f; }
};
struct PairHash {
    size_t operator()(std::pair<int, int> p) const {
        return std::hash<long long>()(
            (long long)p.first << 32 | (unsigned)p.second
            );
    }
};
using Key = std::pair<int, int>;
using Map = std::unordered_map<Key, Key, PairHash>;
using Cost = std::unordered_map<Key, float, PairHash>;
static float heuristic(int x1, int y1, int x2, int y2) {
    float dx = (float)(x1 - x2);
    float dy = (float)(y1 - y2);
    return std::sqrt(dx * dx + dy * dy);
}
std::vector<sf::Vector2f> Pathfinding::findPath(
    sf::Vector2f startPx,
    sf::Vector2f goalPx,
    const TileMap& map)
{
    int ts = TileMap::TILE_SIZE;
    int sx = (int)(startPx.x / ts);
    int sy = (int)(startPx.y / ts);
    int gx = (int)(goalPx.x / ts);
    int gy = (int)(goalPx.y / ts);
    if (map.getTile(sx, sy) == TileType::Wall ||
        map.getTile(gx, gy) == TileType::Wall)
        return {};
    std::priority_queue<Node, std::vector<Node>,
        std::greater<Node>> open;
    Map  cameFrom;
    Cost gScore;
    auto key = [](int x, int y) -> Key { return { x, y }; };
    gScore[key(sx, sy)] = 0.f;
    open.push({ sx, sy, 0.f, heuristic(sx, sy, gx, gy) });
    const int   dx[] = { 0, 0, 1,-1,  1, 1,-1,-1 };
    const int   dy[] = { 1,-1, 0, 0,  1,-1, 1,-1 };
    const float cost[] = { 1, 1, 1, 1,  1.41f,1.41f,1.41f,1.41f };
    int maxIter = 2000;
    while (!open.empty() && maxIter-- > 0) {
        Node cur = open.top();
        open.pop();
        if (cur.x == gx && cur.y == gy) {
            std::vector<sf::Vector2f> path;
            Key pos = key(gx, gy);
            Key start = key(sx, sy);
            while (pos != start) {
                path.push_back({
                    pos.first * ts + ts * 0.5f,
                    pos.second * ts + ts * 0.5f
                    });
                pos = cameFrom[pos];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (int i = 0; i < 8; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];
            if (map.getTile(nx, ny) == TileType::Wall)
                continue;
            if (i >= 4) {
                if (map.getTile(cur.x + dx[i], cur.y) == TileType::Wall ||
                    map.getTile(cur.x, cur.y + dy[i]) == TileType::Wall)
                    continue;
            }
            float newG = cur.g + cost[i];
            Key   nkey = key(nx, ny);
            auto it = gScore.find(nkey);
            if (it == gScore.end() || newG < it->second) {
                gScore[nkey] = newG;
                cameFrom[nkey] = key(cur.x, cur.y);
                open.push({ nx, ny, newG,
                           newG + heuristic(nx, ny, gx, gy) });
            }
        }
    }
    return {};
}