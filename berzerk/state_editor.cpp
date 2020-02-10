#include "state_editor.h"

StateEditor::StateEditor(Game* game)
{
	this->game = game;

	if (!txBackground.loadFromFile("assets/background.png"))
		log.Write("Could not load title screen image");
	else
	{
		spBackground.setTexture(txBackground);
		spBackground.setPosition(0, 0);
	}

	InitMenu();
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

	buttons["show_menu"] = GuiButton(sf::Vector2f(450, 270), sf::Vector2f(25, 40), sf::Vector2f(0, 0), "<", assetManager->GetFontRef("joystix"), 0);
	buttons["show_menu"].SetColors(sf::Color::Black, sf::Color::Green, sf::Color::Green);
	buttons["show_menu"].SetHighlightColors(sf::Color::Black, sf::Color::Red, sf::Color::Red);
	buttons["show_menu"].SetHighlight(false);
	buttons["menu_close"].SetCharacterSize(15);
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
	if (showMenu)
	{
		game->window.draw(menuBg);
	}

	for (auto button : buttons)
	{
		if(button.first.substr(0,4) == "menu" && showMenu)
			game->window.draw(button.second);
		else if (button.first.substr(0, 4) != "menu" && !showMenu)
			game->window.draw(button.second);
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