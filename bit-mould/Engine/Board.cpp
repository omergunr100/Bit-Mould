#include "Board.h"
#include<iostream>
#include<sstream>

using namespace std;

Board::Board(int cols, int rows, int numPlayers, int endTurn, int scaleX, int scaleY)
	:m_cols(cols),
	m_rows(rows),
	m_numPlayers(numPlayers),
	m_endTurn(endTurn),
	m_scaleX(scaleX),
	m_scaleY(scaleY),
	m_turnNumber(0)
{
	m_gameBoard.resize(m_rows);
	for (int i = 0; i < m_rows; i++) {
		m_gameBoard[i].resize(m_cols);
		for (int j = 0; j < m_cols; j++) {
			m_gameBoard[i][j] = AntVariant();
		}
	}
	cout << "Generated empty " << m_gameBoard[0].size() << ", " << m_gameBoard.size() << " board" << endl;
	std::vector<Vector2> playerLoc = GeneratePlayerVector2();
	for (int i = 0; i < playerLoc.size(); i++) {
		cout << "Placing player number " << i << " at: [" << playerLoc[i].m_x << ", " << playerLoc[i].m_y << "]" << endl;
		m_gameBoard[playerLoc[i].m_y][playerLoc[i].m_x] = AntVariant::GetRandom();
		AntVariant::EmptyCells--;
	}
	cout << "Populated the board with " << m_numPlayers << " players" << endl;
	_mkdir(Config::GetAsChar("IMAGE_PATH"));
	TakeImage();
}

Board::Board(int cols, int rows, int numPlayers, int endTurn)
	:m_cols(cols),
	m_rows(rows),
	m_numPlayers(numPlayers),
	m_endTurn(endTurn),
	m_scaleX(cols),
	m_scaleY(rows),
	m_turnNumber(0)
{
	m_gameBoard.resize(m_rows);
	for (int i = 0; i < m_rows; i++) {
		m_gameBoard[i].resize(m_cols);
		for (int j = 0; j < m_cols; j++) {
			m_gameBoard[i][j] = AntVariant();
		}
	}
	cout << "Generated empty " << m_gameBoard[0].size() << ", " << m_gameBoard.size() << " board" << endl;
	std::vector<Vector2> playerLoc = GeneratePlayerVector2();
	for (int i = 0; i < playerLoc.size(); i++) {
		cout << "Placing player number " << i << " at: [" << playerLoc[i].m_x << ", " << playerLoc[i].m_y << "]" << endl;
		m_gameBoard[playerLoc[i].m_y][playerLoc[i].m_x] = AntVariant::GetRandom();
		AntVariant::EmptyCells--;
	}
	cout << "Populated the board with " << m_numPlayers << " players" << endl;
	_mkdir(Config::GetAsChar("IMAGE_PATH"));
	TakeImage();
}

Board::~Board()
{
}

vector<Vector2> Board::GeneratePlayerVector2()
{
	cout << "Started generating players (" << m_numPlayers << ")" << endl;
	vector<Vector2> resultVect(m_numPlayers);
	cout << "Generated vector of size: " << resultVect.size() << endl;
	for (int i = 0; i < m_numPlayers; i++) {
		int x = Randomness::dist_custom(0, m_cols-1), y = Randomness::dist_custom(0, m_rows-1);
		bool replicant = false;
		for (int j = 0; j < resultVect.size(); j++) {
			if (resultVect[j].m_x == x && resultVect[j].m_y == y){
				replicant = true;
				break;
			}
		}
		if(!replicant){
			cout << "Player " << i << ": [" << x << ", " << y << "]" << endl;
			resultVect[i] = Vector2(x, y);
		}
		else {
			i--;
			cout << "Duplicate, running again" << endl;
		}
	}
	return resultVect;
}

vector<BMP> Board::GetImages()
{
	return m_images;
}

vector<AntVariant> Board::GetImmediateNeighbors(const int & x, const int & y, vector<vector<AntVariant>>& changes)
{
	//cout << "Getting immediate neighbors" << endl;
	vector<AntVariant> neighbors;
	neighbors.reserve(9);
	for (int dy = -1; dy < 2; dy++) {
		for (int dx = -1; dx < 2; dx++) {
			if (x + dx < 0 || y + dy < 0 || x+dx >= m_cols || y+dy >= m_rows) {
				neighbors.push_back(AntVariant(AntVariant::SpecialId::PlaceHolder));
			}
			else {
				if (m_gameBoard[y + dy][x + dx].GetId() == changes[y + dy][x + dx].GetId()) {
					//cout << "Inserting neighbor from board: [" << x + dx << ", " << y + dy << "]" << endl;
					neighbors.push_back(m_gameBoard[y + dy][x + dx]);
				}
				else {
					//cout << "Inserting neighbor from changes: [" << x + dx << ", " << y + dy << "]" << endl;
					neighbors.push_back(changes[y + dy][x + dx]);
				}
			}
		}
	}
	return neighbors;
}

void Board::GameLoop()
{
	if (m_endTurn < 1) {
		cout << "Game-mode: Until the last empty cell is full +" << -m_endTurn << endl;
		while (!AntVariant::EmptyCells == 0)
		{
			m_turnNumber++;
			cout << "Playing turn: " << m_turnNumber << endl;
			PlayTurn();
		}
		m_endTurn = -m_endTurn + m_turnNumber;
		cout << "Last empty cell is full, game will end on the: " << m_endTurn << " turn" << endl;
		while (m_turnNumber <= m_endTurn)
		{
			m_turnNumber++;
			cout << "Playing turn: " << m_turnNumber << "; Turns remaining: " << m_endTurn - m_turnNumber << endl;
			PlayTurn();
		}
	}
	else {
		cout << "Game-mode: Until turn " << m_endTurn << endl;
		while (m_turnNumber <= m_endTurn)
		{
			m_turnNumber++;
			cout << "Playing turn: " << m_turnNumber << "; Turns remaining: " << m_endTurn - m_turnNumber << endl;
			PlayTurn();
		}
	}
}

void Board::PlayTurn()
{
	AntVariant::EmptyCells = 0;
	vector<vector<AntVariant>> changes = m_gameBoard;
	cout << "Empty cells remaining: " << AntVariant::EmptyCells << ", " << round(100 * AntVariant::EmptyCells / (m_rows*m_cols)) << "%" << endl;
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			if (m_gameBoard[y][x].GetId() < 0)
				continue;
			//cout << "Playing: [" << x << ", " << y << "]" << endl;
			vector<AntVariant*> results = m_gameBoard[y][x].PlayTurn(GetImmediateNeighbors(x,y,changes));
			int counter = 0;
			for (int dy = -1; dy < 2; dy++) {
				for (int dx = -1; dx < 2; dx++) {
					if (results[counter]->GetId() >= 0) {
						changes[y + dy][x + dx] = *results[counter];
						delete results[counter];
					}
					counter++;
				}
			}
		}
	}
	TakeImage();
	cout << endl;
	m_gameBoard = changes;
}

BMP Board::TakeImage()
{
	cout << "Started taking image, turn: " << m_turnNumber << endl;
	BMP image = BMP(m_cols, m_rows);
	
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			BMP::SetColor(image, m_gameBoard[y][x].GetColor(), x, y);
		}
	}
	stringstream ss;
	ss.str(string());
	ss << Config::GetAsChar("IMAGE_PATH") << "Image_" << m_turnNumber << ".bmp";
	std::string temp = ss.str();
	std::thread{ &BMP::ThreadedExport, image, temp, m_scaleX, m_scaleY }.detach();
	/*
	if (m_cols == m_scaleX && m_rows == m_scaleY)
		BMP::Export(image, ss.str().c_str());
	else
		BMP::Export(image, ss.str().c_str(), m_scaleX, m_scaleY);
	*/
	//m_images.push_back(image);
	cout << "Finished exporting image, resolution: " << image.GetWidth() << ", " << image.GetHeight() << endl;
	return image;
}

Vector2::Vector2()
	:m_x(0),
	m_y(0)
{
}

Vector2::Vector2(int x, int y)
	:m_x(x),
	m_y(y)
{
}
