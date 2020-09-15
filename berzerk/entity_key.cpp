#include "entity_key.h"
#include "entity_player.h"

EntityKey::EntityKey(sf::Vector2f pos, bool isRing)
{
	hitbox.top = pos.y;
	hitbox.left = pos.x;
	sprite.setPosition(pos);
	keyAlreadyCollectedCheck = false;
	this->isRing = isRing;

#ifdef _DEBUG
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setFillColor(sf::Color::Transparent);
	shape.setPosition(pos);
#endif

}

void EntityKey::Think(const float dt)
{
	if (sprite.getTexture() == NULL)
	{
		sprite.setTexture(game->assetManager.GetTextureRef("sprites"));
		const sf::IntRect animRect = animManager.Animate(isRing ? "ring" : "key");
		sprite.setTextureRect(animRect);
		hitbox.width = animRect.width;
		hitbox.height = animRect.height;
#ifdef _DEBUG
		shape.setSize(sf::Vector2f(animRect.width, animRect.height));
#endif
	}

	if (KeyAlreadyCollected())
		deleteMe = true;
}

void EntityKey::Draw() const
{
	game->window.draw(sprite);
#ifdef _DEBUG
	game->window.draw(shape);
#endif

}

void EntityKey::HandleCollision(Entity* other)
{

	if (dynamic_cast<EntityPlayer*>(other) != NULL && game != NULL)
	{
		game->AddKey();
		deleteMe = true;
	}
}

void EntityKey::Move(sf::Vector2f move, const float dt)
{
	const sf::Vector2f moveVec = sf::Vector2f(move.x * dt, move.y * dt);
	hitbox.left += moveVec.x;
	hitbox.top += moveVec.y;
	sprite.setPosition(sprite.getPosition() + moveVec);
#ifdef _DEBUG
	shape.setPosition(shape.getPosition() + moveVec);
#endif
}

bool EntityKey::KeyAlreadyCollected()
{
	if (!keyAlreadyCollectedCheck)
	{
		keyAlreadyCollectedCheck = true;

		if (game->keysCollected.empty())
			return false;

		return std::find(game->keysCollected.begin(), game->keysCollected.end(), game->currentRoom) != game->keysCollected.end();
	}

	return false;
}