#include "Bonus.h"

Bonus::Bonus(int curX, int curY, Object* object): Object(curX, curY) {
	setSize(sf::Vector2f(BONUS_WIDTH, BONUS_HEIGHT));
	texture.loadFromFile("bonuses.png");
	setTexture(&texture);

	speedProjection.y = SPEED_MODUL;
	targetObject = object;
}

bool Bonus::UpdatePosition(const Paddle* paddle, int windowHeight, float time) {
	move(speedProjection * time);

	if (GetTopBorder() > windowHeight)
		needToDraw = false;

	if (getGlobalBounds().intersects(paddle->getGlobalBounds())) {
		needToDraw = false;
		isActivated = true;
		return true;
	}

	return false;
}

void TemporaryBonus::ManageActiveTime(float time) {
	if (!isActivated)
		return;

	curActiveTime += time;
	if (curActiveTime >= ACTION_TIME)
		isActivated = false;
}

ResizePaddle::ResizePaddle(int curX, int curY, Object* object) : TemporaryBonus(curX, curY, object) {
	int typeChance = rand() % 2;
	if (typeChance == 0) {
		curCoefficient = RESIZE_COEFFICIENT;
		setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::ENLARGE_PADDLE), 0, BONUS_WIDTH, BONUS_HEIGHT));
	}
	else {
		curCoefficient = -RESIZE_COEFFICIENT;
		setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::REDUCE_PADDLE), 0, BONUS_WIDTH, BONUS_HEIGHT));
	}
}

void ResizePaddle::BonusAction() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);

	int oldWidth = paddle->getSize().x;
	int newWidth = paddle->getSize().x + curCoefficient;

	paddle->setSize(sf::Vector2f(newWidth, paddle->getSize().y));
	if (curCoefficient > 0) //enlarge
		paddle->setTextureRect(sf::IntRect(oldWidth, 0, newWidth, paddle->getSize().y));
	else //reduce
		paddle->setTextureRect(sf::IntRect(oldWidth * 2 + RESIZE_COEFFICIENT, 0, newWidth, paddle->getSize().y));
}

void ResizePaddle::DeactivateBonus() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);

	int newWidth = paddle->getSize().x - curCoefficient;
	paddle->setSize(sf::Vector2f(newWidth, paddle->getSize().y));
	paddle->setTextureRect(sf::IntRect(0, 0, newWidth, paddle->getSize().y));
}

ChangeBallSpeed::ChangeBallSpeed(int curX, int curY, Object* object) : Bonus(curX, curY, object) {
	int typeChance = rand() % 2;
	if (typeChance == 0) {
		curCoefficient = SPEED_CHANGE_COEFFICIENT;
		setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::SPEED_UP_BALL), 0, BONUS_WIDTH, BONUS_HEIGHT));
	}
	else {
		curCoefficient = -SPEED_CHANGE_COEFFICIENT;
		setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::SLOW_DOWN_BALL), 0, BONUS_WIDTH, BONUS_HEIGHT));
	}
}

void ChangeBallSpeed::BonusAction() {
	Ball* ball = static_cast<Ball*>(targetObject);

	sf::Vector2f oldSpeed = ball->GetSpeed();
	float speedMultiplier = 1.f + curCoefficient;

	sf::Vector2f newSpeed = oldSpeed * speedMultiplier;

	ball->UpdateSpeed(newSpeed);
}

BottomHelper::BottomHelper(int curX, int curY, Object* object) : TemporaryBonus(curX, curY, object) {
	ACTION_TIME = 9000.f;

	setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::BOTTOM_HELPER), 0, BONUS_WIDTH, BONUS_HEIGHT));
}

void BottomHelper::BonusAction() {
	Bottom* bottom = static_cast<Bottom*>(targetObject);
	bottom->isActive = true;
}

void BottomHelper::DeactivateBonus() {
	Bottom* bottom = static_cast<Bottom*>(targetObject);
	if (bottom->isActive)
		bottom->isActive = false;
}

StickyPaddle::StickyPaddle(int curX, int curY, Object* object) :TemporaryBonus(curX, curY, object) {
	setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::STICKY_PADDLE), 0, BONUS_WIDTH, BONUS_HEIGHT));
}

void StickyPaddle::BonusAction() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);
	paddle->isSticky = true;
}

void StickyPaddle::DeactivateBonus() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);
	paddle->isSticky = false;
}

ChangeBallTrajectory::ChangeBallTrajectory(int curX, int curY, Object* object) : Bonus(curX, curY, object) {
	setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::CHANGE_TRAJECTORY), 0, BONUS_WIDTH, BONUS_HEIGHT));
}

void ChangeBallTrajectory::BonusAction() {
	Ball* ball = static_cast<Ball*>(targetObject);

	ball->ReverseSpeedX();
}

SecondBall::SecondBall(int curX, int curY, Object* object) : Bonus(curX, curY, object) {
	setTextureRect(sf::IntRect(BONUS_WIDTH * static_cast<int>(TextureIndent::SECOND_BALL), 0, BONUS_WIDTH, BONUS_HEIGHT));
}

void SecondBall::BonusAction() {
	Ball* ball = static_cast<Ball*>(targetObject);

	ball->isActive = true;
	ball->isOnPaddle = true;
	ball->SetDefaultSpeed();
}