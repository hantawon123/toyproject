#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "Player.h"
#include "Monster.h"

class Game
{
private:
    sf::RenderWindow window;

    // UI 배경
    sf::RectangleShape statusPanel;

    // HP Bar
    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarFront;

    // EXP Bar
    sf::RectangleShape expBarBack;
    sf::RectangleShape expBarFront;

    // UI Text
    std::optional<sf::Text> levelText;
    std::optional<sf::Text> hpText;
    std::optional<sf::Text> expText;

    Player player;
    Monster monster;

    sf::RectangleShape ground;

    bool prevjumped;
    bool prevlevelup;
    bool prevchoosed;

    // 레벨업 창 표시 여부
    bool isLevelUpChoosing = false;

    // 현재 레벨업 선택지 3개
    std::array<LevelUpOption, 3> currentOptions;

    // 글씨 출력용
    sf::Font font;
    std::optional<sf::Text> levelUpTitle;
    std::vector<sf::Text> optionTexts;

    std::vector<Monster> monsters;

    // 스폰 관련
    float spawnTimer = 0.f;
    float spawnInterval = 2.0f; // 2초마다 생성

public:
    Game();
    void run();

    void handleInput();
    void update(float dt);
    void render();

    // 레벨업 UI 준비
    void openLevelUpMenu();
    void applyLevelUpOption(int index);
};