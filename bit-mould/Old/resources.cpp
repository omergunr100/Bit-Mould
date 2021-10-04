#include<math.h>
#include<iostream>
#include<time.h>
#include<random>
#include "BMP.h"
#include <sstream>
#include "Randomness.h"

using namespace std;

namespace resources {

	struct ant_variant {
	public: // struct wide constants and counter
		static const int color_size = 3;
		static int counter;
		static int empty_cells;
		
	public: // value fields
		int last_turn_check;
		int id;
		Color color;
		float spread_chance;
		float aggressiveness;
		float strength;
		int strong_direction;
		int null;
	public:

		// Default empty constructor
		ant_variant()
			:id(-2),
			color{ 0,0,0 },
			spread_chance(0.0f),
			aggressiveness(0.0f),
			strength(0.0f),
			strong_direction(4)
		{
			this->null = 1;
		}
		// Fully defined constructor
		ant_variant(const Color& color, float spread_chance = 0.0f, float aggressiveness = 0.0f, float strength = 0.0f, int id = -1, int strong_direction = 4)
			:spread_chance(spread_chance - 0.5f),
			aggressiveness(aggressiveness),
			strength(strength),
			strong_direction(strong_direction),
			color(color),
			last_turn_check(0)
		{
			if (id != -1) {
				this->id = id;
			}
			else {
				this->id = counter++;
			}
			this->null = 1;
		}
		// Copy constructor
		ant_variant(const ant_variant& other)
			:id(other.id),
			spread_chance(other.spread_chance),
			aggressiveness(other.aggressiveness),
			strength(other.strength),
			strong_direction(other.strong_direction),
			color(other.color),
			last_turn_check(other.last_turn_check)
		{
			this->null = 1;
		}

		~ant_variant() {
			/*
			cout << "ant_variant destructor" << endl;
			cout << "\n";
			*/
		}

		static ant_variant get_blank() {
			return ant_variant();
		}

		static ant_variant get_random() {
			//cout << "Generating random ant" << endl;
			Color color;
			for (int i = 0; i < color_size; i++) {
				color.r = Randomness::dist_1_0();
				color.g = Randomness::dist_1_0();
				color.b = Randomness::dist_1_0();
			}
			float spread_chance = Randomness::dist_1_0();
			float aggressiveness = Randomness::dist_1_0();
			float strength = Randomness::dist_1_0();
			int strong_direction = (int)(Randomness::dist_1_0() * 8);
			/*
			cout << "Generating ant:" << endl;
			cout << "Color: [" << color.r << ", " << color.g << ", " << color.b << "]" << endl;
			cout << "Spread: " << spread_chance << endl;
			cout << "Aggressiveness: " << aggressiveness << endl;
			cout << "Strength: " << strength << endl;
			cout << "Strong direction: " << strong_direction << endl;
			cout << "\n";
			*/
			return ant_variant(color, spread_chance, aggressiveness, strength, -1, strong_direction);
		}

		static ant_variant get_null() {
			ant_variant temp = ant_variant();
			temp.null = NULL;
			return temp;
		}

		static int get_id() {
			return ant_variant::counter++;
		}
		int get_random_direction() {
			return (int)(Randomness::dist_1_0() * 8);
		}

		bool spread(const int& direction) {
			if (Randomness::dist_1_0() <= spread_chance + static_cast<float>(direction == strong_direction)*0.5f) {
				ant_variant::empty_cells--;
				//cout << "Spread\n";
				return true;
			}
			return false;
		}

		bool roll_aggression(const int& direction) {
			if (Randomness::dist_1_0() <= aggressiveness + static_cast<float>(direction == strong_direction)*0.5f) {
				//cout << "Aggressive\n";
				return true;
			}
			return false;
		}

		bool attack(const ant_variant& neighbor, const int& direction) {
			if (strength + static_cast<float>(strong_direction == direction)*0.5f > neighbor.strength + static_cast<float>(neighbor.strong_direction == (8 - direction))*0.5f) {
				//cout << "Eat\n";
				return true;
			}
			return false;
		}

		ant_variant play_turn(const ant_variant& neighbor, const int& direction, const int& turn_number) {
			bool color_ops = false;
			if (last_turn_check <= turn_number) {
				color_ops = true;
				last_turn_check = turn_number;
			}

			if (neighbor.null == NULL) {
				return neighbor;
			}
			else if (this->id == neighbor.id) {
				aggressiveness -= 0.01f;
				// average out the colors
				if (color_ops) {
					color.r = (neighbor.color.r + color.r*4) / 5;
					color.g = (neighbor.color.g + color.g*4) / 5;
					color.b = (neighbor.color.b + color.b*4) / 5;
				}
				return neighbor;
			}
			else if (neighbor.id == -2) {
				if (this->spread(direction)) {
					spread_chance -= 0.05f;
					return ant_variant(*this);
					/*
					ant_variant spreaded = ant_variant::get_random(rand);
					spreaded.id = id;
					spreaded.color = color;
					return spreaded;
					*/
				}
				spread_chance += 0.05f;
			}
			else if (this->roll_aggression(direction)) {
				if (this->attack(neighbor, direction)) {
					// Eat
					//strength -= 0.03f;
					ant_variant mixed = ant_variant(*this);
					mixed.color.r += (min(color.r, neighbor.color.r) - max(color.r, neighbor.color.r)) / 10;
					mixed.color.g += (min(color.g, neighbor.color.g) - max(color.g, neighbor.color.g)) / 10;
					mixed.color.b += (min(color.b, neighbor.color.b) - max(color.b, neighbor.color.b)) / 10;
					return mixed;
					// Mix
					/*
					ant_variant mixed = ant_variant::get_random(rand);
					mixed.color = color;
					if (color.r != neighbor.color.r)
						mixed.color.r += (min(color.r,neighbor.color.r)-max(color.r, neighbor.color.r)) / 16;
					if (color.g != neighbor.color.g)
						mixed.color.g += (min(color.g, neighbor.color.g) - max(color.g, neighbor.color.g)) / 16;
					if (color.b != neighbor.color.b)
						mixed.color.b += (min(color.b, neighbor.color.b) - max(color.b, neighbor.color.b)) / 16;
					return mixed;
					*/
				}
				strength += 0.01f;
			}
			aggressiveness += 0.01f;
			return neighbor;
		}

	private:

		ant_variant(int null) {
			this->null = NULL;
		}
	};

	class nest {
	public:
		int row;
		int col;
		ant_variant ant;
	public:

		nest(int row, int col)
			:row(row),
			col(col)
		{
			this->ant = ant_variant::get_random();
		}

		nest(int row, int col, const ant_variant& ant)
			:row(row),
			col(col),
			ant(ant)
		{
		}


		~nest() {
			/*
			cout << "Nest destructor" << endl;
			cout << "\n";
			*/
		}

		static nest get_random(int row_limit, int col_limit) {
			int row = (int)(Randomness::dist_1_0()*(float)row_limit);
			int col = (int)(Randomness::dist_1_0()*(float)col_limit);
			cout << "Generating random nest" << endl;
			cout << "Column: " << col << endl;
			cout << "Row: " << row << endl;
			cout << "\n";
			return nest(row, col);
		}
		static nest get_random(int row_limit, int col_limit, const ant_variant& ant) {
			int row = (int)(Randomness::dist_1_0()*(float)row_limit);
			int col = (int)(Randomness::dist_1_0()*(float)col_limit);
			cout << "Generating random nest" << endl;
			cout << "Column: " << col << endl;
			cout << "Row: " << row << endl;
			cout << "\n";
			return nest(row, col, ant);
		}
	};


	class board {
	private:
		int rows;
		int columns;
		int end_turn;
		int turn_number;
		int num_players;
		int scale_x;
		int scale_y;
		vector<vector<ant_variant>> game_board;
		vector<BMP> images;
	public:
		/*
		// Fully defined constructor, number of players
		board(randomness& rand, int end_turn = -1, int num_players = 1, int columns = 10, int rows = 10)
			:end_turn(end_turn),
			turn_number(0),
			rows(rows),
			columns(columns),
			num_players(num_players),
			rand(rand)
		{

			//cout << "Rows: " << this->rows << ", Columns: " << this->columns << endl;
			//this->game_board.reserve(this->rows);
			this->game_board.resize(this->rows);
			for (int r = 0; r < this->rows; r++) {
				//this->game_board[r].reserve(this->columns);
				this->game_board[r].resize(this->columns);
				for (int c = 0; c < this->columns; c++) {
					//cout << "Initializing: [" << c << ", " << r << "]\n";
					this->game_board[r][c] = ant_variant::get_blank();
					ant_variant::empty_cells++;
					//cout << "Blank nest generated at: [" << c << ", " << r << "]\n";
				}
			}

			for (int i = 0; i < this->num_players; i++) {
				nest player = nest::get_random(this->rows, this->columns, this->rand);
				cout << "Creating player at: [" << player.row << ", " << player.col << "]\n";
				if (game_board[player.row][player.col].id == -2)
					ant_variant::empty_cells--;
				this->game_board[player.row][player.col] = player.ant;
			}

			this->take_pixel_map();

			cout << "Initiated board\n";
			cout << "Number of turns: " << end_turn << endl;
			cout << "Number of players: " << num_players << endl;
			cout << "Number of rows: " << rows << endl;
			cout << "Number of columns: " << columns << endl;
			cout << "\n";
		}
		*/
		
		board(int end_turn = -1, int num_players = 1, int columns = 10, int rows = 10, int scale_x = 1, int scale_y = 1)
			:end_turn(end_turn),
			turn_number(0),
			rows(rows),
			columns(columns),
			num_players(num_players),
			scale_x(scale_x),
			scale_y(scale_y)
		{

				//cout << "Rows: " << this->rows << ", Columns: " << this->columns << endl;
				//this->game_board.reserve(this->rows);
				this->game_board.resize(this->rows);
				for (int r = 0; r < this->rows; r++) {
					//this->game_board[r].reserve(this->columns);
					this->game_board[r].resize(this->columns);
					for (int c = 0; c < this->columns; c++) {
						//cout << "Initializing: [" << c << ", " << r << "]\n";
						this->game_board[r][c] = ant_variant::get_blank();
						ant_variant::empty_cells++;
						//cout << "Blank nest generated at: [" << c << ", " << r << "]\n";
					}
				}

				for (int i = 0; i < this->num_players; i++) {
					nest player = nest::get_random(this->rows, this->columns);
					cout << "Creating player at: [" << player.row << ", " << player.col << "]\n";
					if (game_board[player.row][player.col].id == -2)
						ant_variant::empty_cells--;
					this->game_board[player.row][player.col] = player.ant;
				}
				

			this->take_pixel_map();

			cout << "Initiated board\n";
			cout << "Number of turns: " << end_turn << endl;
			cout << "Number of players: " << num_players << endl;
			cout << "Number of rows: " << rows << endl;
			cout << "Number of columns: " << columns << endl;
			cout << "\n";
		}
		

		~board() {
			cout << "Board destructor called\n";
		}

		vector<BMP> get_images() {
			return this->images;
		}

		void play_game() {
			cout << "Play game called, Total turns to be played: " << this->end_turn << endl;
			if (this->end_turn<=0) {
				cout << "End turn <= 0" << endl << endl;
				while (ant_variant::empty_cells != 0)
				{
					this->turn_number++;
					cout << "Playing Turn: " << this->turn_number << ", Number of types: " << ant_variant::counter << endl;
					this->play_turn();
					cout << "There are " << ant_variant::empty_cells << " - " << round((float)ant_variant::empty_cells / (float)(rows*columns) * 100) << "%" << " empty cells left" << "\n\n";
				}
				this->end_turn = this->turn_number - this->end_turn;
				while (this->end_turn - this->turn_number > 0) 
				{
					this->turn_number++;
					cout << "Playing Turn: " << this->turn_number << ", Number of types: " << ant_variant::counter << endl;
					cout << "There are " << this->end_turn - this->turn_number << " turns left" << endl;
					this->play_turn();
					cout << "There are " << ant_variant::empty_cells << " - " << round((float)ant_variant::empty_cells / (float)(rows*columns) * 100) << "%" << " empty cells left" << "\n\n";
				}
			}
			else {
				cout << "End turn > 0" << endl << endl;
				while (this->end_turn-this->turn_number > 0)
				{
					this->turn_number++;
					cout << "Playing Turn: " << this->turn_number << ", Number of types: " << ant_variant::counter << endl;
					cout << "There are " << this->end_turn - this->turn_number << " turns left" << endl;
					this->play_turn();
					cout << "There are " << ant_variant::empty_cells << " - " << round((float)ant_variant::empty_cells / (float)(rows*columns)*100) << "%" << " empty cells left" << "\n\n";
				}
			}
		}

	private:
		void take_pixel_map() {
			cout << "Started taking picture - Turn: " << this->turn_number << endl;
			BMP image = BMP(columns, rows);
			for (int y = 0; y < rows; y++) {
				for (int x = 0; x < columns; x++) {
					image.SetColor(game_board[y][x].color,x,y);
				}
			}
			stringstream ss;
			ss.str(string());
			ss << "images/out_" << turn_number << ".bmp";
			image.Export(ss.str().c_str(),scale_x,scale_y);
			//image.Export(ss.str().c_str());
			//images.push_back(image);
			cout << "Image taken: Turn: " << this->turn_number << ", Pixels: " << rows*columns << endl;
		}

		void play_turn() {
			vector<vector<ant_variant>> changes = this->game_board;
			
			for (int row = 0; row < this->rows; row++) {
				for (int col = 0; col < this->columns; col++) {
					if (this->game_board[row][col].null == NULL || this->game_board[row][col].id == -2)
						continue;
					for (int r = -1; r < 2; r++) {
						if ((row == 0 && r == -1) || (row == this->rows - 1 && r == 1)) {
							continue;
						}
						for (int c = -1; c < 2; c++) {
							if ((col == 0 && c == -1) || (col == this->columns - 1 && c == 1)) {
								continue;
							}
							else {
								int direction = 4;
								if (r == -1) {
									if (c == -1)
										direction = 0;
									if (c == 0)
										direction = 1;
									if (c == 1)
										direction = 2;
								}
								else if (r == 0) {
									if (c == -1)
										direction = 3;
									if (c == 0)
										direction = 4;
									if (c == 1)
										direction = 5;
								}
								else if (r == 1) {
									if (c == -1)
										direction = 6;
									if (c == 0)
										direction = 7;
									if (c == 1)
										direction = 8;
								}
								//cout << "Playing: [" << row + r << ", " << col + c << "]\n";
								if(game_board[row+r][col+c].id != changes[row+r][col+c].id)
									changes[row + r][col + c] = game_board[row][col].play_turn(changes[row + r][col + c], direction, turn_number);
								else
									changes[row + r][col + c] = game_board[row][col].play_turn(game_board[row + r][col + c], direction, turn_number);
							}
						}
					}
				}
			}
			this->game_board = changes;
			this->take_pixel_map();
		}
	};

};