#include "Player.h"

// enum class를 배열 인덱스로 바꾸기 위한 함수
static int toIndex(StatType type)
{
    return static_cast<int>(type);
}

// 초기화
void Player::init()
{
    shape.setSize({50.f, 50.f});
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({100.f, 400.f});

    // 기본 스탯 설정
    stats[toIndex(StatType::Attack)] = 1.f;
    stats[toIndex(StatType::MoveSpeed)] = 5.f;
    stats[toIndex(StatType::JumpPower)] = -15.f;
    stats[toIndex(StatType::MaxHp)] = 100.f;

    // 공격 범위
    attackBox.setSize({70.f, 50.f});
    attackBox.setFillColor(sf::Color(255, 255, 0, 120));

    hp = static_cast<int>(stats[toIndex(StatType::MaxHp)]);
}

// 왼쪽 이동
void Player::moveLeft()
{
    direction = -1;
    shape.move({-stats[toIndex(StatType::MoveSpeed)], 0.f});
}
// 오른쪽 이동
void Player::moveRight()
{
    direction = 1;
    shape.move({stats[toIndex(StatType::MoveSpeed)], 0.f});
}

// 점프
void Player::jump()
{
    if (isGrounded)
    {
        velocityY = stats[toIndex(StatType::JumpPower)];
        isGrounded = false;
    }
    else if (doubleJump)
    {
        velocityY = stats[toIndex(StatType::JumpPower)];
        doubleJump = false;
    }
}

// 중력 적용
void Player::update(float dt)
{
    velocityY += gravity;
    shape.move({0.f, velocityY});

    // 무적시간 갱신
    if (isInvincible)
    {
        invincibleTimer += dt;

        if (invincibleTimer >= invincibleDuration)
        {
            isInvincible = false;
            invincibleTimer = 0.f;
            shape.setFillColor(sf::Color::Green);
        }
    }

    // 공격 시간 갱신
    if (isAttacking)
    {
        attackTimer += dt;

        if (attackTimer >= attackDuration)
        {
            isAttacking = false;
            attackTimer = 0.f;
        }
    }
}

void Player::attack()
{
    if (isAttacking)
        return;

    isAttacking = true;
    attackTimer = 0.f;

    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f size = shape.getSize();

    if (direction == 1)
        attackBox.setPosition({pos.x + size.x, pos.y});
    else
        attackBox.setPosition({pos.x - attackBox.getSize().x, pos.y});
}

bool Player::getIsAttacking()
{
    return isAttacking;
}

sf::FloatRect Player::getAttackBounds()
{
    return attackBox.getGlobalBounds();
}
// 착지
void Player::land(float groundY)
{
    shape.setPosition({shape.getPosition().x, groundY - shape.getSize().y});
    velocityY = 0.f;
    isGrounded = true;
    doubleJump = true;
}

// 경험치 획득
void Player::addExp(int amount)
{
    exp += amount;
}

// 레벨업 가능한지 확인
bool Player::canLevelUp()
{
    return exp >= needExp;
}

// 레벨업 선택지 반환
std::vector<LevelUpOption> Player::getLevelUpOptions()
{
    return {
        {StatType::Attack, 1.f, "Attack +1"},
        {StatType::MoveSpeed, 1.f, "MoveSpeed +1"},
        {StatType::JumpPower, -2.f, "JumpPower +2"},
        {StatType::MaxHp, 10.f, "MaxHp +10"}};
}

// 실제 레벨업 적용
void Player::levelUp(const LevelUpOption &option)
{
    // 경험치 차감
    exp = 0;

    // 레벨 증가
    level++;

    // 필요 경험치 증가
    needExp += 2;

    // 선택한 스탯 증가
    stats[toIndex(option.type)] += option.amount;
}

// getter
sf::Vector2f Player::getPosition()
{
    return shape.getPosition();
}

sf::Vector2f Player::getSize()
{
    return shape.getSize();
}

sf::FloatRect Player::getBounds()
{
    return shape.getGlobalBounds();
}

float Player::getStat(StatType type)
{
    return stats[toIndex(type)];
}

int Player::getLevel()
{
    return level;
}

int Player::getExp()
{
    return exp;
}

int Player::getNeedExp()
{
    return needExp;
}

void Player::setPosition(float position[2])
{
    shape.setPosition({position[0], position[1]});
}

// 렌더링
void Player::draw(sf::RenderWindow &window)
{
    window.draw(shape);

    // 공격 중일 때만 공격 범위 표시
    if (isAttacking)
        window.draw(attackBox);
}

void Player::takeDamage(int dmg)
{
    // 무적 상태면 데미지 무시
    if (isInvincible)
        return;

    hp -= dmg;

    if (hp < 0)
        hp = 0;

    // 맞은 직후 무적 시작
    isInvincible = true;
    invincibleTimer = 0.f;

    // 피격 표시
    shape.setFillColor(sf::Color::Cyan);
}

bool Player::getIsInvincible()
{
    return isInvincible;
}

int Player::getHp()
{
    return hp;
}

int Player::getMaxHp()
{
    return static_cast<int>(stats[toIndex(StatType::MaxHp)]);
}