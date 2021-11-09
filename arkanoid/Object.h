#pragma once
#include <SFML/Graphics.hpp>

class Object: public sf::RectangleShape {
protected:
	sf::Vector2f speedProjection{ 0.f,0.f };
	sf::Texture texture;
public:
	Object(int x, int y);

	float GetLeftBorder()const;
	float GetTopBorder()const;
	float GetRightBorder()const;
	float GetLowerBorder()const;

	void ReverseSpeedX();
	void ReverseSpeedY();

	sf::Vector2f GetSpeed()const;
};
