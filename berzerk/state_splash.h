#pragma once
#include "game_state.h"

class StateSplash : public GameState
{
private:
	void LoadSounds(); // Load all sounds
	bool graphicShown;
	sf::Texture tex;
	sf::Sprite graphic;

public:

	StateSplash( Game *game );
	virtual void Draw() const;
	virtual void Update( const float dt );
	virtual void HandleInput();
};