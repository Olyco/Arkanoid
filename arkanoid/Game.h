#pragma once
#include "Object.h"
#include "Map.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bottom.h"
#include "Bonus.h"

class Game {
	static const int WND_HEIGHT = 875;
	static const int WND_WIDTH = 1052;
	static const int VERTICAL_BORDER_WIDTH = 20;
	static const int HORIZONTAL_BORDER_HEIGHT = 30;

	static const int LEFT_INDENT = 63; //54
	static const int TOP_INDENT = 75;
	//static const int RIGHT_INDENT = 54;

	static const int TOP_TEXT_INDENT = 30;

	static const int PADDLE_TOP_INDENT = 800;

	static const int FRAMES_PER_SECOND = 800;
	static const int FRAMERATE = 200;

	sf::Texture baseBackground;
	sf::Sprite backgroundSprite;

	unsigned long long score = 0;
	const std::string scoreText = "Score: ";
	sf::Text scoreLine;
	sf::Font font;

	sf::Clock* clock;

	Map* map;
	Paddle* paddle;
	Ball* ball;
	Bottom* bottom;
	Bonus* bonus;

	void SetBackground();
	void ProcessMapCollision();

public:
	Game();
	~Game();

	sf::RenderWindow window{ sf::VideoMode(WND_WIDTH, WND_HEIGHT), "Arkanoid", sf::Style::Close };

	void Run();
	void KeyPressEvent();

	void ManageBallCollisions();
	float ManageBonusGeneration(sf::Vector2i& blockPositionOnMap);
	void DrawInWindow();
};