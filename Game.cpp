#include "Game.h"
#include <optional>

// 생성자
Game::Game()
    : window(sf::VideoMode({1600, 1200}), "ToyProject")
{
    window.setFramerateLimit(60);

    player.init();
    monster.init();

    ground.setSize({1600.f, 50.f});
    ground.setFillColor(sf::Color::White);
    ground.setPosition({0.f, 1150.f});

    prevjumped = false;
    prevchoosed = false;
    prevlevelup = false;
    isLevelUpChoosing = false;

    // 폰트 로드
    if (!font.openFromFile("arial.ttf"))
    {
        // 실패해도 일단 진행 (나중에 로그 처리 추천)
    }

    // 제목 텍스트 생성
    levelUpTitle.emplace(font);
    levelUpTitle->setCharacterSize(40);
    levelUpTitle->setFillColor(sf::Color::Yellow);
    levelUpTitle->setString("LEVEL UP! Choose 1 / 2 / 3");
    levelUpTitle->setPosition({500.f, 250.f});

    // 선택지 텍스트 생성
    for (int i = 0; i < 3; i++)
    {
        optionTexts.emplace_back(font);
        optionTexts[i].setCharacterSize(30);
        optionTexts[i].setFillColor(sf::Color::White);
        optionTexts[i].setPosition({520.f, 350.f + i * 80.f});
    }
}

// 메인 루프
void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        handleInput();
        update(dt);
        render();
    }
}

// 입력 처리
void Game::handleInput()
{
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        window.close();

    bool curChoosed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) ||
                      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) ||
                      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3);
    // 레벨업 선택 중이면 입력 제한
    if (isLevelUpChoosing)
    {
        if (curChoosed && !prevchoosed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
                applyLevelUpOption(0);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
                applyLevelUpOption(1);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
                applyLevelUpOption(2);
        }
        prevchoosed = curChoosed;
        return;
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            player.moveLeft();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            player.moveRight();

        bool curSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

        if (curSpacePressed && !prevjumped)
            player.jump();

        prevjumped = curSpacePressed;
        bool curLPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L);
        // 테스트용 경험치
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) && !prevlevelup)
        {
            player.addExp(1);

            if (player.canLevelUp())
                openLevelUpMenu();
        }
        prevlevelup = curLPressed;
    }
}

// 업데이트
void Game::update(float dt)
{
    if (isLevelUpChoosing)
        return;

    player.update(dt);

    // 바닥 충돌
    if (player.getPosition().y + player.getSize().y >= 1150.f)
        player.land(1150.f);

    // 좌우 벽 제한
    if (player.getPosition().x <= 0)
    {
        float position[2] = {0.f, player.getPosition().y};
        player.setPosition(position);
    }

    if (player.getPosition().x + player.getSize().x >= 1600.f)
    {
        float position[2] = {1600.f - player.getSize().x, player.getPosition().y};
        player.setPosition(position);
    }

    // 몬스터 추적
    monster.update(player.getPosition());

    // 충돌
    if (monster.isAlive() &&
        player.getBounds().findIntersection(monster.getBounds()))
    {
        monster.takeDamage(static_cast<int>(player.getStat(StatType::Attack)));
    }
}

// 렌더링
void Game::render()
{
    window.clear();

    player.draw(window);
    monster.draw(window);
    window.draw(ground);

    // 레벨업 UI
    if (isLevelUpChoosing)
    {
        if (levelUpTitle.has_value())
            window.draw(*levelUpTitle);

        for (int i = 0; i < 3; i++)
            window.draw(optionTexts[i]);
    }

    window.display();
}

// 레벨업 메뉴 열기
void Game::openLevelUpMenu()
{
    std::vector<LevelUpOption> options = player.getLevelUpOptions();

    currentOptions[0] = options[0];
    currentOptions[1] = options[1];
    currentOptions[2] = options[2];

    optionTexts[0].setString("1. " + currentOptions[0].label);
    optionTexts[1].setString("2. " + currentOptions[1].label);
    optionTexts[2].setString("3. " + currentOptions[2].label);

    isLevelUpChoosing = true;
}

// 선택 적용
void Game::applyLevelUpOption(int index)
{
    player.levelUp(currentOptions[index]);
    isLevelUpChoosing = false;
}