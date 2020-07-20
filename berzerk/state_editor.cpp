#include "state_editor.h"

StateEditor::StateEditor(Game* game)
{
	this->game = game;

	spBackground.setTexture(game->assetManager.GetTextureRef("background"));
	spBackground.setPosition(0, 0);
	shTextRoom.setFillColor(sf::Color::Black);
	shTextRoom.setPosition(48, 48);
	shTextRoom.setSize(sf::Vector2f(384, 224));
	currentRoom = START_ROOM;
	currentlyDragging = NULL;
	deleteMode = false;

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
			if(x == currentRoom.x && y == currentRoom.y)
				buttons["menu_room_" + strCoord].SetColors(sf::Color::Black, sf::Color::Blue, sf::Color::Blue);
			else
				buttons["menu_room_" + strCoord].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
			buttons["menu_room_" + strCoord].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
			buttons["menu_room_" + strCoord].SetHighlight(false);
		}
	}

	buttons["show_menu"] = GuiButton(sf::Vector2f(450, 270), sf::Vector2f(20, 20), sf::Vector2f(0, 0), "<", assetManager->GetFontRef("joystix"), 0);
	buttons["show_menu"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["show_menu"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["show_menu"].SetHighlight(false);
	buttons["show_menu"].SetCharacterSize(15);

	text["menu_coord"] = sf::Text( std::to_string(START_ROOM.x) + "," + std::to_string(START_ROOM.y), assetManager->GetFontRef("joystix"), 10 );
	text["menu_coord"].setFillColor(sf::Color::Green);
	text["menu_coord"].setPosition(sf::Vector2f(414, 5));

	text["saved"] = sf::Text("Saved to file", assetManager->GetFontRef("joystix"), 15);
	text["saved"].setFillColor(sf::Color::Transparent);
	text["saved"].setPosition(sf::Vector2f(5, 5));

	// Using a vector/loop to make it easy to populate menu
	auto pos = sf::Vector2f(342, 80);
	const auto posIncrement = sf::Vector2f(20, 20);
	std::vector< std::tuple<std::string, std::string> > menuEntityItems = {
		{"keese", "keese_still"}, // Type, animation name (in XML)
		{"stalfos", "stalfos_still"},
		{"block", "block"},
		{"block_inverted", "block_inverted"},
		{"water", "water"},
		{"hole", "hole"},
		{"face_right_light", "face_right_light"},
		{"face_right_dark", "face_right_dark"},
		{"face_left_light", "face_left_light"},
		{"face_left_dark", "face_left_dark"},
		{"gravel_1", "gravel_1"},
		{"gravel_2", "gravel_2"},
		{"gravel_3", "gravel_3"},
		{"key", "key"},
		{"delete", "eraser"}
	};

	for (auto item : menuEntityItems)
	{
		auto type = std::get<0>(item);
		auto animName = std::get<1>(item);
		menuEntities[type] = sf::Sprite(game->assetManager.GetTextureRef("sprites"), animManager.Animate(animName));
		menuEntities[type].setScale(sf::Vector2f(0.5f, 0.5f));
		menuEntities[type].setPosition(pos);

		pos.x += posIncrement.x;
		// New line
		if (pos.x > GAME_WIDTH - posIncrement.x)
		{
			pos.x = 342;
			pos.y += posIncrement.y;
		}
	}

	deleteIcon = sf::Sprite(game->assetManager.GetTextureRef("sprites"), animManager.Animate("eraser"));
	deleteIcon.setOrigin(deleteIcon.getGlobalBounds().width / 2, deleteIcon.getGlobalBounds().height / 2);

	buttons["menu_text_room_toggle"] = GuiButton(sf::Vector2f(342, 150), sf::Vector2f(15, 15), sf::Vector2f(2, 0),
		rooms[currentRoom.x][currentRoom.y].textRoom ? "X" : " ",
		assetManager->GetFontRef("joystix"), 0);
	buttons["menu_text_room_toggle"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["menu_text_room_toggle"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["menu_text_room_toggle"].SetHighlight(false);
	buttons["menu_text_room_toggle"].SetCharacterSize(12);

	text["menu_text_room_toggle"] = sf::Text("Text Room", assetManager->GetFontRef("joystix"), 13);
	text["menu_text_room_toggle"].setFillColor(sf::Color::Green);
	text["menu_text_room_toggle"].setPosition(sf::Vector2f(365, 150));

	buttons["menu_text_room_string"] = GuiButton(sf::Vector2f(342, 175), sf::Vector2f(120, 15), sf::Vector2f(2, 0), "", assetManager->GetFontRef("joystix"), 0);
	buttons["menu_text_room_string"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["menu_text_room_string"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["menu_text_room_string"].SetHighlight(false);
	buttons["menu_text_room_string"].SetCharacterSize(12);

	text["room_text"] = sf::Text(rooms[currentRoom.x][currentRoom.y].roomText, assetManager->GetFontRef("joystix"), 15);
	text["room_text"].setFillColor(sf::Color::White);
	text["room_text"].setPosition(sf::Vector2f(80, 80));
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

void StateEditor::AddEntity(const std::string type, const sf::Vector2f pos, sf::Vector2i room)
{
	if (room == sf::Vector2i(-999, -999))
	{
		room = currentRoom;
	}

	std::string  animName = "";
	if (type == "keese") animName = "keese_still";
	else if (type == "stalfos") animName = "stalfos_still";
	else if (type == "block") animName = "block";
	else if (type == "block_inverted") animName = "block_inverted";
	else if (type == "water") animName = "water";
	else if (type == "hole") animName = "hole";
	else if (type == "face_right_light") animName = "face_right_light";
	else if (type == "face_right_dark") animName = "face_right_dark";
	else if (type == "face_left_light") animName = "face_left_light";
	else if (type == "face_left_dark") animName = "face_left_dark";
	else if (type == "gravel_1") animName = "gravel_1";
	else if (type == "gravel_2") animName = "gravel_2";
	else if (type == "gravel_3") animName = "gravel_3";
	else if (type == "key") animName = "key";
	// Special exception: Enable delete mode
	else if (type == "delete")
	{
		deleteMode = !deleteMode;
		return; // Don't add sprite to screen
	}
	else
	{
		log.Write("Invalid entity type specified: " + type);
	}

	auto newSprite = sf::Sprite(game->assetManager.GetTextureRef("sprites"), animManager.Animate(animName));
	newSprite.setPosition(pos);
	rooms[room.x][room.y].entities.push_back(EditorEntities{ type, newSprite });
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
			if (room.attribute("x") != NULL && room.attribute("y") != NULL
				&& room.attribute("x").as_int() < MAX_ROOM_X && room.attribute("y").as_int() < MAX_ROOM_Y)
			{
				auto x = room.attribute("x").as_int();
				auto y = room.attribute("y").as_int();

				if (room.attribute("door_n") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::N, Room::GetDoorStateFromString( room.attribute("door_n").as_string() ) ) );
				if (room.attribute("door_s") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::S, Room::GetDoorStateFromString( room.attribute("door_s").as_string() ) ) );
				if (room.attribute("door_e") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::E, Room::GetDoorStateFromString (room.attribute("door_e").as_string() ) ) );
				if (room.attribute("door_w") != NULL)
					rooms[x][y].doorStates.insert( std::make_pair( Directions::W, Room::GetDoorStateFromString( room.attribute("door_w").as_string() ) ) );

				rooms[x][y].textRoom = room.attribute("text_room").as_bool();

				rooms[x][y].roomText = room.attribute("room_text").as_string();

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
						AddEntity(entity.attribute("type").value(), pos, sf::Vector2i(x, y));
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

				room.append_attribute("text_room").set_value(rooms[x][y].textRoom);

				room.append_attribute("room_text").set_value(rooms[x][y].roomText.c_str());

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
	// Highlight current room/de-highlight old room
	buttons["menu_room_" + std::to_string(currentRoom.x) + std::to_string(currentRoom.y) ].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["menu_room_" + std::to_string(newRoom.x) + std::to_string(newRoom.y)].SetColors(sf::Color::Black, sf::Color::Blue, sf::Color::Blue);
	UpdateTextRoomToggle(rooms[newRoom.x][newRoom.y].textRoom);

	UpdateRoomTextString(rooms[newRoom.x][newRoom.y].roomText);

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

	if(rooms[currentRoom.x][currentRoom.y].textRoom)
		game->window.draw(shTextRoom);

	for (auto& door : doors)
	{
		game->window.draw(door.second.sprite);
		game->window.draw(door.second.shape);
	}

	for (auto& entity : rooms[currentRoom.x][currentRoom.y].entities)
	{
		game->window.draw(entity.sprite);
	}

	if (showMenu)
	{
		game->window.draw(menuBg);
		for (auto& menuEntity : menuEntities)
		{
			auto shape = sf::RectangleShape();
			shape.setPosition(menuEntity.second.getPosition());
			shape.setSize(sf::Vector2f(menuEntity.second.getGlobalBounds().width, menuEntity.second.getGlobalBounds().height));
			shape.setFillColor(sf::Color(0,255,0,100));

			game->window.draw(shape);
			game->window.draw(menuEntity.second);
		}
	}

	for (auto& button : buttons)
	{
		if ( (button.first.substr(0, 4) == "menu" && showMenu) || // Show menu items when menu is open
			(button.first.substr(0, 4) != "menu" && !showMenu) ) // Show other items when it's not
		{
			// Don't draw text room string field if this isn't a text room
			if(button.first != "menu_text_room_string" || rooms[currentRoom.x][currentRoom.y].textRoom)
				game->window.draw(button.second);
		}
	}

	for (auto& t : text)
	{
		if ((t.first.substr(0, 4) == "menu" && showMenu) || // Same conditions as above
			(t.first.substr(0, 4) != "menu" && !showMenu) ||
			(t.first == "saved") ||
			(t.first == "room_text") )  // Exceptions
		{
			game->window.draw(t.second);
		}
	}

	if (deleteMode)
	{
		game->window.draw(deleteIcon);
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

	if (deleteMode)
	{
		deleteIcon.setPosition(m);
	}

	while (game->window.pollEvent(event))
	{
		// Close the window
		if (event.type == sf::Event::Closed)
		{
			game->Close();
			return;
		}

		// Return to title screen
		if (event.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				Save();
				game->PopState();
				return;
			}
		}

		if (typingMode)
			TypeTextIntoField(event);

		// Click on/select button
		for (auto button : buttons)
		{
			if (event.type == sf::Event::MouseButtonPressed && button.second.hitbox.contains(m))
			{
				typingMode = false;

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
				else if (button.first == "menu_save" && showMenu)
				{
					Save();
				}
				else if (button.first == "menu_text_room_toggle" && showMenu)
				{
					// Toggle selection
					bool& textRoom = rooms[currentRoom.x][currentRoom.y].textRoom;
					textRoom = !textRoom;
					UpdateTextRoomToggle(textRoom);
				}
				else if (button.first == "menu_text_room_string" && showMenu)
				{
					typingMode = true;
				}
			}
		}

		if (showMenu && event.type == sf::Event::MouseButtonPressed)
		{
			for (auto& menuEntity : menuEntities)
			{
				if (menuEntity.second.getGlobalBounds().contains(m))
				{
					typingMode = false;
					AddEntity(menuEntity.first, ENTITY_SPAWN_POS);
				}
			}
		}

		// Click on doors
		for (auto& door : doors)
		{
			if ( (event.type == sf::Event::MouseButtonPressed && door.second.hitbox.contains(m))) 
			{
				typingMode = false;
				if (showMenu && door.first == Directions::E) // E door is under the menu
					continue;

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
			const sf::FloatRect spriteBounds = entity.sprite.getGlobalBounds();

			if (event.type == sf::Event::MouseButtonPressed && spriteBounds.contains(m))
			{
				typingMode = false;
				draggingOffset = sf::Vector2f( m.x - spriteBounds.left, m.y - spriteBounds.top );

				// Don't allow clicking anything 'under' the menu when it's showing
				if (showMenu && entity.sprite.getGlobalBounds().intersects(menuBg.getGlobalBounds()))
					continue;

				showMenu = false;
				if (deleteMode)
				{
					RemoveEntity(entity);
				}
				else
				{
					currentlyDragging = &entity.sprite;
				}
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
				// Using round to keep position at pixel (not subpixel) level
				currentlyDragging->setPosition( sf::Vector2f( round(m.x - draggingOffset.x), round(m.y - draggingOffset.y) ) );
			}
		}
	} // End while pollEvent
} // End HandleInput()

void StateEditor::RemoveEntity(const EditorEntities entity)
{
	rooms[currentRoom.x][currentRoom.y].entities.erase(
		std::remove(
			rooms[currentRoom.x][currentRoom.y].entities.begin(),
			rooms[currentRoom.x][currentRoom.y].entities.end(),
			entity
		),
		rooms[currentRoom.x][currentRoom.y].entities.end()
	);
}

void StateEditor::UpdateTextRoomToggle(const bool textRoom)
{
	if(textRoom)
	{
		buttons["menu_text_room_toggle"].SetText("X");
	}
	else
	{
		buttons["menu_text_room_toggle"].SetText(" ");
	}
}

void StateEditor::TypeTextIntoField(sf::Event &event)
{
	std::string& inputString = rooms[currentRoom.x][currentRoom.y].roomText;

	if (event.type == sf::Event::TextEntered)
	{
		// Printable chars; 0x08 is backspace (handled below)
		if (event.text.unicode < 0x80 && event.text.unicode != 0x08)
		{
			inputString += (char)event.text.unicode;
		}
	}

	if (event.type == sf::Event::KeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
		{
			typingMode = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace))
		{
			// Remove last char from string
			if(inputString.length() > 0)
				inputString.pop_back();
		}
	}

	UpdateRoomTextString(inputString);
}

void StateEditor::UpdateRoomTextString(std::string inputString)
{
	// Set button text only to what'll fit
	buttons["menu_text_room_string"].SetText(
		inputString.size() > 12
		? inputString.substr(inputString.size() - 12, 12)
		: inputString
	);

	text["room_text"].setString(inputString);
}