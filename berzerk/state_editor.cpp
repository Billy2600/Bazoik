#include "state_editor.h"

StateEditor::StateEditor(Game* game)
{
	this->game = game;


	spBackground.setTexture(game->assetManager.GetTextureRef("background"));
	spBackground.setPosition(0, 0);
	currentRoom = sf::Vector2i(0, 0);

	InitMenu();
	InitDoors();
}

void StateEditor::InitMenu()
{
	AssetManager* assetManager = &this->game->assetManager;

	showMenu = true;
	menuBg.setFillColor(sf::Color(0, 0, 0, 125));
	menuBg.setSize(sf::Vector2f(160, 320));
	menuBg.setPosition(sf::Vector2f(320, 0));

	buttons["menu_save"] = GuiButton(sf::Vector2f(328, 270), sf::Vector2f(140, 40), sf::Vector2f(10, 0), "Save", assetManager->GetFontRef("joystix"), 0);
	buttons["menu_save"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["menu_save"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["menu_save"].SetHighlight(false);

	buttons["menu_close"] = GuiButton(sf::Vector2f(450, 8), sf::Vector2f(20, 20), sf::Vector2f(3, 0), "X", assetManager->GetFontRef("joystix"), 0);
	buttons["menu_close"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["menu_close"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["menu_close"].SetHighlight(false);
	buttons["menu_close"].SetCharacterSize(15);

	for (int x = 0; x < MAX_ROOM_X; x++)
	{
		for (int y = 0; y < MAX_ROOM_Y; y++)
		{
			auto strCoord = std::to_string(x) + std::to_string(y);
			buttons["menu_room_" + strCoord] = GuiButton(sf::Vector2f(342 + (x * 9), 5 + (y * 9)), sf::Vector2f(3, 3), sf::Vector2f(0, 0), " ", assetManager->GetFontRef("joystix"), 0);
			buttons["menu_room_" + strCoord].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
			buttons["menu_room_" + strCoord].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
			buttons["menu_room_" + strCoord].SetHighlight(false);
		}
	}

	text["menu_coord"] = sf::Text("0,0", assetManager->GetFontRef("joystix"), 10);
	text["menu_coord"].setFillColor(sf::Color::Green);
	text["menu_coord"].setPosition(sf::Vector2f(414, 5));

	buttons["show_menu"] = GuiButton(sf::Vector2f(450, 270), sf::Vector2f(20, 20), sf::Vector2f(0, 0), "<", assetManager->GetFontRef("joystix"), 0);
	buttons["show_menu"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["show_menu"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["show_menu"].SetHighlight(false);
	buttons["show_menu"].SetCharacterSize(15);
}

void StateEditor::InitDoors()
{
	const sf::Vector2f posN = sf::Vector2f(208, 10);
	doors[Directions::N] = EditorDoor();
	doors[Directions::N].state = DoorStates::Closed;
	doors[Directions::N].sprite = sf::Sprite(game->assetManager.GetTextureRef("sprites"));
	doors[Directions::N].sprite.setPosition(posN);
	doors[Directions::N].sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	doors[Directions::N].shape = sf::RectangleShape(sf::Vector2f(DOOR_WIDTH, DOOR_HEIGHT));
	doors[Directions::N].shape.setOutlineColor(sf::Color::Red);
	doors[Directions::N].shape.setOutlineThickness(1.f);
	doors[Directions::N].shape.setFillColor(sf::Color::Transparent);
	doors[Directions::N].shape.setPosition(posN);
	doors[Directions::N].hitbox = sf::FloatRect(posN, doors[Directions::N].shape.getSize());

	const sf::Vector2f posS = sf::Vector2f(208, 280);
	doors[Directions::S] = doors[Directions::N];
	doors[Directions::S].sprite.setRotation(180.f);
	doors[Directions::S].sprite.setOrigin(DOOR_WIDTH, DOOR_HEIGHT);
	doors[Directions::S].sprite.setPosition(posS);
	doors[Directions::S].shape.setPosition(posS);
	doors[Directions::S].hitbox.left = posS.x;
	doors[Directions::S].hitbox.top = posS.y;

	const sf::Vector2f posW = sf::Vector2f(10, 128);
	doors[Directions::W] = doors[Directions::N];
	doors[Directions::W].sprite.setRotation(270.f);
	doors[Directions::W].sprite.setPosition(posW);
	doors[Directions::W].sprite.setOrigin(DOOR_WIDTH, 0);
	doors[Directions::W].shape.setPosition(posW);
	doors[Directions::W].shape.setSize(sf::Vector2f(DOOR_HEIGHT, DOOR_WIDTH)); // Put it on its side
	doors[Directions::W].hitbox.left = posW.x;
	doors[Directions::W].hitbox.top = posW.y;
	doors[Directions::W].hitbox.width = DOOR_HEIGHT;
	doors[Directions::W].hitbox.height = DOOR_WIDTH;

	const sf::Vector2f posE = sf::Vector2f(431, 128);
	doors[Directions::E] = doors[Directions::W];
	doors[Directions::E].sprite.setRotation(90.f);
	doors[Directions::E].sprite.setPosition(posE);
	doors[Directions::E].sprite.setOrigin(0, DOOR_HEIGHT);
	doors[Directions::E].shape.setPosition(posE);
	doors[Directions::E].hitbox.left = posE.x;
	doors[Directions::E].hitbox.top = posE.y;
}

void StateEditor::Load()
{

}

void StateEditor::Save()
{

}

void StateEditor::ChangeRoom(const sf::Vector2i newRoom)
{
	currentRoom = newRoom;
	text["menu_coord"].setString( std::to_string(newRoom.x) + "," + std::to_string(newRoom.y) );
}

void StateEditor::Start()
{

}

void StateEditor::Draw() const
{
	game->window.draw(spBackground);

	for (auto& door : doors)
	{
		game->window.draw(door.second.sprite);
		game->window.draw(door.second.shape);
	}

	if (showMenu)
	{
		game->window.draw(menuBg);
	}

	for (auto& button : buttons)
	{
		if ( (button.first.substr(0, 4) == "menu" && showMenu) || // Show menu items when menu is open
			(button.first.substr(0, 4) != "menu" && !showMenu) ) // Show other items when it's not
		{
			game->window.draw(button.second);
		}
	}

	for (auto& t : text)
	{
		if ((t.first.substr(0, 4) == "menu" && showMenu) || // Show menu items when menu is open
			(t.first.substr(0, 4) != "menu" && !showMenu)) // Show other items when it's not
		{
			game->window.draw(t.second);
		}
	}
}

void StateEditor::Update(const float dt)
{
	
}

void StateEditor::HandleInput()
{
	sf::Event event;
	sf::Vector2f m = game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window));

	while (game->window.pollEvent(event))
	{
		// Close the window
		if (event.type == sf::Event::Closed)
		{
			game->Close();
			return;
		}

		// Click on/select button
		for (auto button : buttons)
		{
			if (event.type == sf::Event::MouseButtonPressed && button.second.hitbox.contains(m))
			{
				// Perform action based on which button this is
				if (button.first == "menu_close" && showMenu)
				{
					showMenu = false;
				}
				if (button.first == "show_menu" && !showMenu)
				{
					showMenu = true;
				}
			}
		}

		// Click on doors
		for (auto& door : doors)
		{
			if (event.type == sf::Event::MouseButtonPressed && door.second.hitbox.contains(m))
			{
				// Rotate through states when clicked
				switch (door.second.state)
				{
				case DoorStates::Closed:
					door.second.state = DoorStates::Open;
					door.second.sprite.setTextureRect(animManager.Animate("door_open"));
					break;
				case DoorStates::Open:
					door.second.state = DoorStates::Locked;
					door.second.sprite.setTextureRect(animManager.Animate("door_locked"));
					break;
				case DoorStates::Locked:
					door.second.state = DoorStates::None;
					door.second.sprite.setTextureRect(animManager.Animate("door_none"));
					break;
				case DoorStates::None:
					door.second.state = DoorStates::Closed;
					door.second.sprite.setTextureRect(animManager.Animate("door_closed"));
					break;
				}
			}
		}

		if (event.type == sf::Event::MouseMoved)
		{
			// Check GUI highlights
			for (auto& button : buttons)
			{
				if (button.second.hitbox.contains(sf::Vector2f((float)m.x, (float)m.y)))
					button.second.SetHighlight(true);
				else
					button.second.SetHighlight(false);
			}
		}
	} // End while pollEvent
} // End HandleInput()