#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Enemy.hpp"
#include "TileMap.hpp"
#include "Dungeon.hpp"
#include "Pickup.hpp"
#include "Chest.hpp"
#include "HUD.hpp"
#include "MainMenu.hpp"

enum class GameState { Menu, Playing, GameOver, Win };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void checkBulletCollisions();
    void checkEnemyPlayerCollision();
    void checkPickupCollection();
    void checkChestInteraction();
    void restart();

    void enterRoom(Direction fromDir);
    void checkDoorTransition();

    // Р”СЂРѕРї СЃ СѓР±РёС‚РѕРіРѕ РІСЂР°РіР°
    void dropFromEnemy(sf::Vector2f pos);

    sf::RenderWindow    mWindow;
    Player              mPlayer;
    TileMap             mTileMap;
    Dungeon             mDungeon;
    std::vector<Enemy>  mEnemies;
    std::vector<Pickup> mPickups;
    std::vector<Chest>  mChests;
    HUD                 mHUD;
    MainMenu            mMenu;

    GameState mState = GameState::Menu;
    int       mScore = 0;
    int       mCoins = 0;

    sf::Font  mFont;
    sf::Text  mFpsText;
    sf::Text  mOverlayTitle;
    sf::Text  mOverlaySub;
    sf::Text  mRoomText;
    sf::Text  mInteractText;  // "Press E to open" РІРѕР·Р»Рµ СЃСѓРЅРґСѓРєР°
    sf::Clock mFpsClock;

    float mRoomTextTimer = 0.f;
    bool  mNearChest     = false;
};
