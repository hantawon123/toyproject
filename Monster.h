#pragma once
#include <SFML/Graphics.hpp>

class Monster
{
private:
    sf::RectangleShape shape;

    float speed = 2.f;
    int hp = 3;
    int maxHp = 3;

    bool alive = true;

public:
    void init();

    void update(sf::Vector2f playerPos);
    void takeDamage(int dmg);

    bool isAlive();

    sf::FloatRect getBounds();

    void draw(sf::RenderWindow &window);
};