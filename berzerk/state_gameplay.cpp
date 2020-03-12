#include "state_gameplay.h"
#include "state_titlescreen.h"
#include "pugixml.hpp"

Directions StateGameplay::lastMove = Directions::S;

StateGameplay::StateGameplay( Game *game, const bool recordDemo , const bool playDemo, const std::string demoName )
{
	this->game = game;

	this->game->SetRoomVisited(game->currentRoom.x, game->currentRoom.y);

	player.SetPos( GetPlayerStart(lastMove, player) );
	transition = false;

	// Init entities 
	entityManager.game = game;
	entityManager.Add( &player );
	room = Room(game->currentRoom, &entityManager);
	room.SetupRoom();

	LoadBackgroundAndUiElements();

	clock.restart();

	this->recordDemo = recordDemo;
	this->playDemo = playDemo;
	if ( playDemo )
	{
		demo.LoadFromFile( demoName );
		//game->level = demo.GetLevel();
	}
	if ( recordDemo )
	{
		//demo.SetLevel( game->level );
	}

	pause.SetGame( game );
	pauseTime = 0;

	if ( game->music.getStatus() != sf::Music::Status::Playing )
	{
		if (!game->music.openFromFile("assets/music.wav"))
		{
			ErrorLog log;
			log.Write("Could not open music file music.wav");
		}
		else
		{
			game->music.play();
		}
	}
}

void StateGameplay::LoadBackgroundAndUiElements()
{
	AssetManager* assetManager = &this->game->assetManager;
	background = sf::Sprite(assetManager->GetTextureRef("background"));

	for (int i = 0; i < MAX_HP; i++)
	{
		uiHitPoint[i] = sf::Sprite(assetManager->GetTextureRef("sprites"));

		if (i % 2 == 0)
		{
			uiHitPoint[i].setTextureRect(animManager.Animate("heart_left"));
		}
		else
		{
			uiHitPoint[i].setTextureRect(animManager.Animate("heart_right"));
		}

		uiHitPoint[i].setPosition(4 + ((uiHitPoint[i].getGlobalBounds().width) * i), 5);
	}
	
	uiKey = sf::Sprite(assetManager->GetTextureRef("sprites"));
	uiKey.setTextureRect(animManager.Animate("key"));
	uiKey.setScale(sf::Vector2f(0.5f, 0.5f));
	uiKey.setPosition(sf::Vector2f(uiHitPoint[MAX_HP - 1].getPosition().x + uiHitPoint[MAX_HP - 1].getLocalBounds().width + 16, 5));

	uiNumKeys = sf::Text("x0", assetManager->GetFontRef("joystix"), 16);
	uiNumKeys.setFont(assetManager->GetFontRef("joystix"));
	uiNumKeys.setCharacterSize(16);
	uiNumKeys.setPosition(uiKey.getPosition().x + 8, 5);
	uiNumKeys.setFillColor(sf::Color::White);

	for (int x = 0; x < MAX_ROOM_X; x++)
	{
		for (int y = 0; y < MAX_ROOM_Y; y++)
		{
			uiMap[x][y] = sf::RectangleShape(sf::Vector2f(10, 5));
			if(game->GetRoomVisited(x, y)) uiMap[x][y].setFillColor(sf::Color(255, 0, 0, 150));
			else uiMap[x][y].setFillColor(sf::Color(0, 0, 0, 150));
			uiMap[x][y].setPosition(sf::Vector2f(
				330 + (11 * x),
				5 + (6 * y)
			));
		}
	}
}

void StateGameplay::Start()
{
	
}

void StateGameplay::HandleInput()
{
	// Show or hide cursor based on whether the pause menu is open
	game->window.setMouseCursorVisible( pause.open );

	if ( pause.open )
	{
		pause.HandleInput();
		if ( pause.quit )
		{
			game->music.stop();
			ReturnToTitle();
			return;
		}

		// Prevent controls from getting stuck when pause menu is opened
		input.right = false;
		input.left = false;
		input.up = false;
		input.down = false;
		input.fire = false;
		return;
	}

	sf::Event event;

	while( game->window.pollEvent( event ) )
	{
		// Close window
		if ( event.type == sf::Event::Closed )
		{
			game->Close();
			return;
		}

		if ( ResetIfDead() )
			return; // Do not continue if we're resetting the state

		if( event.type == sf::Event::KeyPressed )
		{
			if ( event.key.code == sf::Keyboard::Escape || game->inputManager.TestKeyDown( "pause", event ) )
			{
				if ( recordDemo || playDemo )
				{
					ReturnToTitle();
				}
				else
				{
					pauseTime = clock.getElapsedTime().asMilliseconds();
					pause.open = true; // Pause menu will close itself
				}
			}

			if( event.key.code == sf::Keyboard::Key::F11 )
			{
				transition = true;
				lastMove = Directions::N;
			}
			if( event.key.code == sf::Keyboard::Key::F10 )
			{
				this->game->SwitchState( new StateGameplay( this->game ) );
				break;
			}
			if( event.key.code == sf::Keyboard::Key::F9 )
			{
				demo.SaveToFile( game->GetConfigDir() + "demo1.xml" );
			}

			if ( event.key.code == sf::Keyboard::Key::Equal )
			{
				game->music.setVolume( game->music.getVolume() + 10 );
			}

			if ( event.key.code == sf::Keyboard::Key::Dash )
			{
				game->music.setVolume( game->music.getVolume() - 10 );
			}
		}

		if( event.type == sf::Event::MouseButtonPressed )
		{
			/*if( event.mouseButton.button == sf::Mouse::Left )
			{
				input.fire = true;
			}*/
		}

		if( event.type == sf::Event::MouseButtonReleased )
		{
			/*if( event.mouseButton.button == sf::Mouse::Left )
			{
				input.fire = false;
			}*/
		}

		// Early out if we're in the middle of a transition
		if( transition )
		{
			return;
		}

		// Keys presed
		if( game->inputManager.TestKeyDown( "right", event ) ) input.right = true;

		if( game->inputManager.TestKeyDown( "left", event ) ) input.left = true;

		if( game->inputManager.TestKeyDown( "up", event ) ) input.up = true;

		if( game->inputManager.TestKeyDown( "down", event ) ) input.down = true;

		if( game->inputManager.TestKeyDown( "fire", event ) ) input.fire = true;

		// Keys released
		if( game->inputManager.TestKeyUp( "right", event ) ) input.right = false;

		if( game->inputManager.TestKeyUp( "left", event ) ) input.left = false;

		if( game->inputManager.TestKeyUp( "up", event ) ) input.up = false;
		
		if( game->inputManager.TestKeyUp( "down", event ) ) input.down = false;

		if( game->inputManager.TestKeyUp( "fire", event ) ) input.fire = false;
	}

	if( playDemo && !recordDemo )
	{
		player.SetInput( demo.Play() );
		// Return to title screen if demo is done
		if ( demo.IsDone() )
		{
			ReturnToTitle();
			return;
		}
	}
	else if( !playDemo )
	{
		player.SetInput( input );
		if( recordDemo ) // This is here so you can't record while playing a demo
			demo.Record( input );
	}
}

void StateGameplay::Update( const float dt )
{
	if ( pause.open )
	{
		clock.restart();
		return;
	}

	if( !transition )
	{
		entityManager.Think( dt );
		entityManager.CheckCollisions();

		// Begin screen transition if player moves outside screen
		sf::Vector2f plPos( player.hitbox.left, player.hitbox.top );
		sf::Int32 now = clock.getElapsedTime().asMilliseconds();
		bool startTrans = false;

		// Decide if we should start transition
		if( plPos.x > GAME_WIDTH )
		{
			lastMove = Directions::W;
			startTrans = true;
		}
		else if( ( plPos.x + player.hitbox.width ) < 0 )
		{
			lastMove = Directions::E;
			startTrans = true;
		}
		else if( plPos.y > GAME_HEIGHT )
		{
			lastMove = Directions::N;
			startTrans = true;
		}
		else if( ( plPos.y + player.hitbox.height ) < 0 )
		{
			lastMove = Directions::S;
			startTrans = true;
		}

		// Kick off transition
		if ( startTrans )
		{
			transition = true;
			transStart = now;
		}

		if (game != NULL)
		{
			uiNumKeys.setString( "x" + std::to_string(game->GetKeys()) );
		}
	}
	else
	{
		ScreenTransition(dt);
	}
}

void StateGameplay::Draw() const
{
	game->window.draw(background);

	entityManager.Draw();


	for (int i = 0; i < game->GetHitPoints(); i++)
	{
		if (game->GetHitPoints() == 0)
			break;

		game->window.draw(uiHitPoint[i]);
	}

	game->window.draw(uiKey);
	game->window.draw(uiNumKeys);

	for (int x = 0; x < MAX_ROOM_X; x++)
	{
		for (int y = 0; y < MAX_ROOM_Y; y++)
		{
			game->window.draw(uiMap[x][y]);
		}
	}

	if ( pause.open )
		pause.Draw();
}

void StateGameplay::ScreenTransition( const float dt )
{
	sf::Vector2f move;
	sf::Vector2i nextRoom = game->currentRoom;

	switch( lastMove )
	{
	case Directions::N:
		move = sf::Vector2f( 0, -VERT_TRANS_SPEED );
		nextRoom.y++;
		break;
	case Directions::E:
		move = sf::Vector2f( HORZ_TRANS_SPEED, 0 );
		nextRoom.x--;
		break;
	case Directions::S:
		move = sf::Vector2f( 0, VERT_TRANS_SPEED );
		nextRoom.y--;
		break;
	case Directions::W:
		move = sf::Vector2f( -HORZ_TRANS_SPEED, 0 );
		nextRoom.x++;
		break;
	}

	transBoundry.width = GAME_WIDTH;
	transBoundry.height = GAME_HEIGHT;
	transBoundry.left += move.x * dt;
	transBoundry.top += move.y * dt;

	player.SetPos( sf::Vector2f( -1000, -1000 ) ); // Move player off of screen

	auto backgroundPos = background.getPosition();
	background.setPosition(sf::Vector2f(backgroundPos.x + (move.x * dt), backgroundPos.y + (move.y * dt)));

	entityManager.MoveAllEntities( move, dt );

	// Check for done
	if ( ( transBoundry.left + transBoundry.width ) < 0 || transBoundry.left > GAME_WIDTH ||
		( transBoundry.top + transBoundry.height ) < 0 || transBoundry.top > GAME_HEIGHT )
	{
		this->game->currentRoom = nextRoom;
		this->game->SwitchState( new StateGameplay( this->game ) );
	}
}

bool StateGameplay::ResetIfDead()
{
	if ( player.CheckReset() )
	{
		if ( playDemo )
		{
			ReturnToTitle();
		}
		else
		{
			if ( recordDemo )
			{
				PlayerInput last;
				last.down = true;
				demo.Record( last ); // Add input on current frame, so it doesn't end too early 
				demo.SaveToFile( game->GetConfigDir() + "demo1.xml" );
			}
			game->ResetState();
		}
		return true;
	}
	return false;
}

void StateGameplay::ReturnToTitle()
{
	game->window.setMouseCursorVisible( true );
	lastMove = Directions::S;
	//game->music.stop();
	game->PopState();
	game->ResetVisitedRooms();
}

sf::Vector2f StateGameplay::GetPlayerStart(const Directions lastMove, EntityPlayer& player) const
{
	// Spawn player based on last move
	// Keep in mind you'll end up on the other side that you exited
	switch (lastMove)
	{
	case Directions::N:
		return sf::Vector2f(240, 48);
		break;
	case Directions::S:
		return sf::Vector2f(230, 240);
		break;
	case Directions::W:
		return sf::Vector2f(48, 144);
		break;
	case Directions::E:
	default:
		return sf::Vector2f(400, 144);
		break;
	}

	// Should never happen
	return sf::Vector2f(0, 0);
}

StateGameplay::~StateGameplay()
{
	
}