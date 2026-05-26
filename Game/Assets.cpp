#include "Assets.h"
#include <iostream>
Assets& Assets::instance() {
    static Assets a;
    return a;
}
void Assets::tryLoad(const std::string& name, const std::string& path) {
    sf::Texture tex;
    if (tex.loadFromFile(path)) {
        mTextures[name] = tex;
        std::cout << "[Assets] Loaded: " << path << "\n";
    }
    else {
        std::cerr << "[Assets] FAILED to load: " << path
            << " — using fallback texture.\n";
    }
}
void Assets::loadAll() {
    sf::Image fb;
    fb.create(32, 32, sf::Color(255, 0, 255));
    for (int y = 0; y < 32; y++)
        for (int x = 0; x < 32; x++)
            if ((x / 8 + y / 8) % 2 == 0)
                fb.setPixel(x, y, sf::Color::Black);
    mFallback.loadFromImage(fb);
    tryLoad("floor", "assets/floor.png");
    tryLoad("wall", "assets/wall.png");
    tryLoad("door_open", "assets/door_open.png");
    tryLoad("door_closed", "assets/door_closed.png");
    tryLoad("player", "assets/player.png");
    tryLoad("enemy", "assets/enemy.png");
    tryLoad("w_pistol", "assets/weapons/pistol.png");
    tryLoad("w_shotgun", "assets/weapons/shotgun.png");
    tryLoad("w_smg", "assets/weapons/smg.png");
    tryLoad("w_sniper", "assets/weapons/sniper.png");
    tryLoad("w_laser", "assets/weapons/laser.png");
    tryLoad("w_rocket", "assets/weapons/rocket.png");
    tryLoad("coin", "assets/items/coin.png");
    tryLoad("heart", "assets/items/heart.png");
    tryLoad("chest_closed", "assets/items/chest_closed.png");
    tryLoad("chest_open", "assets/items/chest_open.png");
    std::cout << "[Assets] Total loaded: " << mTextures.size() << "\n";
}
const sf::Texture& Assets::get(const std::string& name) const {
    auto it = mTextures.find(name);
    if (it != mTextures.end()) return it->second;
    return mFallback;
}