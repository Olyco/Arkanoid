#include "Game.h"
#include <iostream>

int main(void){
	srand(static_cast<unsigned int>(time(0)));
	Game game;

	while (game.window.isOpen()) {
		game.Run();
		game.DrawInWindow();
	}
	
	return 0;
}