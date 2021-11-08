#include "Game.h"
#include <iostream>

Game::Game() {
	SetBackground();
	map = new Map(LEFT_INDENT, TOP_INDENT);
	paddle = new Paddle(WND_WIDTH, PADDLE_TOP_INDENT);
	ball = new Ball(WND_WIDTH, static_cast<int>(paddle->getPosition().y));
	bottom = new Bottom(VERTICAL_BORDER_WIDTH, WND_HEIGHT);
	bonus = nullptr;

	font.loadFromFile("dolcevita.ttf");
	scoreLine.setFont(font);
	scoreLine.setCharacterSize(35);
	scoreLine.setFillColor(sf::Color::White);
	scoreLine.setString(scoreText + std::to_string(score));
	scoreLine.setPosition(sf::Vector2f(LEFT_INDENT, TOP_TEXT_INDENT));

	clock = new sf::Clock();
	window.setFramerateLimit(FRAMERATE);
}

Game::~Game() {
	delete map;
	delete paddle;
	delete ball;
	delete bottom;

	if (bonus)
		delete bonus;
}

void Game::SetBackground() {
	baseBackground.loadFromFile("base background.png");
	backgroundSprite.setTexture(baseBackground);
}

void Game::KeyPressEvent() {
	sf::Event event;

	window.pollEvent(event);
	if (event.type == sf::Event::Closed)
		window.close();

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		paddle->direction = Direction::LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		paddle->direction = Direction::RIGHT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (ball->isOnPaddle) {
			ball->SetDefaultSpeed();
			ball->isOnPaddle = false;
		}
	}
}

float Game::ManageBonusGeneration(sf::Vector2i& blockPositionOnMap) {
	BonusType type = static_cast<BonusType>(rand() % static_cast<int>(BonusType::NUM_OF_TYPES));
	Object* object = nullptr;

	switch (type) {
	case BonusType::RESIZE_PADDLE:
		object = paddle;
		break;
	case BonusType::CHANGE_BALL_SPEED:
		object = ball;
		break;
	case BonusType::BOTTOM_HELPER:
		object = bottom;
		break;
	case BonusType::STICKY_PADDLE:
		object = paddle;
		break;
	}
	
	float accelerationCoef = map->blocksMap[blockPositionOnMap.y][blockPositionOnMap.x]->CollisionProcessing(&bonus, type, object);
	return accelerationCoef;
}

void Game::ProcessMapCollision() {
	if (!map->GetRectangle().intersects(ball->getGlobalBounds()))
		return;

	for (int i = 0; i < map->GetMapSize().y; ++i) {
		for (int j = 0; j < map->GetMapSize().x; ++j) {
			if (map->blocksMap[i][j]->isDeleted || !ball->getGlobalBounds().intersects(map->blocksMap[i][j]->getGlobalBounds()))
				continue;

			ball->ProcessBlockCollision(map->blocksMap[i][j]);

			float accelerationCoef;
			//выбирать тип бонуса здесь и передавать его в обработчик коллизии вместе с объектом
			if (map->blocksMap[i][j]->hasBonus && bonus == nullptr) {
				sf::Vector2i curPosition(j, i);
				accelerationCoef = ManageBonusGeneration(curPosition);
			}
			else
				accelerationCoef = map->blocksMap[i][j]->CollisionProcessing(&bonus, BonusType::VOID, map->blocksMap[i][j]);

			if (accelerationCoef != 0.f) //need to accelerate the ball with such a coefficient
				ball->AccelerateBall(accelerationCoef);

			if (map->blocksMap[i][j]->isDeleted)
				++map->numOfDeleted;

		}
	}
}

void Game::ManageBallCollisions() {
	ball->ProcessBordersCollision(VERTICAL_BORDER_WIDTH, HORIZONTAL_BORDER_HEIGHT, WND_WIDTH, bottom);
	ball->ProcessPaddleCollision(paddle);
	ProcessMapCollision();
}

void Game::DrawInWindow() {
	window.draw(backgroundSprite);
	map->DisplayMap(window);
	window.draw(*paddle);
	window.draw(*ball);

	if (bottom->isActive)
		window.draw(*bottom);

	if (bonus != nullptr && bonus->needToDraw)
		window.draw(*bonus);

	scoreLine.setString(scoreText + std::to_string(score));
	window.draw(scoreLine);
	window.display();
}

void Game::Run() {
	float time = static_cast<float>(clock->getElapsedTime().asMicroseconds());
	clock->restart(); 
	time = time / FRAMES_PER_SECOND;

	KeyPressEvent();

	paddle->UpdatePosition(VERTICAL_BORDER_WIDTH, WND_WIDTH, time);
	ball->UpdatePosition(paddle, time);

	ManageBallCollisions();
	if (bonus != nullptr && bonus->needToDraw) {
		if (bonus->UpdatePosition(paddle, WND_HEIGHT, time)) {
			bonus->BonusAction();
		}
	}
	if (bonus != nullptr) {
		if(bonus->hasActionTime /*&& bonus->isActivated*/) {
		TemporaryBonus* tempBonus = static_cast<TemporaryBonus*>(bonus);
		tempBonus->ManageActiveTime(time);

		if (!bonus->isActivated) {
			tempBonus->DeactivateBonus();
			delete bonus;
			bonus = nullptr;
			std::cout << "bonus end" << std::endl;
		}
	}
		else{ //has already worked
			delete bonus;
			bonus = nullptr;
		}
	}
}