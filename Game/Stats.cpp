#include "Stats.h"
#include <fstream>
#include <sstream>
#include <iostream>
Stats& Stats::instance() {
    static Stats s;
    return s;
}

void Stats::load() {
    std::ifstream file(FILENAME);
    if (!file.is_open()) {
        std::cout << "[Stats] No save file yet — starting fresh.\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        std::stringstream ss(value);
        if (key == "enemies_killed")     ss >> mEnemiesKilled;
        else if (key == "coins_collected")    ss >> mCoinsCollected;
        else if (key == "shots_fired")        ss >> mShotsFired;
        else if (key == "chests_opened")      ss >> mChestsOpened;
        else if (key == "dungeons_completed") ss >> mDungeonsCompleted;
        else if (key == "deaths")             ss >> mDeaths;
        else if (key == "best_score")         ss >> mBestScore;
        else if (key == "play_time_seconds")  ss >> mPlayTimeSeconds;
    }
    std::cout << "[Stats] Loaded.\n";
}
void Stats::save() {
    std::ofstream file(FILENAME);
    if (!file.is_open()) {
        std::cerr << "[Stats] Failed to save!\n";
        return;
    }
    file << "enemies_killed=" << mEnemiesKilled << "\n";
    file << "coins_collected=" << mCoinsCollected << "\n";
    file << "shots_fired=" << mShotsFired << "\n";
    file << "chests_opened=" << mChestsOpened << "\n";
    file << "dungeons_completed=" << mDungeonsCompleted << "\n";
    file << "deaths=" << mDeaths << "\n";
    file << "best_score=" << mBestScore << "\n";
    file << "play_time_seconds=" << mPlayTimeSeconds << "\n";
}
void Stats::reset() {
    mEnemiesKilled = 0;
    mCoinsCollected = 0;
    mShotsFired = 0;
    mChestsOpened = 0;
    mDungeonsCompleted = 0;
    mDeaths = 0;
    mBestScore = 0;
    mPlayTimeSeconds = 0.f;
    save();
}