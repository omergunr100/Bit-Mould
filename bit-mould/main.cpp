#include "Board.h"
#include <iostream>

using namespace std;

int main(int /*argc*/, char **argv)
{
	
	int width = 480;
	int height = 360;
	int players = 100;
	int turns = 0;
	int scale_x = 1920;
	int scale_y = 1080;

	Board game = Board(width, height, players, turns,scale_x, scale_y);
	game.GameLoop();

    cout << "End of Code!\n";
	cin.get();
	return 0;
}