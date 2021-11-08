#include "Object.h"

Object::Object(int x, int y) {
	sf::Transformable::setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
}

float Object::GetLeftBorder()const {
	return getPosition().x;
}

float Object::GetTopBorder()const {
	return getPosition().y;
}

float Object::GetRightBorder()const {
	return getPosition().x + getSize().x;
}

float Object::GetLowerBorder()const {
	return getPosition().y + getSize().y;
}

sf::Vector2f Object::GetSpeed()const {
	return speedProjection;
}
void Object::ReverseSpeedX() {
	speedProjection.x *= -1;
}

void Object::ReverseSpeedY() {
	speedProjection.y *= -1;
}

//Object::Object(int x, int y, sf::Vector2i& speedProjection) {
//	sf::Transformable::setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
//	this->speedProjection = speedProjection;
//	/*position.x = x;
//	position.y = y;*/
//}