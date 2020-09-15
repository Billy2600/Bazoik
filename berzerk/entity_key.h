#pragma once
#include "entity.h"

class EntityKey : public Entity
{
private:
	bool isRing;
	sf::Sprite sprite;
	AnimManager animManager;
#ifdef  _DEBUG
	sf::RectangleShape shape;
#endif //  _DEBUG

	bool keyAlreadyCollectedCheck;
	bool KeyAlreadyCollected();

public:
	EntityKey(sf::Vector2f pos, bool isRing = false);
	virtual void Think(const float dt);
	virtual void Draw() const;
	virtual void HandleCollision(Entity* other);
	virtual void Move(sf::Vector2f move, const float dt);
};