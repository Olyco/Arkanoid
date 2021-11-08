#include "Bonus.h"

Bonus::Bonus(int curX, int curY): Object(curX, curY) {
	setSize(sf::Vector2f(BONUS_WIDTH, BONUS_HEIGHT));
	texture.loadFromFile("bonuses.png");
	setTexture(&texture);

	speedProjection.y = SPEED_MODUL;
	targetObject = nullptr;
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

ResizePaddle::ResizePaddle(int curX, int curY, Object* object) : TemporaryBonus(curX, curY) {
	int typeChance = rand() % 2;
	if (typeChance == 0)
		curCoefficient = RESIZE_COEFFICIENT;
	else
		curCoefficient = -RESIZE_COEFFICIENT;
	setTextureRect(sf::IntRect(typeChance * BONUS_WIDTH, 0, BONUS_WIDTH, BONUS_HEIGHT)); //enlarge if 0, reduce if 1
	targetObject = object;
}

void ResizePaddle::BonusAction() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);

	int oldWidth = paddle->getSize().x;
	int newWidth = paddle->getSize().x + curCoefficient;

	paddle->setSize(sf::Vector2f(newWidth, paddle->getSize().y));
	if (curCoefficient > 0) //enlarge
		paddle->setTextureRect(sf::IntRect(oldWidth, 0, newWidth, paddle->getSize().y));
	else //reduce
		paddle->setTextureRect(sf::IntRect(oldWidth * 2 + RESIZE_COEFFICIENT, 0, newWidth, paddle->getSize().y));//??????????????????????
}

void ResizePaddle::DeactivateBonus() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);

	int newWidth = paddle->getSize().x - curCoefficient;
	paddle->setSize(sf::Vector2f(newWidth, paddle->getSize().y));
	paddle->setTextureRect(sf::IntRect(0, 0, newWidth, paddle->getSize().y));
}

ChangeBallSpeed::ChangeBallSpeed(int curX, int curY, Object* object) : Bonus(curX, curY) {
	int typeChance = rand() % 2;
	if (typeChance == 0)
		curCoefficient = SPEED_CHANGE_COEFFICIENT;
	else
		curCoefficient = -SPEED_CHANGE_COEFFICIENT;

	setTextureRect(sf::IntRect(BONUS_WIDTH * 2 + typeChance * BONUS_WIDTH, 0, BONUS_WIDTH, BONUS_HEIGHT)); //speed up if 0, slow down if 1
	targetObject = object;
}

void ChangeBallSpeed::BonusAction() {
	Ball* ball = static_cast<Ball*>(targetObject);

	sf::Vector2f oldSpeed = ball->GetSpeed();
	float speedMultiplier = 1.f + curCoefficient;

	sf::Vector2f newSpeed = oldSpeed * speedMultiplier;

	ball->UpdateSpeed(newSpeed);
}

BottomHelper::BottomHelper(int curX, int curY, Object* object) : TemporaryBonus(curX, curY) {
	ACTION_TIME = 12000.f;

	setTextureRect(sf::IntRect(BONUS_WIDTH * 4, 0, BONUS_WIDTH, BONUS_HEIGHT));
	targetObject = object;
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

StickyPaddle::StickyPaddle(int curX, int curY, Object* object) :TemporaryBonus(curX, curY) {
	setTextureRect(sf::IntRect(BONUS_WIDTH * 5, 0, BONUS_WIDTH, BONUS_HEIGHT));
	targetObject = object;
}

void StickyPaddle::BonusAction() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);
	paddle->isSticky = true;
}

void StickyPaddle::DeactivateBonus() {
	Paddle* paddle = static_cast<Paddle*>(targetObject);
	paddle->isSticky = false;
}