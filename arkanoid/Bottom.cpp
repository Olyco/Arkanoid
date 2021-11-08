#include "Bottom.h"

Bottom::Bottom(int borderWidth, int windowHeight) : Object(borderWidth, windowHeight - BOTTOM_HEIGHT) {
	setSize(sf::Vector2f(BOTTOM_WIDTH, BOTTOM_HEIGHT));
	texture.loadFromFile("bottom.png");
	setTexture(&texture);
	setTextureRect(sf::IntRect(0, 0, BOTTOM_WIDTH, BOTTOM_HEIGHT));
}
