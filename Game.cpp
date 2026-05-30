#include "Game.h"
#include <optional>
#include <algorithm>
#include <random>

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

    // 몬스터 스폰 타이머
    spawnTimer = 0.f;

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

    // =======================
    // UI 패널
    // =======================
    statusPanel.setSize({360.f, 130.f});
    statusPanel.setFillColor(sf::Color(30, 30, 30, 200));
    statusPanel.setOutlineColor(sf::Color::White);
    statusPanel.setOutlineThickness(2.f);
    statusPanel.setPosition({20.f, 20.f});

    // =======================
    // HP Bar
    // =======================
    hpBarBack.setSize({300.f, 22.f});
    hpBarBack.setFillColor(sf::Color(80, 80, 80));
    hpBarBack.setPosition({45.f, 75.f});

    hpBarFront.setSize({300.f, 22.f});
    hpBarFront.setFillColor(sf::Color::Red);
    hpBarFront.setPosition({45.f, 75.f});

    // =======================
    // EXP Bar
    // =======================
    expBarBack.setSize({300.f, 18.f});
    expBarBack.setFillColor(sf::Color(80, 80, 80));
    expBarBack.setPosition({45.f, 120.f});

    expBarFront.setSize({300.f, 18.f});
    expBarFront.setFillColor(sf::Color::Blue);
    expBarFront.setPosition({45.f, 120.f});

    // =======================
    // UI Text
    // =======================
    levelText.emplace(font);
    levelText->setCharacterSize(22);
    levelText->setFillColor(sf::Color::White);
    levelText->setPosition({45.f, 35.f});

    hpText.emplace(font);
    hpText->setCharacterSize(18);
    hpText->setFillColor(sf::Color::White);
    hpText->setPosition({45.f, 52.f});

    expText.emplace(font);
    expText->setCharacterSize(16);
    expText->setFillColor(sf::Color::White);
    expText->setPosition({45.f, 100.f});
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
    }

    bool curAttack = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J);

    if (curAttack && !prevAttack)
    {
        player.attack();
    }

    prevAttack = curAttack;
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

    // 몬스터 스폰
    spawnTimer += dt;

    if (spawnTimer >= spawnInterval)
    {
        Monster m;
        m.init();

        // 랜덤 위치 (오른쪽에서 등장)
        float x = 1200.f + rand() % 200;
        m.update({x, 1100.f}, 0.f, player); // 초기 위치 맞추기용

        monsters.push_back(m);
        spawnTimer = 0.f;
    }

    // =======================
    // 몬스터 업데이트
    // =======================
    for (auto &m : monsters)
    {
        m.update(player.getPosition(), dt, player);
    }

    // 충돌
    for (auto &m : monsters)
    {
        if (player.getIsAttacking() &&
            m.isAlive() &&
            player.getAttackBounds().findIntersection(m.getBounds()))
        {
            int damage = static_cast<int>(player.getStat(StatType::Attack));
            m.takeDamage(damage);
            addDamageText(damage, m.getPosition());
        }
    }

    for (int i = 0; i < monsters.size(); i++)
    {
        if (!monsters[i].isAlive())
        {
            player.addExp(1); // 경험치 증가
            if (player.canLevelUp())
            {
                openLevelUpMenu();
                return;
            }
            monsters.erase(monsters.begin() + i);
            i--;
        }
    }

    // =======================
    // UI 갱신
    // =======================
    float hpRatio = static_cast<float>(player.getHp()) / player.getMaxHp();
    hpBarFront.setSize({300.f * hpRatio, 22.f});

    float expRatio = static_cast<float>(player.getExp()) / player.getNeedExp();
    expBarFront.setSize({300.f * expRatio, 18.f});

    if (levelText.has_value())
    {
        levelText->setString("LV. " + std::to_string(player.getLevel()));
    }

    if (hpText.has_value())
    {
        hpText->setString(
            "HP  " + std::to_string(player.getHp()) +
            " / " + std::to_string(player.getMaxHp()));
    }

    if (expText.has_value())
    {
        expText->setString(
            "EXP " + std::to_string(player.getExp()) +
            " / " + std::to_string(player.getNeedExp()));
    }

    for (int i = 0; i < damageTexts.size(); i++)
    {
        damageTexts[i].timer += dt;

        // 위로 떠오르게 이동
        damageTexts[i].text.move({0.f, -50.f * dt});

        if (damageTexts[i].timer >= damageTexts[i].duration)
        {
            damageTexts.erase(damageTexts.begin() + i);
            i--;
        }
    }
}

// 렌더링
void Game::render()
{
    window.clear();

    player.draw(window);

    for (auto &m : monsters)
    {
        m.draw(window);
    }

    window.draw(ground);

    // UI
    window.draw(statusPanel);

    window.draw(hpBarBack);
    window.draw(hpBarFront);

    window.draw(expBarBack);
    window.draw(expBarFront);

    if (levelText.has_value())
        window.draw(*levelText);

    if (hpText.has_value())
        window.draw(*hpText);

    if (expText.has_value())
        window.draw(*expText);

    // 레벨업 UI
    if (isLevelUpChoosing)
    {
        if (levelUpTitle.has_value())
            window.draw(*levelUpTitle);

        for (int i = 0; i < 3; i++)
            window.draw(optionTexts[i]);
    }

    for (auto &damageText : damageTexts)
    {
        window.draw(damageText.text);
    }

    window.display();
}

// 레벨업 메뉴 열기
void Game::openLevelUpMenu()
{
    std::vector<LevelUpOption> options = player.getLevelUpOptions();

    // 랜덤 엔진 생성
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // 선택지 섞기
    std::shuffle(options.begin(), options.end(), gen);

    // 앞에서 3개 선택
    currentOptions[0] = options[0];
    currentOptions[1] = options[1];
    currentOptions[2] = options[2];

    // UI 텍스트 갱신
    optionTexts[0].setString("1. " + currentOptions[0].label);
    optionTexts[1].setString("2. " + currentOptions[1].label);
    optionTexts[2].setString("3. " + currentOptions[2].label);

    isLevelUpChoosing = true;
}

void Game::addDamageText(int damage, sf::Vector2f position)
{
    DamageText damageText{sf::Text(font)};

    damageText.text.setCharacterSize(24);
    damageText.text.setFillColor(sf::Color::Yellow);
    damageText.text.setString(std::to_string(damage));
    damageText.text.setPosition(position);

    damageTexts.push_back(damageText);
}

// 선택 적용
void Game::applyLevelUpOption(int index)
{
    player.levelUp(currentOptions[index]);
    isLevelUpChoosing = false;
}