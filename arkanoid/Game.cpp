#include "Game.h"

Game::Game() {
	SetBackground();
	map = new Map(LEFT_INDENT, TOP_INDENT);
	paddle = new Paddle(WND_WIDTH, PADDLE_TOP_INDENT);
	ball = new Ball(WND_WIDTH, static_cast<int>(paddle->getPosition().y));
	ball->isActive = true;
	secondBall = new Ball(WND_WIDTH, static_cast<int>(paddle->getPosition().y));
	bottom = new Bottom(VERTICAL_BORDER_WIDTH, WND_HEIGHT);
	bonus = nullptr;
	liveBar = new LiveBar(LEFT_LIVE_BAR_INDENT, TOP_TEXT_INDENT + 5);

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
	delete clock;
	delete map;
	delete paddle;
	delete ball;
	delete secondBall;
	delete bottom;
	delete liveBar;

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
		if (ball->isOnPaddle)
			ball->isOnPaddle = false;
		if (secondBall->isActive && secondBall->isOnPaddle)
			secondBall->isOnPaddle = false;
	}
}

void Game::GenerateBonus(BonusType type, Object* object, Block* block) {
	switch (type) {
	case BonusType::RESIZE_PADDLE:
		bonus = new ResizePaddle(block->getPosition().x + block->getSize().x / 2 - BONUS_HALF_WIDTH, block->getPosition().y + block->getSize().y / 2, object);
		break;
	case BonusType::CHANGE_BALL_SPEED :
		bonus = new ChangeBallSpeed(block->getPosition().x + block->getSize().x / 2 - BONUS_HALF_WIDTH, block->getPosition().y + block->getSize().y / 2, object);
		break;
	case BonusType::BOTTOM_HELPER:
		bonus = new BottomHelper(block->getPosition().x + block->getSize().x / 2 - BONUS_HALF_WIDTH, block->getPosition().y + block->getSize().y / 2, object);
		break;
	case BonusType::STICKY_PADDLE:
		bonus = new StickyPaddle(block->getPosition().x + block->getSize().x / 2 - BONUS_HALF_WIDTH, block->getPosition().y + block->getSize().y / 2, object);
		break;
	case BonusType::CHANGE_BALL_TRAJECTORY:
		bonus = new ChangeBallTrajectory(block->getPosition().x + block->getSize().x / 2 - BONUS_HALF_WIDTH, block->getPosition().y + block->getSize().y / 2, object);
		break;
	case BonusType::SECOND_BALL:
		bonus = new SecondBall(block->getPosition().x + block->getSize().x / 2 - BONUS_HALF_WIDTH, block->getPosition().y + block->getSize().y / 2, object);
		break;
	}

	bonus->needToDraw = true;
}

float Game::ManageBonusGeneration(Block* block, HelthNum curHelthNum) {
	BonusType type = static_cast<BonusType>(rand() % static_cast<int>(BonusType::NUM_OF_TYPES));
	Object* object = nullptr;

	if (secondBall->isActive) {
		while (type == BonusType::SECOND_BALL)
			type = static_cast<BonusType>(rand() % static_cast<int>(BonusType::NUM_OF_TYPES));
	}

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
	case BonusType::CHANGE_BALL_TRAJECTORY:
		object = ball;
		break;
	case BonusType::SECOND_BALL:
		object = secondBall;
		break;
	}
	
	float accelerationCoef = block->CollisionProcessing(type, object);
	if (curHelthNum == HelthNum::ONE) {
		block->bonusType = type;
		GenerateBonus(type, object, block);
	}
	return accelerationCoef;
}

void Game::ProcessMapCollision(Ball* _ball) {
	if (!map->GetRectangle().intersects(_ball->getGlobalBounds()))
		return;

	for (int i = 0; i < map->GetMapSize().y; ++i) {
		for (int j = 0; j < map->GetMapSize().x; ++j) {
			if (map->blocksMap[i][j]->isDeleted || !_ball->getGlobalBounds().intersects(map->blocksMap[i][j]->getGlobalBounds()))
				continue;

			_ball->ProcessBlockCollision(map->blocksMap[i][j]);

			float accelerationCoef;
			if (map->blocksMap[i][j]->hasBonus && bonus == nullptr) {
				sf::Vector2i curPositionOnMap(j, i);
				accelerationCoef = ManageBonusGeneration(map->blocksMap[i][j], map->CheckHelthNum(curPositionOnMap));
			}
			else
				accelerationCoef = map->blocksMap[i][j]->CollisionProcessing(BonusType::VOID, map->blocksMap[i][j]);

			if (accelerationCoef != 0.f) //need to accelerate the ball with such a coefficient
				_ball->AccelerateBall(accelerationCoef);

			if (map->blocksMap[i][j]->isDeleted) {
				++map->numOfDeleted;
				sf::Vector2i curPosition(j, i);
				IncreaseScore(curPosition);
			}
		}
	}
}

void Game::ManageBallCollisions(Ball* _ball) {
	_ball->ProcessBordersCollision(VERTICAL_BORDER_WIDTH, HORIZONTAL_BORDER_HEIGHT, WND_WIDTH, bottom);
	_ball->ProcessPaddleCollision(paddle);
	ProcessMapCollision(_ball);
}

void Game::ManageBallBallCollision() {
	if (!ball->getGlobalBounds().intersects(secondBall->getGlobalBounds()) || ball->isOnPaddle || secondBall->isOnPaddle)
		return;

	ball->ReverseSpeedX();
	secondBall->ReverseSpeedX();
}

void Game::ManageBonusAction(float time) {
	if (bonus != nullptr && bonus->needToDraw) {
		if (bonus->UpdatePosition(paddle, WND_HEIGHT, time)) {
			bonus->BonusAction();
		}
		else if (!bonus->needToDraw) {
			delete bonus;
			bonus = nullptr;
		}
	}
	if (bonus != nullptr) {
		if (bonus->hasActionTime) {
			if (bonus->isActivated) {
				TemporaryBonus* tempBonus = static_cast<TemporaryBonus*>(bonus);
				tempBonus->ManageActiveTime(time);

				if (!bonus->isActivated) {
					tempBonus->DeactivateBonus();
					delete bonus;
					bonus = nullptr;
				}
			}
		}
		else if(!bonus->needToDraw){ //has already worked
			delete bonus;
			bonus = nullptr;
		}
	}
}

void Game::ManageBallActivity() {
	if (!ball->BallIsActive(WND_HEIGHT)) {
		ReduceScore();
		if (secondBall->isActive) {
			std::swap(ball, secondBall);
			ball->isActive = true;
			secondBall->isActive = false;
			secondBall->FollowPaddle(paddle);
		}
		else {
			if (!liveBar->ProcessBallDrop()) {
				ball->isActive = false;
			}
			else {
				ball->isOnPaddle = true;
			}
		}
	}
	if (secondBall->isActive && !secondBall->BallIsActive(WND_HEIGHT)) {
		ReduceScore();
		secondBall->isActive = false;
		secondBall->FollowPaddle(paddle);
	}

}

void Game::DrawInWindow() {
	window.draw(backgroundSprite);
	map->DisplayMap(window);
	window.draw(*paddle);
	window.draw(*ball);
	if (secondBall->isActive)
		window.draw(*secondBall);

	if (bottom->isActive)
		window.draw(*bottom);

	if (bonus != nullptr && bonus->needToDraw)
		window.draw(*bonus);

	liveBar->DisplayLiveBar(window);

	scoreLine.setString(scoreText + std::to_string(score));
	window.draw(scoreLine);
	window.display();
}

void Game::Run() {
	float time = static_cast<float>(clock->getElapsedTime().asMicroseconds());
	clock->restart(); 
	time = time / FRAMES_PER_SECOND;

	if (!ball->isActive) {
		sf::sleep(sf::seconds(5.f));
		window.close();
		return;
	}

	KeyPressEvent();

	paddle->UpdatePosition(VERTICAL_BORDER_WIDTH, WND_WIDTH, time);
	ball->UpdatePosition(paddle, time);
	ManageBallCollisions(ball);

	if (secondBall->isActive) {
		secondBall->UpdatePosition(paddle, time);
		ManageBallCollisions(secondBall);
		ManageBallBallCollision();
	}

	ManageBonusAction(time);
	ManageBallActivity();
	CheckWin();
}

void Game::IncreaseScore(sf::Vector2i& positionOnMap) {
	score += SCORE_INCREMENT * static_cast<int>(map->CheckHelthNum(positionOnMap));
}

void Game::ReduceScore() {
	if (score - SCORE_DECREMENT > 0)
		score -= SCORE_DECREMENT;
	else
		score = 0;

}

void Game::CheckWin() {
	if (map->numOfDeleted == map->NUM_OF_BLOCKS) {
		sf::sleep(sf::seconds(5.f));
		window.close();
	}
}