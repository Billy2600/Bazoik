#pragma once
#include <SFML/Graphics.hpp>
#include "game.h"
#include "input_manager.h"
#include "error_log.h"

class GameState
{
protected:
	ErrorLog log;

public:
	Game *game;

	virtual void Start() = 0;
	virtual void Draw() const = 0;
	virtual void Update( const float dt ) = 0;
	virtual void HandleInput() = 0;
};