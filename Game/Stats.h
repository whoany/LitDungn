#pragma once
#include <string>

class Stats {
public:
    static Stats& instance();
    void load();
    void save();
    void addEnemyKill() { mEnemiesKilled++;     save(); }
    void addCoin(int n) { mCoinsCollected += n; save(); }
    void addShot() { mShotsFired++; }
    void addChestOpened() { mChestsOpened++;     save(); }
    void addDungeonWin() { mDungeonsCompleted++; save(); }
    void addDeath() { mDeaths++;            save(); }
    void addPlayTime(float seconds) { mPlayTimeSeconds += seconds; }
    void updateBestScore(int score) {
        if (score > mBestScore) { mBestScore = score; save(); }
    }
    int   getEnemiesKilled()     const { return mEnemiesKilled; }
    int   getCoinsCollected()    const { return mCoinsCollected; }
    int   getShotsFired()        const { return mShotsFired; }
    int   getChestsOpened()      const { return mChestsOpened; }
    int   getDungeonsCompleted() const { return mDungeonsCompleted; }
    int   getDeaths()            const { return mDeaths; }
    int   getBestScore()         const { return mBestScore; }
    float getPlayTimeMinutes()   const { return mPlayTimeSeconds / 60.f; }
    void reset();
private:
    Stats() = default;
    int   mEnemiesKilled = 0;
    int   mCoinsCollected = 0;
    int   mShotsFired = 0;
    int   mChestsOpened = 0;
    int   mDungeonsCompleted = 0;
    int   mDeaths = 0;
    int   mBestScore = 0;
    float mPlayTimeSeconds = 0.f;
    const std::string FILENAME = "grimveil_stats.dat";
};