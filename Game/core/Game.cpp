#include "Game.h"
#include <string>
#include <cstdlib>
#include <ctime>
Game::Game()
    : mWindow(sf::VideoMode(800, 608), "Grimveil")
{
    mWindow.setFramerateLimit(60);
    std::srand((unsigned)std::time(nullptr));
    mDungeon.generate(6);
    enterRoom(Direction::North);
    if (mFont.loadFromFile("arial.ttf")) {
        mHUD.load(mFont, &mDungeon);
        mFpsText.setFont(mFont);
        mFpsText.setCharacterSize(14);
        mFpsText.setFillColor(sf::Color::Yellow);
        mFpsText.setPosition(710.f, 10.f);
        mOverlayTitle.setFont(mFont);
        mOverlayTitle.setCharacterSize(52);
        mOverlaySub.setFont(mFont);
        mOverlaySub.setCharacterSize(22);
        mOverlaySub.setFillColor(sf::Color(180, 180, 180));
        mRoomText.setFont(mFont);
        mRoomText.setCharacterSize(26);
        mRoomText.setFillColor(sf::Color(255, 255, 255, 200));
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
void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}
void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            mWindow.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
                mWindow.close();
            if (event.key.code == sf::Keyboard::R)
                restart();
        }
        if (mState == GameState::Playing &&
            event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse = mWindow.mapPixelToCoords(
                sf::Mouse::getPosition(mWindow)
            );
            mPlayer.shoot(mouse);
        }
    }
}
void Game::update(float dt) {
    if (mState != GameState::Playing) return;
    mPlayer.update(dt, mTileMap);
    for (auto& e : mEnemies)
        e.update(dt, mPlayer.getPosition(), mTileMap);
    checkBulletCollisions();
    checkEnemyPlayerCollision();
    Room& room = mDungeon.currentRoom();
    if (!room.cleared && mEnemies.empty() &&
        !room.enemySpawns.empty()) {
        room.cleared = true;
        mTileMap.setDoorsOpen(true);
    }
    checkDoorTransition();
    if (!mPlayer.isAlive())
        mState = GameState::GameOver;
    for (auto& r : mDungeon.rooms()) {
        if (r.type == RoomType::Boss && r.cleared) {
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
                e.takeDamage(25.f);
                b.kill();
                if (!e.isAlive()) mScore += 10;
                break;
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
    mState = GameState::Playing;
    enterRoom(Direction::North);
}
void Game::render() {
    float fps = 1.f / mFpsClock.restart().asSeconds();
    mFpsText.setString("FPS: " + std::to_string((int)fps));
    mWindow.clear(sf::Color(20, 20, 30));
    mTileMap.draw(mWindow);
    for (auto& e : mEnemies) e.draw(mWindow);
    mPlayer.draw(mWindow);
    mHUD.draw(mWindow, mPlayer.getHP(), mPlayer.getMaxHP(),
        mScore, (int)mEnemies.size());
    mWindow.draw(mFpsText);
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
            "\nPress R to restart  |  Esc to quit"
        );
        mOverlaySub.setPosition(230.f, 300.f);
        mWindow.draw(mOverlayTitle);
        mWindow.draw(mOverlaySub);
    }
    mWindow.display();
}