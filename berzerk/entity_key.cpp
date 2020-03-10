#include "entity_key.h"
#include "entity_player.h"

EntityKey::EntityKey(sf::Vector2f pos)
{
	hitbox.top = pos.y;
	hitbox.left = pos.x;
	sprite.setPosition(pos);
}

void EntityKey::Think(const float dt)
{
	if (sprite.getTexture() == NULL)
	{
		sprite.setTexture(game->assetManager.GetTextureRef("sprites"));
		sprite.setTextureRect(animManager.Animate("key"));
	}
}

void EntityKey::Draw() const
{
	game->window.draw(sprite);
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

}