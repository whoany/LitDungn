#include "Game.h"
#include "Stats.h"
#include "Assets.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game()
    : mWindow(sf::VideoMode(800, 608), "LitDungn")
{
    mWindow.setFramerateLimit(60);
    std::srand((unsigned)std::time(nullptr));
    Assets::instance().loadAll();
    Stats::instance().load();
    printf("Hello world!");
    if (mFont.loadFromFile("arial.ttf")) {
        mHUD.load(mFont, &mDungeon);
        mMenu.load(mFont);

        mOverlayTitle.setFont(mFont);
        mOverlayTitle.setCharacterSize(52);
        mOverlaySub.setFont(mFont);
        mOverlaySub.setCharacterSize(22);
        mOverlaySub.setFillColor(sf::Color(180, 180, 180));
        mRoomText.setFont(mFont);
        mRoomText.setCharacterSize(26);
        mRoomText.setFillColor(sf::Color(255, 255, 255, 200));
        mInteractText.setFont(mFont);
        mInteractText.setCharacterSize(16);
        mInteractText.setFillColor(sf::Color(255, 220, 100));
        mInteractText.setString("Press E to open");
    }
}
void Game::enterRoom(Direction fromDir) {
    Room& room = mDungeon.currentRoom();
    mDungeon.loadInto(mTileMap);
    mPlayer.setPosition(mDungeon.getEntryPos(fromDir));
    mPlayer.clearBullets();
    mEnemies.clear();
    if (!room.cleared) {
        for (auto& sp : room.enemySpawns)
            mEnemies.emplace_back(sp.x, sp.y);
    }
    mChests.clear();
    if (!room.chestsSpawned) {
        for (auto& cs : room.chestSpawns)
            mChests.emplace_back(cs);
        room.chestsSpawned = true;
    }
    mPickups.clear();
    mRoomTextTimer = 2.f;
    std::string label;
    if (room.type == RoomType::Start) label = "Start";
    else if (room.type == RoomType::Boss) label = "BOSS ROOM";
    else label = "Room " + std::to_string(mDungeon.currentIndex());
    mRoomText.setString(label);
    auto bounds = mRoomText.getLocalBounds();
    mRoomText.setPosition(400.f - bounds.width * 0.5f, 80.f);
}
void Game::checkDoorTransition() {
    sf::Vector2f pos = mPlayer.getPosition();
    const float ts = 32.f;
    Direction dir = Direction::North;
    bool atDoor = false;
    if (pos.y < ts * 0.5f &&
        pos.x > 11 * ts && pos.x < 14 * ts) {
        dir = Direction::North; atDoor = true;
    }
    else if (pos.y > 18 * ts - ts * 0.5f &&
        pos.x > 11 * ts && pos.x < 14 * ts) {
        dir = Direction::South; atDoor = true;
    }
    else if (pos.x > 24 * ts - ts * 0.5f &&
        pos.y > 8 * ts && pos.y < 11 * ts) {
        dir = Direction::East; atDoor = true;
    }
    else if (pos.x < ts * 0.5f &&
        pos.y > 8 * ts && pos.y < 11 * ts) {
        dir = Direction::West; atDoor = true;
    }
    if (!atDoor) return;
    if (mDungeon.tryTransition(dir)) {
        Direction entry;
        switch (dir) {
        case Direction::North: entry = Direction::South; break;
        case Direction::South: entry = Direction::North; break;
        case Direction::East:  entry = Direction::West;  break;
        case Direction::West:  entry = Direction::East;  break;
        }
        enterRoom(entry);
    }
}
void Game::dropFromEnemy(sf::Vector2f pos) {
    if (std::rand() % 100 >= 70) return;
    int roll = std::rand() % 100;
    if (roll < 50) {
        mPickups.emplace_back(pos, PickupType::CoinSmall);
    }
    else if (roll < 70) {
        mPickups.emplace_back(pos, PickupType::CoinLarge);
    }
    else if (roll < 90) {
        mPickups.emplace_back(pos, PickupType::HealthSmall);
    }
    else if (roll < 95) {
        mPickups.emplace_back(pos, PickupType::HealthLarge);
    }
    else {
        WeaponType weapons[] = {
            WeaponType::Shotgun, WeaponType::SMG,
            WeaponType::Sniper,  WeaponType::Laser,
            WeaponType::Rocket
        };
        mPickups.emplace_back(pos, PickupType::WeaponDrop,
            weapons[std::rand() % 5]);
    }
}
void Game::checkPickupCollection() {
    auto playerBounds = mPlayer.getBounds();
    for (auto& p : mPickups) {
        if (!p.isAlive()) continue;
        if (!p.getBounds().intersects(playerBounds)) continue;
        switch (p.getType()) {
        case PickupType::HealthSmall:
            mPlayer.takeDamage(-20.f);
            break;
        case PickupType::HealthLarge:
            mPlayer.takeDamage(-50.f);
            break;
        case PickupType::CoinSmall:
            mCoins += 1;
            mScore += 5;
            Stats::instance().addCoin(1);
            break;
        case PickupType::CoinLarge:
            mCoins += 5;
            mScore += 25;
            Stats::instance().addCoin(5);
            break;
        case PickupType::WeaponDrop:
            mPlayer.addWeapon(p.getWeapon());
            break;
        }
        p.collect();
    }
    mPickups.erase(
        std::remove_if(mPickups.begin(), mPickups.end(),
            [](const Pickup& p) { return !p.isAlive(); }),
        mPickups.end()
    );
}
void Game::checkChestInteraction() {
    mNearChest = false;
    auto playerPos = mPlayer.getPosition();
    for (auto& c : mChests) {
        if (c.isOpened()) continue;
        sf::Vector2f d = c.getPosition() - playerPos;
        float dist = std::sqrt(d.x * d.x + d.y * d.y);
        if (dist < 40.f) {
            mNearChest = true;
            mInteractText.setPosition(
                c.getPosition().x - 55.f,
                c.getPosition().y - 40.f
            );
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                auto drops = c.open();
                Stats::instance().addChestOpened();
                for (auto& p : drops)
                    mPickups.push_back(p);
            }
        }
    }
}
void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
    Stats::instance().save();
}
void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (mState == GameState::Playing) {
                    mState = GameState::Menu;
                }
                else {
                    mWindow.close();
                }
            }
            if (event.key.code == sf::Keyboard::R &&
                (mState == GameState::GameOver ||
                    mState == GameState::Win))
            {
                restart();
            }
            if (mState == GameState::Playing) {
                int slot = -1;
                if (event.key.code == sf::Keyboard::Num1) slot = 0;
                if (event.key.code == sf::Keyboard::Num2) slot = 1;
                if (event.key.code == sf::Keyboard::Num3) slot = 2;
                if (event.key.code == sf::Keyboard::Num4) slot = 3;
                if (event.key.code == sf::Keyboard::Num5) slot = 4;
                if (event.key.code == sf::Keyboard::Num6) slot = 5;
                if (slot >= 0 && slot < (int)mPlayer.weapons().size()) {
                    auto type = mPlayer.weapons()[slot].getType();
                    mPlayer.switchWeapon(type);
                }
            }
        }
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse = mWindow.mapPixelToCoords(
                sf::Mouse::getPosition(mWindow)
            );
            if (mState == GameState::Menu) {
                MenuAction action = mMenu.handleClick(mouse);
                if (action == MenuAction::Play) {
                    restart();
                    mState = GameState::Playing;
                }
                else if (action == MenuAction::Quit) {
                    mWindow.close();
                }
                else if (action == MenuAction::Stats) {
                    mMenu.showStats();
                }
                else if (action == MenuAction::BackFromStats) {
                    mMenu.showMain();
                }
            }
            else if (mState == GameState::Playing) {
                mPlayer.shoot(mouse);
            }
        }
    }

    if (mState == GameState::Playing &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        WeaponType t = mPlayer.currentWeapon().getType();
        if (t == WeaponType::SMG || t == WeaponType::Laser) {
            sf::Vector2f mouse = mWindow.mapPixelToCoords(
                sf::Mouse::getPosition(mWindow)
            );
            mPlayer.shoot(mouse);
        }
    }
}
void Game::update(float dt) {
    if (mState == GameState::Menu) {
        sf::Vector2f mouse = mWindow.mapPixelToCoords(
            sf::Mouse::getPosition(mWindow)
        );
        mMenu.update(dt, mouse);
        return;
    }
    if (mState != GameState::Playing) return;

    Stats::instance().addPlayTime(dt);
    mPlayer.update(dt, mTileMap);
    for (auto& e : mEnemies)
        e.update(dt, mPlayer.getPosition(), mTileMap);
    for (auto& p : mPickups) p.update(dt);
    for (auto& c : mChests)  c.update(dt);
    mPickups.erase(
        std::remove_if(mPickups.begin(), mPickups.end(),
            [](const Pickup& p) { return !p.isAlive(); }),
        mPickups.end()
    );
    checkBulletCollisions();
    checkEnemyPlayerCollision();
    checkPickupCollection();
    checkChestInteraction();
    Room& room = mDungeon.currentRoom();
    if (!room.cleared && mEnemies.empty() &&
        !room.enemySpawns.empty()) {
        room.cleared = true;
        mTileMap.setDoorsOpen(true);
    }
    checkDoorTransition();

    if (!mPlayer.isAlive() && mState != GameState::GameOver) {
        Stats::instance().addDeath();
        Stats::instance().updateBestScore(mScore);
        Stats::instance().save();
        mState = GameState::GameOver;
    }

    for (auto& r : mDungeon.rooms()) {
        if (r.type == RoomType::Boss && r.cleared) {
            if (mState != GameState::Win) {
                Stats::instance().addDungeonWin();
                Stats::instance().updateBestScore(mScore);
                Stats::instance().save();
            }
            mState = GameState::Win;
            break;
        }
    }
    if (mRoomTextTimer > 0.f) mRoomTextTimer -= dt;
}
void Game::checkBulletCollisions() {
    auto& bullets = mPlayer.getProjectiles();
    for (auto& b : bullets) {
        if (!b.isAlive()) continue;
        for (auto& e : mEnemies) {
            if (!e.isAlive()) continue;
            if (b.getBounds().intersects(e.getBounds())) {
                e.takeDamage(b.getDamage());
                if (!e.isAlive()) {
                    mScore += 10;
                    Stats::instance().addEnemyKill();
                    dropFromEnemy(e.getPosition());
                }
                if (!b.isPiercing()) {
                    if (b.isExplosive()) {
                        sf::Vector2f bp = b.getPosition();
                        float r = b.getExplosionRadius();
                        for (auto& e2 : mEnemies) {
                            if (!e2.isAlive()) continue;
                            sf::Vector2f d = e2.getPosition() - bp;
                            float dist = std::sqrt(d.x * d.x + d.y * d.y);
                            if (dist <= r) {
                                e2.takeDamage(b.getDamage() * 0.7f);
                                if (!e2.isAlive()) {
                                    mScore += 10;
                                    Stats::instance().addEnemyKill();
                                    dropFromEnemy(e2.getPosition());
                                }
                            }
                        }
                    }
                    b.kill();
                    break;
                }
            }
        }
    }
    mEnemies.erase(
        std::remove_if(mEnemies.begin(), mEnemies.end(),
            [](const Enemy& e) { return !e.isAlive(); }),
        mEnemies.end()
    );
}
void Game::checkEnemyPlayerCollision() {
    for (auto& e : mEnemies) {
        if (!e.isAlive()) continue;
        if (e.getBounds().intersects(mPlayer.getBounds()))
            mPlayer.takeDamage(10.f);
    }
}
void Game::restart() {
    mPlayer = Player();
    mDungeon.generate(6);
    mScore = 0;
    mCoins = 0;
    mState = GameState::Playing;
    enterRoom(Direction::North);
}
void Game::render() {
    float fps = 1.f / mFpsClock.restart().asSeconds();
    mFpsText.setString("FPS: " + std::to_string((int)fps));
    if (mState == GameState::Menu) {
        mWindow.clear(sf::Color(15, 15, 22));
        mMenu.draw(mWindow);
        mWindow.display();
        return;
    }
    mWindow.clear(sf::Color(20, 20, 30));
    mTileMap.draw(mWindow);
    for (auto& c : mChests) c.draw(mWindow);
    for (auto& p : mPickups) p.draw(mWindow);
    for (auto& e : mEnemies) e.draw(mWindow);
    mPlayer.draw(mWindow);
    mHUD.draw(mWindow, mPlayer.getHP(), mPlayer.getMaxHP(),
        mScore, (int)mEnemies.size(), mPlayer, mCoins);
    mWindow.draw(mFpsText);
    if (mNearChest) mWindow.draw(mInteractText);
    if (mRoomTextTimer > 0.f) {
        float alpha = std::min(1.f, mRoomTextTimer) * 200.f;
        mRoomText.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)alpha));
        mWindow.draw(mRoomText);
    }
    if (mState == GameState::GameOver || mState == GameState::Win) {
        sf::RectangleShape overlay(sf::Vector2f(800.f, 608.f));
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        mWindow.draw(overlay);
        bool won = (mState == GameState::Win);
        mOverlayTitle.setString(won ? "YOU WIN!" : "GAME OVER");
        mOverlayTitle.setFillColor(
            won ? sf::Color(50, 220, 100)
            : sf::Color(220, 50, 50)
        );
        mOverlayTitle.setPosition(won ? 255.f : 220.f, 210.f);
        mOverlaySub.setString(
            "Score: " + std::to_string(mScore) +
            "    Coins: " + std::to_string(mCoins) +
            "\nPress R to restart  |  Esc for menu"
        );
        mOverlaySub.setPosition(180.f, 300.f);
        mWindow.draw(mOverlayTitle);
        mWindow.draw(mOverlaySub);
    }
    mWindow.display();
}