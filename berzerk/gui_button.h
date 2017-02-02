#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class GuiButton : public sf::Drawable
{
private:
	//static unsigned int number; // Number added
	sf::Text text; // Text displayed
	sf::Color hoverColor; // Color when hovered
	sf::RectangleShape shape; // Shape around text
	std::map<std::string, sf::Color> colors;
	std::map<std::string, sf::Color> highlightColors;
	sf::Vector2f padding;

public:
	sf::FloatRect hitbox; // Check if we were clicked

	GuiButton();
	GuiButton( const sf::Vector2f pos, const sf::Vector2f size, const sf::Vector2f padding, const std::string string, const sf::Font &font );
	void SetHighlight( const bool highlight ); // Tell this button it's highlighted or not
	void SetColors( const sf::Color background, sf::Color text, sf::Color outline );
	void SetHighlightColors( const sf::Color background, sf::Color text, sf::Color outline );
	void SetPos( const sf::Vector2f pos );
	void SetSize( const sf::Vector2f size );
	void SetText( const std::string text );
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;
};