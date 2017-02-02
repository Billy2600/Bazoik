#pragma once
#include <SFML/Graphics.hpp>
#include "game.h"
#include "input_manager.h"

class GameState
{
protected:

public:
	Game *game;

	virtual void Draw() const = 0;
	virtual void Update( const float dt ) = 0;
	virtual void HandleInput() = 0;
};