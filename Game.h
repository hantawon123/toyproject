#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Player.h"
#include "Monster.h"

class Game
{
private:
    sf::RenderWindow window;

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