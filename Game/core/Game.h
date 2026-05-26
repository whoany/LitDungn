#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Dungeon.h"
#include "HUD.h"
enum class GameState { Playing, GameOver, Win };
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
    void restart();
    void enterRoom(Direction fromDir);
    void checkDoorTransition();
    sf::RenderWindow    mWindow;
    Player              mPlayer;
    TileMap             mTileMap;
    Dungeon             mDungeon;
    std::vector<Enemy>  mEnemies;
    HUD                 mHUD;
    GameState mState = GameState::Playing;
    int       mScore = 0;
    sf::Font  mFont;
    sf::Text  mFpsText;
    sf::Text  mOverlayTitle;
    sf::Text  mOverlaySub;
    sf::Text  mRoomText;
    sf::Clock mFpsClock;
    float mRoomTextTimer = 0.f;
};