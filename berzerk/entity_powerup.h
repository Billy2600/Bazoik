#pragma once
#include "entity.h"
#include "powerup_types.h"

class EntityPowerup : public Entity
{
private:
    sf::RectangleShape shape;

    void ChooseColor();

public:
    PowerupType type;

    EntityPowerup(sf::Vector2f position, PowerupType type);

    void Think(const float dt) override;

    void Draw() const override;

    void HandleCollision(Entity *other) override;

    void Move(sf::Vector2f move, const float dt) override;


};