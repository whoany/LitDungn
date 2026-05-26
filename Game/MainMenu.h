#pragma once
#include <SFML/Graphics.hpp>

enum class MenuAction { None, Play, Quit, Stats, BackFromStats };
enum class MenuView { Main, Stats };
class MainMenu {
public:
    void load(sf::Font& font);
    void update(float dt, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
    MenuAction handleClick(sf::Vector2f mousePos);
    void showMain() { mView = MenuView::Main; }
    void showStats() { mView = MenuView::Stats; }
private:
    void drawMain(sf::RenderWindow& window);
    void drawStats(sf::RenderWindow& window);
    sf::Font* mFont = nullptr;
    MenuView  mView = MenuView::Main;
    sf::Text  mTitle;
    sf::Text  mSubtitle;
    sf::Text  mPlayText;
    sf::Text  mStatsText;
    sf::Text  mQuitText;
    sf::Text  mHintText;
    sf::FloatRect mPlayBtn;
    sf::FloatRect mStatsBtn;
    sf::FloatRect mQuitBtn;
    bool mPlayHover = false;
    bool mStatsHover = false;
    bool mQuitHover = false;
    sf::Text mStatsTitle;
    sf::Text mBackText;
    sf::FloatRect mBackBtn;
    bool mBackHover = false;
    float mAnimTime = 0.f;
};