#include "Engine/Board.h"
#include <iostream>
#include <string>
#include "Definitions/Config/Config.h"
#include "Definitions/Logger/Logger.h"

using namespace std;

int main(int /*argc*/, char **argv)
{
	Logger::InitLogger(Config::GetAsChar("LOG_PATH"));
	

	Board game = Board(Config::GetAsInteger("BOARD_WIDTH"), Config::GetAsInteger("BOARD_HEIGHT"), Config::GetAsInteger("BOARD_PLAYERS"), Config::GetAsInteger("BOARD_TURNS"), Config::GetAsInteger("BOARD_SCALE_WIDTH"), Config::GetAsInteger("BOARD_SCALE_HEIGHT"));
	game.GameLoop();

	Logger::EndLog();
    cout << "End of Code!\n";
	cin.get();
	return 0;
}