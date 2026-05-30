#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <string>

class Game;

// 스탯 종류
enum class StatType
{
    Attack,
    MoveSpeed,
    JumpPower,
    MaxHp,
    Count
};

// 레벨업 선택지
struct LevelUpOption
{
    StatType type;
    float amount;
    std::string label;
};

class Player
{
private:
    // 스탯 배열
    std::array<float, static_cast<int>(StatType::Count)> stats;

    sf::RectangleShape shape;

    // 현재 상태값
    float velocityY = 0.f;
    float gravity = 0.8f;

    bool isGrounded = false;
    bool doubleJump = true;

    // 성장 관련
    int level = 1;
    int exp = 0;
    int needExp = 3;

    // 플레이어 체력
    int hp = 100;

    // 피격 무적시간
    bool isInvincible = false;
    float invincibleTimer = 0.f;
    float invincibleDuration = 1.0f;

    // 공격
    bool isAttacking = false;
    float attackTimer = 0.f;
    float attackDuration = 0.15f;

    int direction = 1; // 1: 오른쪽, -1: 왼쪽
    sf::RectangleShape attackBox;

public:
    void init();

    // 이동
    void moveLeft();
    void moveRight();
    void jump();

    // 공격
    void attack();
    bool getIsAttacking();
    sf::FloatRect getAttackBounds();

    // 피격 이벤트
    void takeDamage(int dmg);
    bool getIsInvincible();

    // 업데이트
    void update(float dt);
    void land(float groundY);

    // 경험치 / 레벨업
    void addExp(int amount);
    bool canLevelUp();
    void levelUp(const LevelUpOption &option);
    std::vector<LevelUpOption> getLevelUpOptions();

    // getter
    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    sf::FloatRect getBounds();
    float getStat(StatType type);
    int getLevel();
    int getExp();
    int getNeedExp();
    int getHp();
    int getMaxHp();

    // setter
    void setPosition(float position[2]);

    // 렌더링
    void draw(sf::RenderWindow &window);
};