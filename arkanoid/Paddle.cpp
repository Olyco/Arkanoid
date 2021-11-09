#include "Paddle.h"

Paddle::Paddle(int windowWidth, int topIndent) : Object((windowWidth - PADDLE_WIDTH) / 2, topIndent) {
	setSize(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
	texture.loadFromFile("paddle.png");
	setTexture(&texture);
	setTextureRect(sf::IntRect(0, 0, PADDLE_WIDTH, PADDLE_HEIGHT));

	isSticky = false;
}

void Paddle::ProcessBordersCollision(int borderWidth, int windowWidth) {
	if (GetLeftBorder() < borderWidth) {
		setPosition(sf::Vector2f(static_cast<float>(borderWidth), getPosition().y));
		//speedProjection.x = 0.f;
	}
	else if (GetRightBorder() > windowWidth - borderWidth) {
		setPosition(sf::Vector2f(static_cast<float>(windowWidth - borderWidth - getSize().x), getPosition().y));
		//speedProjection.x = 0.f;
	}
}

void Paddle::UpdatePosition(int borderWidth, int windowWidth, float time) {
	if (direction == Direction::LEFT && GetLeftBorder() > borderWidth /*+ SPEED_MODUL/* && !ProcessBordersCollision(borderWidth, windowWidth)*/)
		speedProjection.x = -SPEED_MODUL;
	else if (direction == Direction::RIGHT && GetRightBorder() < windowWidth - borderWidth/* - SPEED_MODUL/* && !ProcessBordersCollision(borderWidth, windowWidth)*/)
		speedProjection.x = SPEED_MODUL;
	else
		speedProjection.x = 0;

	ProcessBordersCollision(borderWidth, windowWidth);
	move(speedProjection * time);

	direction = Direction::PAUSE;
}