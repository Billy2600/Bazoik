#include "state_editor.h"

StateEditor::StateEditor(Game* game)
{
	this->game = game;


	spBackground.setTexture(game->assetManager.GetTextureRef("background"));
	spBackground.setPosition(0, 0);

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

	buttons["show_menu"] = GuiButton(sf::Vector2f(450, 270), sf::Vector2f(20, 20), sf::Vector2f(0, 0), "<", assetManager->GetFontRef("joystix"), 0);
	buttons["show_menu"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["show_menu"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["show_menu"].SetHighlight(false);
	buttons["show_menu"].SetCharacterSize(15);
}

void StateEditor::InitDoors()
{
	const sf::Vector2f posN = sf::Vector2f(208, 10);
	doors['n'] = EditorDoor();
	doors['n'].state = DoorStates::Closed;
	doors['n'].sprite = sf::Sprite(game->assetManager.GetTextureRef("sprites"));
	doors['n'].sprite.setPosition(posN);
	doors['n'].sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	doors['n'].shape = sf::RectangleShape(sf::Vector2f(DOOR_WIDTH, DOOR_HEIGHT));
	doors['n'].shape.setOutlineColor(sf::Color::Red);
	doors['n'].shape.setOutlineThickness(1.f);
	doors['n'].shape.setFillColor(sf::Color::Transparent);
	doors['n'].shape.setPosition(posN);
	doors['n'].hitbox = sf::FloatRect(posN, doors['n'].shape.getSize());

	const sf::Vector2f posS = sf::Vector2f(208, 280);
	doors['s'] = doors['n'];
	doors['s'].sprite.setRotation(180.f);
	doors['s'].sprite.setOrigin(DOOR_WIDTH, DOOR_HEIGHT);
	doors['s'].sprite.setPosition(posS);
	doors['s'].shape.setPosition(posS);
	doors['s'].hitbox.left = posS.x;
	doors['s'].hitbox.top = posS.y;

	const sf::Vector2f posW = sf::Vector2f(10, 128);
	doors['w'] = doors['n'];
	doors['w'].sprite.setRotation(270.f);
	doors['w'].sprite.setPosition(posW);
	doors['w'].sprite.setOrigin(DOOR_WIDTH, 0);
	doors['w'].shape.setPosition(posW);
	doors['w'].shape.setSize(sf::Vector2f(DOOR_HEIGHT, DOOR_WIDTH)); // Put it on its side
	doors['w'].hitbox.left = posW.x;
	doors['w'].hitbox.top = posW.y;
	doors['w'].hitbox.width = DOOR_HEIGHT;
	doors['w'].hitbox.height = DOOR_WIDTH;

	const sf::Vector2f posE = sf::Vector2f(431, 128);
	doors['e'] = doors['w'];
	doors['e'].sprite.setRotation(90.f);
	doors['e'].sprite.setPosition(posE);
	doors['e'].sprite.setOrigin(0, DOOR_HEIGHT);
	doors['e'].shape.setPosition(posE);
	doors['e'].hitbox.left = posE.x;
	doors['e'].hitbox.top = posE.y;
}

void StateEditor::Load()
{

}

void StateEditor::Save()
{

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