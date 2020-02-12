#include "state_editor.h"

StateEditor::StateEditor(Game* game)
{
	this->game = game;


	spBackground.setTexture(game->assetManager.GetTextureRef("background"));
	spBackground.setPosition(0, 0);
	currentRoom = sf::Vector2i(0, 0);
	currentlyDragging = NULL;

	Load();
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

	text["saved"] = sf::Text("Saved to file", assetManager->GetFontRef("joystix"), 15);
	text["saved"].setFillColor(sf::Color::Transparent);
	text["saved"].setPosition(sf::Vector2f(5, 5));

	buttons["show_menu"] = GuiButton(sf::Vector2f(450, 270), sf::Vector2f(20, 20), sf::Vector2f(0, 0), "<", assetManager->GetFontRef("joystix"), 0);
	buttons["show_menu"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["show_menu"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["show_menu"].SetHighlight(false);
	buttons["show_menu"].SetCharacterSize(15);
}

void StateEditor::InitDoors()
{
	const std::map<Directions, sf::Vector2f> pos = {
		std::make_pair(Directions::N, sf::Vector2f(208, 10)),
		std::make_pair(Directions::S, sf::Vector2f(208, 280)),
		std::make_pair(Directions::W, sf::Vector2f(10, 128)),
		std::make_pair(Directions::E, sf::Vector2f(431, 128))
	};

	doors[Directions::N] = EditorDoor();
	doors[Directions::N].sprite = sf::Sprite(game->assetManager.GetTextureRef("sprites"));
	doors[Directions::N].sprite.setPosition(pos.at(Directions::N)); // Have to use .at() because const
	doors[Directions::N].shape = sf::RectangleShape(sf::Vector2f(DOOR_WIDTH, DOOR_HEIGHT));
	doors[Directions::N].shape.setOutlineColor(sf::Color::Red);
	doors[Directions::N].shape.setOutlineThickness(1.f);
	doors[Directions::N].shape.setFillColor(sf::Color::Transparent);
	doors[Directions::N].shape.setPosition(pos.at(Directions::N));
	doors[Directions::N].hitbox = sf::FloatRect(pos.at(Directions::N), doors[Directions::N].shape.getSize());

	doors[Directions::S] = doors[Directions::N];
	doors[Directions::S].sprite.setRotation(180.f);
	doors[Directions::S].sprite.setOrigin(DOOR_WIDTH, DOOR_HEIGHT);
	doors[Directions::S].sprite.setPosition(pos.at(Directions::S));
	doors[Directions::S].shape.setPosition(pos.at(Directions::S));
	doors[Directions::S].hitbox.left = pos.at(Directions::S).x;
	doors[Directions::S].hitbox.top = pos.at(Directions::S).y;

	doors[Directions::W] = doors[Directions::N];
	doors[Directions::W].sprite.setRotation(270.f);
	doors[Directions::W].sprite.setPosition(pos.at(Directions::W));
	doors[Directions::W].sprite.setOrigin(DOOR_WIDTH, 0);
	doors[Directions::W].shape.setPosition(pos.at(Directions::W));
	doors[Directions::W].shape.setSize(sf::Vector2f(DOOR_HEIGHT, DOOR_WIDTH)); // Put it on its side
	doors[Directions::W].hitbox.left = pos.at(Directions::W).x;
	doors[Directions::W].hitbox.top = pos.at(Directions::W).y;
	doors[Directions::W].hitbox.width = DOOR_HEIGHT;
	doors[Directions::W].hitbox.height = DOOR_WIDTH;

	doors[Directions::E] = doors[Directions::W];
	doors[Directions::E].sprite.setRotation(90.f);
	doors[Directions::E].sprite.setPosition(pos.at(Directions::E));
	doors[Directions::E].sprite.setOrigin(0, DOOR_HEIGHT);
	doors[Directions::E].shape.setPosition(pos.at(Directions::E));
	doors[Directions::E].hitbox.left = pos.at(Directions::E).x;
	doors[Directions::E].hitbox.top = pos.at(Directions::E).y;

	// Init door states
	UpdateDoors();
}

void StateEditor::UpdateDoors()
{
	for (auto& door : doors)
	{
		auto& state = rooms[currentRoom.x][currentRoom.y].doorStates[door.first];
		door.second.state = state;
		door.second.sprite.setTextureRect(animManager.Animate("door_" + Room::GetDoorStateStringFromState(state)));
	}
}

void StateEditor::AddEntity(const std::string type, const sf::Vector2f pos)
{
	if (type == "keese")
	{
		auto newSprite = sf::Sprite(game->assetManager.GetTextureRef("sprites"), animManager.Animate("keese_still"));
		newSprite.setPosition(pos);
		rooms[currentRoom.x][currentRoom.y].entities.push_back(EditorEntities { type, newSprite });
	}
	else
	{
		log.Write("Invalid entity type specified: " + type);
	}
}

void StateEditor::Load()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("assets/rooms.xml");
	if (!result) // Error check
	{
		log.Write("Unable to load assets/rooms.xml");
		return;
	}

	try
	{
		pugi::xml_node roomNodes = doc.child("rooms");
		for (pugi::xml_node room : roomNodes.children("room"))
		{
			// Don't do anything if room position is invalid
			if (room.attribute("x") != NULL && room.attribute("y") != NULL)
			{
				auto x = room.attribute("x").as_int();
				auto y = room.attribute("t").as_int();

				if (room.attribute("door_n") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::N, Room::GetDoorStateFromString( room.attribute("door_n").value() ) ) );
				if (room.attribute("door_s") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::S, Room::GetDoorStateFromString( room.attribute("door_s").value() ) ) );
				if (room.attribute("door_e") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::E, Room::GetDoorStateFromString (room.attribute("door_e").value() ) ) );
				if (room.attribute("door_w") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::W, Room::GetDoorStateFromString( room.attribute("door_w").value() ) ) );

				for (pugi::xml_node entity : room.children("entity"))
				{
					sf::Vector2f pos = sf::Vector2f(0, 0);
					if (entity.attribute("x") != NULL && entity.attribute("y") != NULL)
					{
						pos.x = std::stof(entity.attribute("x").value());
						pos.y = std::stof(entity.attribute("y").value());
					}
					if (entity.attribute("type") != NULL)
					{
						AddEntity(entity.attribute("type").value(), pos);
					}
				}
			} // End if
		} // End for
	}
	catch (int e)
	{
		log.Write("Error while loading assets/rooms.xml");
	}
}

void StateEditor::Save()
{
	pugi::xml_document doc;

	try
	{
		pugi::xml_node roomNode = doc.append_child("rooms");

		for (int x = 0; x < MAX_ROOM_X; x++)
		{
			for (int y = 0; y < MAX_ROOM_Y; y++)
			{
				pugi::xml_node room = roomNode.append_child("room");

				room.append_attribute("x").set_value(x);
				room.append_attribute("y").set_value(y);

				room.append_attribute("door_n").set_value(Room::GetDoorStateStringFromState(rooms[x][y].doorStates[Directions::N]).c_str());
				room.append_attribute("door_s").set_value(Room::GetDoorStateStringFromState(rooms[x][y].doorStates[Directions::S]).c_str());
				room.append_attribute("door_e").set_value(Room::GetDoorStateStringFromState(rooms[x][y].doorStates[Directions::E]).c_str());
				room.append_attribute("door_w").set_value(Room::GetDoorStateStringFromState(rooms[x][y].doorStates[Directions::W]).c_str());

				for (auto& entity : rooms[x][y].entities)
				{
					pugi::xml_node entityNode = room.append_child("entity");
					entityNode.append_attribute("type").set_value(entity.type.c_str());
					entityNode.append_attribute("x").set_value(entity.sprite.getPosition().x);
					entityNode.append_attribute("y").set_value(entity.sprite.getPosition().y);
				}
			}
		}

		if (!doc.save_file("assets/rooms.xml"))
		{
			log.Write("Unable to save assets/rooms.xml");
			return;
		}
	}
	catch (int e)
	{
		log.Write("Error while saving assets/rooms.xml");
	}

	// Show message on screen
	text["saved"].setFillColor(sf::Color::Green);
	lastSave.restart();
}

void StateEditor::ChangeRoom(const sf::Vector2i newRoom)
{
	currentRoom = newRoom;
	text["menu_coord"].setString( std::to_string(newRoom.x) + "," + std::to_string(newRoom.y) );
	UpdateDoors();
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
		if ((t.first.substr(0, 4) == "menu" && showMenu) || // Same conditions as above
			(t.first.substr(0, 4) != "menu" && !showMenu) ||
			(t.first == "saved") )  // Exceptions
		{
			game->window.draw(t.second);
		}
	}

	for (auto& entity : rooms[currentRoom.x][currentRoom.y].entities)
	{
		game->window.draw(entity.sprite);
	}
}

void StateEditor::Update(const float dt)
{
	if (lastSave.getElapsedTime().asSeconds() > SHOW_MESSAGE_TIME)
	{
		text["saved"].setFillColor(sf::Color::Transparent);
	}
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
				if (button.first.substr(0, 10) == "menu_room_")
				{
					int x = std::stoi( button.first.substr(10, 1) );
					int y = std::stoi( button.first.substr(11, 1) );
					ChangeRoom(sf::Vector2i(x, y));
				}
				else if (button.first == "menu_close" && showMenu)
				{
					showMenu = false;
				}
				else if (button.first == "show_menu" && !showMenu)
				{
					showMenu = true;
				}
				else if (button.first == "menu_save")
				{
					Save();
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
					// Open the underlying room door state
					// Actual interactable door will be updated by UpdateDoors() below
					rooms[currentRoom.x][currentRoom.y].doorStates[door.first] = DoorStates::Open;
					break;
				case DoorStates::Open:
					rooms[currentRoom.x][currentRoom.y].doorStates[door.first] = DoorStates::Locked;
					break;
				case DoorStates::Locked:
					rooms[currentRoom.x][currentRoom.y].doorStates[door.first] = DoorStates::None;
					break;
				case DoorStates::None:
					rooms[currentRoom.x][currentRoom.y].doorStates[door.first] = DoorStates::Closed;
					break;
				}

				UpdateDoors();
			}
		}

		// Click on entities
		for (auto& entity : rooms[currentRoom.x][currentRoom.y].entities)
		{
			if (event.type == sf::Event::MouseButtonPressed && entity.sprite.getGlobalBounds().contains(m))
			{
				currentlyDragging = &entity.sprite;
			}
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			// Stop dragging item
			currentlyDragging = NULL;
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

			// Move object we're currently dragging, if applicable
			if (currentlyDragging != NULL)
			{
				currentlyDragging->setPosition( sf::Vector2f(round(m.x), round(m.y)) ); // Keep position to pixel level
			}
		}
	} // End while pollEvent
} // End HandleInput()