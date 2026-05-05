#include "Monster.h"
#include <cmath>

// 초기화
void Monster::init()
{
    shape.setSize({50.f, 50.f});
    shape.setFillColor(sf::Color::Red);
    shape.setPosition({1200.f, 1100.f});
}

// 플레이어 추적
void Monster::update(sf::Vector2f playerPos)
{
    if (!alive)
        return;

    float dx = playerPos.x - shape.getPosition().x;

    if (std::abs(dx) > 50.f)
    {
        if (dx > 0)
            shape.move({speed, 0.f});
        else
            shape.move({-speed, 0.f});
    }
}

// 데미지 처리
void Monster::takeDamage(int dmg)
{
    if (!alive)
        return;

    hp -= dmg;

    if (hp <= 0)
    {
        alive = false;
    }
}

bool Monster::isAlive()
{
    return alive;
}

sf::FloatRect Monster::getBounds()
{
    return shape.getGlobalBounds();
}

// 렌더링
void Monster::draw(sf::RenderWindow &window)
{
    if (alive)
        window.draw(shape);
}