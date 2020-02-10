#include "gui_button.h"

//unsigned int GuiButton::number = 0;

GuiButton::GuiButton(const int order)
{
	this->order = order;
}

GuiButton::GuiButton( const sf::Vector2f pos, const sf::Vector2f size, const sf::Vector2f padding, const std::string string, const sf::Font &font, const int order )
{
	this->padding = padding;
	SetPos( pos );
	SetSize( size );
	text.setString( string );
	text.setFont( font );
	this->order = order;
}

void GuiButton::SetHighlight( const bool highlight )
{
	if( colors.empty() || highlightColors.empty() )
		return;

	std::map<std::string, sf::Color> *map;
	// Choose map based on highlight status
	if( !highlight )
		map = &colors;
	else
		map = &highlightColors;

	// Set colors accordingly
	shape.setFillColor( map->at( "background" ) );
	shape.setOutlineColor( map->at( "outline" ) );
	shape.setOutlineThickness( 2.f );
#ifdef OLD_SFML
	text.setColor( map->at( "text" ) );
#else
	text.setFillColor( map->at( "text" ) );
#endif
}

void GuiButton::SetColors( const sf::Color background, sf::Color text, sf::Color outline )
{
	colors["background"] = background;
	colors["text"] = text;
	colors["outline"] = outline;
}

void GuiButton::SetHighlightColors( const sf::Color background, sf::Color text, sf::Color outline )
{
	highlightColors["background"] = background;
	highlightColors["text"] = text;
	highlightColors["outline"] = outline;
}

void GuiButton::SetPos( const sf::Vector2f pos )
{
	text.setPosition( pos + padding );
	shape.setPosition( pos );
	hitbox.left = pos.x;
	hitbox.top = pos.y;
}

void GuiButton::SetSize( const sf::Vector2f size )
{
	shape.setSize( size );
	hitbox.width = size.x;
	hitbox.height = size.y;
}

void GuiButton::SetText( const std::string text )
{
	this->text.setString( text );
}

void GuiButton::SetCharacterSize(const unsigned int size)
{
	this->text.setCharacterSize(size);
}

void GuiButton::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( shape );
	target.draw( text );
}