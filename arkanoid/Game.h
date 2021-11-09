#pragma once
#include "Map.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bottom.h"
#include "Bonus.h"
#include "LiveBar.h"

class Game {
	static const int WND_HEIGHT = 875;
	static const int WND_WIDTH = 1052;
	static const int VERTICAL_BORDER_WIDTH = 20;
	static const int HORIZONTAL_BORDER_HEIGHT = 30;

	static const int LEFT_INDENT = 63;
	static const int TOP_INDENT = 75;

	static const int TOP_TEXT_INDENT = 30;
	static const int LEFT_LIVE_BAR_INDENT = 810;

	static const int PADDLE_TOP_INDENT = 800;

	static const int BONUS_HALF_WIDTH = 18;

	static const int FRAMES_PER_SECOND = 800;
	static const int FRAMERATE = 200;

	static const int SCORE_INCREMENT = 10;
	static const int SCORE_DECREMENT = 20;


	sf::Texture baseBackground;
	sf::Sprite backgroundSprite;

	int score = 0;
	const std::string scoreText = "Score: ";
	sf::Text scoreLine;
	sf::Font font;

	sf::Clock* clock;

	LiveBar* liveBar;

	Map* map;
	Paddle* paddle;
	Ball* ball;
	Ball* secondBall;
	Bottom* bottom;
	Bonus* bonus;

	void SetBackground();

	void KeyPressEvent();

	void ProcessMapCollision(Ball* _ball);
	void ManageBallCollisions(Ball* _ball);

	void GenerateBonus(BonusType type, Object* object, Block* block);
	float ManageBonusGeneration(Block* block, HelthNum curHelthNum);
	void ManageBonusAction(float time);
	void ManageBallActivity();
	void ManageBallBallCollision();

	void IncreaseScore(sf::Vector2i& positionOnMap);
	void ReduceScore();
	void CheckWin();
public:
	sf::RenderWindow window{ sf::VideoMode(WND_WIDTH, WND_HEIGHT), "Arkanoid", sf::Style::Close };

	Game();
	~Game();

	void Run();

	void DrawInWindow();
};