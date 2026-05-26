#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Assets {
public:
    static Assets& instance();
    void loadAll();
    const sf::Texture& get(const std::string& name) const;
private:
    Assets() = default;
    std::map<std::string, sf::Texture> mTextures;
    sf::Texture mFallback;
    void tryLoad(const std::string& name, const std::string& path);
};