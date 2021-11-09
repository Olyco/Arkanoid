#include "Ball.h"

void Ball::SetDefaultSpeed() {
	speedProjection.x = DEFAULT_PROJ_MODUL;
	if (rand() % 2 == 0)
		speedProjection.x *= -1.f;
	speedProjection.y = -DEFAULT_PROJ_MODUL;
}

Ball::Ball(int windowWidth, int bottom) :Object((windowWidth - BALL_SIZE) / 2, bottom-BALL_SIZE) {
	setSize(sf::Vector2f(BALL_SIZE, BALL_SIZE));
	texture.loadFromFile("ball.png");
	setTexture(&texture);
	setTextureRect(sf::IntRect(0, 0, BALL_SIZE, BALL_SIZE));
	SetDefaultSpeed();

	isOnPaddle = true;
	isActive = false;
}

void Ball::FollowPaddle(const Paddle* paddle) {
	setPosition(sf::Vector2f(paddle->getPosition().x + (paddle->getSize().x - getSize().x) / 2, paddle->getPosition().y - getSize().y));
}

void Ball::UpdatePosition(const Paddle* paddle, float time) {
	if (isOnPaddle)
		FollowPaddle(paddle);
	else
		move(speedProjection * time);
}

void Ball::UpdateSpeed(sf::Vector2f& newSpeedProjection) {
	if (abs(newSpeedProjection.x) <= MAX_PROJ_MODUL && abs(newSpeedProjection.y) <= MAX_PROJ_MODUL && abs(newSpeedProjection.x) >= MIN_PROJ_MODUL && abs(newSpeedProjection.y) >= MIN_PROJ_MODUL)
		speedProjection = newSpeedProjection;
}

void Ball::ProcessBordersCollision(int borderWidth, int borderHeight, int windowWidth, Bottom* bottom) {
	if (GetLeftBorder() <= borderWidth) {
		setPosition(sf::Vector2f(static_cast<float>(borderWidth), getPosition().y ));
		ReverseSpeedX();
	}
	else if (GetTopBorder() <= borderHeight) {
		setPosition(sf::Vector2f(getPosition().x, static_cast<float>(borderHeight)));
		ReverseSpeedY();
	}
	else if (GetRightBorder() >= windowWidth - borderWidth) {
		setPosition(sf::Vector2f(static_cast<float>(windowWidth - borderWidth - getSize().x), getPosition().y));
		ReverseSpeedX();
	}
	else if (bottom->isActive && GetLowerBorder() >= bottom->getPosition().y) {
		bottom->isActive = false;
		setPosition(sf::Vector2f(getPosition().x, static_cast<float>(bottom->getPosition().y - getSize().y)) );
		ReverseSpeedY();
	}
}

void Ball::ProcessPaddleCollision(Paddle* paddle) {
	sf::FloatRect intersection;

	if (!getGlobalBounds().intersects(paddle->getGlobalBounds(), intersection) || isOnPaddle)
		return;

	if (paddle->isSticky)
		isOnPaddle = true;

	if (paddle->getPosition().y < getPosition().y) {
		ReverseSpeedX();
		if (getPosition().x < paddle->getPosition().x)
			setPosition(paddle->getPosition().x - getSize().x, getPosition().y);
		else
			setPosition(paddle->getPosition().x + paddle->getSize().x, getPosition().y);
		return;
	}

	//ball collides with horizontal surface
	ReverseSpeedY();
	setPosition(getPosition().x, paddle->getPosition().y - getSize().y);
	/*if (getPosition().x < paddle->GetLeftBorder() + paddle->getSize().x / 4.f || getPosition().x > paddle->GetRightBorder() - paddle->getSize().x / 4.f)
		ReverseSpeedX();*/
}

void Ball::ProcessBlockCollision(Block* block) {
	float ballLeft = GetLeftBorder();
	float ballTop = GetTopBorder();
	float ballRight = GetRightBorder();
	float ballLower = GetLowerBorder();

	float blockLeft = block->GetLeftBorder();
	float blockTop = block->GetTopBorder();
	float blockRight = block->GetRightBorder();
	float blockLower = block->GetLowerBorder();

	sf::FloatRect intersection;

	if (!getGlobalBounds().intersects(block->getGlobalBounds(), intersection) || block->isDeleted)
		return;

	if (intersection.height >= intersection.width) { //ball collides with the side surface 
		if (ballLeft < blockLeft) { //from the left side
			setPosition(blockLeft - BALL_SIZE, ballTop);
		}
		else {
			setPosition(blockRight, ballTop);
		}
		ReverseSpeedX();
	}
	else { //ball collides with the horizontal surface
		if (ballTop < blockTop) { //from the top
			setPosition(ballLeft, blockTop - BALL_SIZE);
		}
		else {
			setPosition(ballLeft, blockLower);
		}
		ReverseSpeedY();
	}
}

void Ball::AccelerateBall(float accelerationCoefficient) {
	if (abs(speedProjection.x * accelerationCoefficient) >= MAX_PROJ_MODUL || abs(speedProjection.y * accelerationCoefficient) >= MAX_PROJ_MODUL)
		return;
	speedProjection *= accelerationCoefficient;
}

bool Ball::BallIsActive(int windowHeight) {
	if (GetTopBorder() > windowHeight)
		return false;

	return true;
}