#include "MainMenu.h"
#include "Stats.h"
#include <cmath>
#include <sstream>
#include <iomanip>

void MainMenu::load(sf::Font& font) {
	mFont = &font;
	mTitle.setFont(font);
	mTitle.setString("LitDungn");
	mTitle.setCharacterSize(72);
	mTitle.setFillColor(sf::Color(220, 80, 80));
	mTitle.setStyle(sf::Text::Bold);
	auto tb = mTitle.getLocalBounds();
	mTitle.setPosition(400.f - tb.width * 0.5f, 80.f);
	mSubtitle.setFont(font);
	mSubtitle.setString("dungeon crawler");
	mSubtitle.setCharacterSize(18);
	mSubtitle.setFillColor(sf::Color(150, 150, 170));
	auto sb = mSubtitle.getLocalBounds();
	mSubtitle.setPosition(400.f - sb.width * 0.5f, 170.f);
	mPlayText.setFont(font);
	mPlayText.setString("PLAY");
	mPlayText.setCharacterSize(28);
	mPlayText.setFillColor(sf::Color::White);
	mStatsText.setFont(font);
	mStatsText.setString("STATS");
	mStatsText.setCharacterSize(28);
	mStatsText.setFillColor(sf::Color::White);
	mQuitText.setFont(font);
	mQuitText.setString("QUIT");
	mQuitText.setCharacterSize(28);
	mQuitText.setFillColor(sf::Color::White);
	mHintText.setFont(font);
	mHintText.setString("WASD - move | LMB - shoot | 1-6 - weapons | E - open chest");
	mHintText.setCharacterSize(13);
	mHintText.setFillColor(sf::Color(100, 100, 120));
	auto hb = mHintText.getLocalBounds();
	mHintText.setPosition(400.f - hb.width * 0.5f, 570.f);
	mPlayBtn = sf::FloatRect(300.f, 250.f, 200.f, 55.f);
	mStatsBtn = sf::FloatRect(300.f, 320.f, 200.f, 55.f);
	mQuitBtn = sf::FloatRect(300.f, 390.f, 200.f, 55.f);
	mStatsTitle.setFont(font);
	mStatsTitle.setString("STATISTICS");
	mStatsTitle.setCharacterSize(48);
	mStatsTitle.setFillColor(sf::Color(120, 200, 255));
	mStatsTitle.setStyle(sf::Text::Bold);
	auto stb = mStatsTitle.getLocalBounds();
	mStatsTitle.setPosition(400.f - stb.width * 0.5f, 50.f);
	mBackText.setFont(font);
	mBackText.setString("BACK");
	mBackText.setCharacterSize(24);
	mBackText.setFillColor(sf::Color::White);
	mBackBtn = sf::FloatRect(320.f, 530.f, 160.f, 50.f);
}
void MainMenu::update(float dt, sf::Vector2f mousePos) {
	mAnimTime += dt;
	if (mView == MenuView::Main) {
		mPlayHover = mPlayBtn.contains(mousePos);
		mStatsHover = mStatsBtn.contains(mousePos);
		mQuitHover = mQuitBtn.contains(mousePos);
	}
	else {
		mBackHover = mBackBtn.contains(mousePos);
	}
}
MenuAction MainMenu::handleClick(sf::Vector2f mousePos) {
	if (mView == MenuView::Main) {
		if (mPlayBtn.contains(mousePos))  return MenuAction::Play;
		if (mStatsBtn.contains(mousePos)) return MenuAction::Stats;
		if (mQuitBtn.contains(mousePos))  return MenuAction::Quit;
	}
	else {
		if (mBackBtn.contains(mousePos))  return MenuAction::BackFromStats;
	}
	return MenuAction::None;
}
void MainMenu::draw(sf::RenderWindow& window) {
	sf::RectangleShape bg({ 800.f, 608.f });
	bg.setFillColor(sf::Color(15, 15, 22));
	window.draw(bg);
	for (int i = 0; i < 30; i++) {
		float x = (float)((i * 137 + 50) % 800);
		float y = (float)((i * 211 + 30) % 608);
		float pulse = 0.5f + 0.5f * std::sin(mAnimTime * 2.f + i * 0.4f);
		sf::CircleShape star(1.5f);
		star.setPosition(x, y);
		star.setFillColor(sf::Color(180, 180, 220, (sf::Uint8)(60 * pulse)));
		window.draw(star);
	}
	if (mView == MenuView::Main)
		drawMain(window);
	else
		drawStats(window);
}
void MainMenu::drawMain(sf::RenderWindow& window) {

	float titlePulse = 0.5f + 0.5f * std::sin(mAnimTime * 1.5f);
	sf::Text titleGlow = mTitle;
	titleGlow.setFillColor(sf::Color(220, 80, 80, (sf::Uint8)(60 * titlePulse)));
	titleGlow.setPosition(mTitle.getPosition().x + 2,
		mTitle.getPosition().y + 2);
	window.draw(titleGlow);
	window.draw(mTitle);
	window.draw(mSubtitle);

	auto drawBtn = [&](sf::FloatRect rect, sf::Text& text,
		bool hover, sf::Color base, sf::Color hoverC,
		sf::Color border, sf::Color borderHover) {
			sf::RectangleShape btn({ rect.width, rect.height });
			btn.setPosition(rect.left, rect.top);
			btn.setFillColor(hover ? hoverC : base);
			btn.setOutlineThickness(2.f);
			btn.setOutlineColor(hover ? borderHover : border);
			window.draw(btn);
			auto tb = text.getLocalBounds();
			text.setPosition(
				rect.left + rect.width * 0.5f - tb.width * 0.5f,
				rect.top + rect.height * 0.5f - tb.height
			);
			window.draw(text);
		};

	drawBtn(mPlayBtn, mPlayText, mPlayHover,
		sf::Color(30, 80, 45), sf::Color(50, 130, 70),
		sf::Color(60, 130, 80), sf::Color(120, 220, 140));

	drawBtn(mStatsBtn, mStatsText, mStatsHover,
		sf::Color(30, 50, 90), sf::Color(50, 90, 150),
		sf::Color(60, 100, 160), sf::Color(120, 180, 240));

	drawBtn(mQuitBtn, mQuitText, mQuitHover,
		sf::Color(60, 30, 30), sf::Color(130, 50, 50),
		sf::Color(120, 60, 60), sf::Color(220, 100, 100));
	window.draw(mHintText);
}
void MainMenu::drawStats(sf::RenderWindow& window) {
	auto& stats = Stats::instance();
	window.draw(mStatsTitle);

	auto drawStat = [&](float y, const std::string& label,
		const std::string& value, sf::Color valColor) {
			sf::Text labelText;
			labelText.setFont(*mFont);
			labelText.setCharacterSize(20);
			labelText.setFillColor(sf::Color(200, 200, 220));
			labelText.setString(label);
			labelText.setPosition(150.f, y);
			window.draw(labelText);
			sf::Text valueText;
			valueText.setFont(*mFont);
			valueText.setCharacterSize(20);
			valueText.setStyle(sf::Text::Bold);
			valueText.setFillColor(valColor);
			valueText.setString(value);

			auto vb = valueText.getLocalBounds();
			valueText.setPosition(650.f - vb.width, y);
			window.draw(valueText);

			sf::RectangleShape dotLine({ 350.f, 1.f });
			dotLine.setPosition(190.f, y + 18.f);
			dotLine.setFillColor(sf::Color(60, 60, 80));
			window.draw(dotLine);
		};

	auto fmt = [](int n) { return std::to_string(n); };

	auto fmtTime = [](float minutes) {
		int totalSec = (int)(minutes * 60);
		int h = totalSec / 3600;
		int m = (totalSec / 60) % 60;
		int s = totalSec % 60;
		std::stringstream ss;
		if (h > 0) ss << h << "h ";
		ss << m << "m " << s << "s";
		return ss.str();
		};

	int shots = stats.getShotsFired();
	int kills = stats.getEnemiesKilled();
	std::string accuracy = "—";
	if (shots > 0) {
		float acc = (float)kills / shots * 100.f;
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << acc << "%";
		accuracy = ss.str();
	}
	float y = 150.f;
	float step = 38.f;
	drawStat(y, "Enemies killed", fmt(kills), sf::Color(255, 180, 100));
	y += step;
	drawStat(y, "Coins collected", fmt(stats.getCoinsCollected()), sf::Color(255, 220, 50));
	y += step;
	drawStat(y, "Chests opened", fmt(stats.getChestsOpened()), sf::Color(220, 180, 100));
	y += step;
	drawStat(y, "Shots fired", fmt(shots), sf::Color(180, 200, 255));
	y += step;
	drawStat(y, "Accuracy", accuracy, sf::Color(120, 220, 140));
	y += step;
	drawStat(y, "Dungeons completed", fmt(stats.getDungeonsCompleted()), sf::Color(120, 220, 140));
	y += step;
	drawStat(y, "Deaths", fmt(stats.getDeaths()), sf::Color(220, 80, 80));
	y += step;
	drawStat(y, "Best score", fmt(stats.getBestScore()), sf::Color(255, 220, 50));
	y += step;
	drawStat(y, "Total play time", fmtTime(stats.getPlayTimeMinutes()), sf::Color(180, 180, 220));
	sf::RectangleShape backBtn({ mBackBtn.width, mBackBtn.height });
	backBtn.setPosition(mBackBtn.left, mBackBtn.top);
	backBtn.setFillColor(mBackHover
		? sf::Color(60, 90, 130)
		: sf::Color(30, 50, 80));
	backBtn.setOutlineThickness(2.f);
	backBtn.setOutlineColor(mBackHover
		? sf::Color(120, 180, 240)
		: sf::Color(60, 100, 160));
	window.draw(backBtn);
	auto bb = mBackText.getLocalBounds();
	mBackText.setPosition(
		mBackBtn.left + mBackBtn.width * 0.5f - bb.width * 0.5f,
		mBackBtn.top + mBackBtn.height * 0.5f - bb.height
	);
	window.draw(mBackText);
}