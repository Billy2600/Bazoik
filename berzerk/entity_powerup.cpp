#include "entity_powerup.h"


EntityPowerup::EntityPowerup(sf::Vector2f position, PowerupType type)
{
    const sf::Vector2f size(32.f, 32.f);

    this->type = type;
    shape.setSize(size);
    ChooseColor();
    shape.setPosition(position);
    hitbox.left = position.x;
    hitbox.top = position.y;
    hitbox.width = size.x;
    hitbox.height = size.y;
}

void EntityPowerup::ChooseColor()
{
    switch (type)
    {
    case PowerupSpreadshot:
        shape.setFillColor(sf::Color::Green);
        break;
    case PowerupRicochet:
        shape.setFillColor(sf::Color::Cyan);
        break;
    default:
        shape.setFillColor(sf::Color::Yellow);
        break;
    }
}

void EntityPowerup::Think(const float dt)
{
    // Powerups don't do anything on their own, so this is empty for now
}

void EntityPowerup::Draw() const
{
    game->window.draw(shape);
}

void EntityPowerup::HandleCollision(Entity *other)
{

}

void EntityPowerup::Move(sf::Vector2f move, const float dt)
{
    // Powerups don't move on their own, but we need to implement this to move with the screen during transitions
    hitbox.left += move.x * dt;
    hitbox.top += move.y * dt;
    shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
}

