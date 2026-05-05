#include "Monster.h"
#include "Player.h"

#include <cmath>

// 초기화
void Monster::init()
{
    shape.setSize({50.f, 50.f});
    shape.setFillColor(sf::Color::Red);
    shape.setPosition({1200.f, 1100.f});

    state = MonsterState::Idle;
    hp = maxHp;
}

// 매 프레임 업데이트
void Monster::update(sf::Vector2f playerPos, float dt, Player &player)
{
    if (state == MonsterState::Dead)
        return;

    attackTimer += dt;

    updateState(playerPos);

    switch (state)
    {
    case MonsterState::Idle:
        idle();
        break;

    case MonsterState::Chase:
        chase(playerPos);
        break;

    case MonsterState::Attack:
        attack(player);
        break;

    case MonsterState::Dead:
        break;
    }
}
// 공격
void Monster::attack(Player &player)
{
    if (attackTimer >= attackCooldown)
    {
        player.takeDamage(attackDamage);
        attackTimer = 0.f;
    }
}
// 상태 변경
void Monster::updateState(sf::Vector2f playerPos)
{
    float dx = playerPos.x - shape.getPosition().x;
    float distance = std::abs(dx);

    if (hp <= 0)
    {
        state = MonsterState::Dead;
    }
    else if (distance <= attackRange)
    {
        state = MonsterState::Attack;
    }
    else if (distance <= detectRange)
    {
        state = MonsterState::Chase;
    }
    else
    {
        state = MonsterState::Idle;
    }
}

// 대기 상태
void Monster::idle()
{
    // 일단 아무 행동 안 함
}

// 추적 상태
void Monster::chase(sf::Vector2f playerPos)
{
    float dx = playerPos.x - shape.getPosition().x;

    if (dx > 0)
        shape.move({speed, 0.f});
    else
        shape.move({-speed, 0.f});
}

// 데미지 처리
void Monster::takeDamage(int dmg)
{
    if (state == MonsterState::Dead)
        return;

    hp -= dmg;

    if (hp <= 0)
    {
        hp = 0;
        state = MonsterState::Dead;
    }
}

// 생존 여부
bool Monster::isAlive()
{
    return state != MonsterState::Dead;
}

// 충돌 범위
sf::FloatRect Monster::getBounds()
{
    return shape.getGlobalBounds();
}

// 렌더링
void Monster::draw(sf::RenderWindow &window)
{
    if (state != MonsterState::Dead)
        window.draw(shape);
}