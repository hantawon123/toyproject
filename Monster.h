#pragma once
#include <SFML/Graphics.hpp>

// 최소 의존성 유지(빠른 컴파일) 어차피 구현은 cpp파일에서
// forward declaration
class Player;

// 몬스터 상태
enum class MonsterState
{
    Idle,
    Chase,
    Attack,
    Dead
};

class Monster
{
private:
    sf::RectangleShape shape;

    MonsterState state = MonsterState::Idle;

    float speed = 2.f;
    int hp = 3;
    int maxHp = 3;

    // 공격 관련 변수
    float attackCooldown = 1.0f;
    float attackTimer = 0.f;
    int attackDamage = 10;

    float detectRange = 500.f; // 추적 시작 거리
    float attackRange = 60.f;  // 공격 가능 거리

    // HP Bar
    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarFront;

public:
    void init();

    void update(sf::Vector2f playerPos, float dt, Player &player);
    void attack(Player &player);
    void updateState(sf::Vector2f playerPos);

    void idle();
    void chase(sf::Vector2f playerPos);
    void attack();

    sf::Vector2f getPosition();
    void takeDamage(int dmg);

    bool isAlive();

    sf::FloatRect getBounds();

    void updateHpBar();

    void draw(sf::RenderWindow &window);
};