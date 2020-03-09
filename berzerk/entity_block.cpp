#include "entity_block.h"

EntityBlock::EntityBlock(const sf::Vector2f pos, const BlockTypes type, const bool pushable, const Directions pushableDirection)
{
	this->hitbox.left = pos.x;
	this->hitbox.top = pos.y;
	this->hitbox.width = 32.f;
	this->hitbox.height = 32.f;

	this->type = type;
	this->pushable = pushable;
	this->pushableDirection = pushableDirection;

#ifdef _DEBUG
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setPosition(pos);
	shape.setSize(sf::Vector2f(hitbox.width,hitbox.width));
#endif
}

void EntityBlock::SetupSprite()
{
	sprite.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
	sprite.setTexture(game->assetManager.GetTextureRef("sprites"));

	switch (type)
	{
	case BlockTypes::Regular:
		sprite.setTextureRect(animManager.Animate("block", true));
		break;
	case BlockTypes::Inverted:
		sprite.setTextureRect(animManager.Animate("block_inverted", true));
		break;
	case BlockTypes::Water:
		sprite.setTextureRect(animManager.Animate("water", true));
		break;
	case BlockTypes::Hole:
		sprite.setTextureRect(animManager.Animate("hole", true));
		break;
	case BlockTypes::FaceRightLight:
		sprite.setTextureRect(animManager.Animate("face_right_light", true));
		break;
	case BlockTypes::FaceRightDark:
		sprite.setTextureRect(animManager.Animate("face_right_dark", true));
		break;
	case BlockTypes::FaceLeftLight:
		sprite.setTextureRect(animManager.Animate("face_left_light", true));
		break;
	case BlockTypes::FaceLeftDark:
		sprite.setTextureRect(animManager.Animate("face_left_dark", true));
		break;
	}
}

void EntityBlock::Think(const float dt)
{
	if (sprite.getTexture() == NULL)
	{
		SetupSprite();
	}
}

void EntityBlock::Draw() const
{
	game->window.draw(sprite);
#ifdef _DEBUG
	game->window.draw(shape);
#endif // _DEBUG

}

void EntityBlock::Move(sf::Vector2f move, const float dt)
{
	hitbox.left += move.x * dt;
	hitbox.top += move.y * dt;
	sprite.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
#ifdef _DEBUG
	shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
#endif // _DEBUG

}

void EntityBlock::HandleCollision(Entity* other)
{

}