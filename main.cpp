#include <array>
#include <chrono>
#include <format>
#include <iostream>
#include <thread>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif

int32_t COLS, ROWS;
void getConsoleSize(int32_t& cols, int32_t& rows) {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
	winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	cols = size.ws_col;
	rows = size.ws_row;
#endif
}

void move_cursor(int32_t x, int32_t y) {
	std::cout << "\033[" << y << ";" << x << "H";
	std::cout.flush();
}

int32_t clear_screen() {
#ifdef _WIN32
	return system("cls");
#else
	return system("clear");
#endif
}

std::ostream& print(const std::string& s, int32_t x, int32_t y) {
	if (x < 0) x = COLS - s.length() + x + 1;
	if (y < 0) y = ROWS + y + 1;
	move_cursor(x, y);
	return std::cout << s;
}

class Game {
	private:
		std::array<std::array<char, 3>, 3> field;
		char player_figure = 0;
		char enemy_figure = 0;
		bool player_turn = false;
		int32_t player_score = 0, enemy_score = 0;

	public:
		Game() {
			field.fill({' ', ' ', ' '});
			while (!(player_figure == 'c' or player_figure == 'z')) {
				clear_screen();
				print("Tic Tac Toe!", 1, 1);
				print("Cross or zero? (c / z): ", 1, -1);
				std::cin >> player_figure;
			}
			if (player_figure == 'c') {
				enemy_figure = 'z';
				player_turn = true;
			}
			else enemy_figure = 'c';
		}

		~Game() {}

		void draw_ui() {
			print("Tic Tac Toe!", 1, 1);
			print("You | Enemy", -1, 1) << std::endl;

			print(std::format("{:<3} | {:<5}", player_score, enemy_score), -1, 2);
		}

		void draw_field() {
			const std::string text_field = std::format("     |     |     |     |\n"
													   "     x  1  |  2  |  3  |\n"
													   "――y――|―――――|―――――|―――――|\n"
													   "  1  |  {}  |  {}  |  {}  |\n"
													   "―――――|―――――|―――――|―――――|\n"
													   "  2  |  {}  |  {}  |  {}  |\n"
													   "―――――|―――――|―――――|―――――|\n"
													   "  3  |  {}  |  {}  |  {}  |\n"
													   "―――――|―――――|―――――|―――――|",
													   field[0][0], field[0][1], field[0][2], field[1][0], field[1][1],
													   field[1][2], field[2][0], field[2][1], field[2][2]);

			print(text_field, 1, ROWS / 2);
		}

		void player_move() {
			while (true) {
				print("Your turn...", 1, 2);
				print("Choose position (x y): ", 1, -1);
				int32_t x, y;
				std::cin >> x >> y;
				if (x >= 1 and x <= 3 and y >= 1 and y <= 3) {
					if (field[y - 1][x - 1] != ' ') continue;
				}
				else continue;

				field[y - 1][x - 1] = player_figure;
				break;
			}
		}

		char check_winner() {
			for (int32_t i = 0; i < 3; ++i) {
				if (field[i][0] != ' ' && field[i][0] == field[i][1] && field[i][0] == field[i][2]) {
					return field[i][0];
				}
			}

			for (int32_t j = 0; j < 3; ++j) {
				if (field[0][j] != ' ' && field[0][j] == field[1][j] && field[0][j] == field[2][j]) {
					return field[0][j];
				}
			}

			if (field[0][0] != ' ' && field[0][0] == field[1][1] && field[0][0] == field[2][2]) {
				return field[0][0];
			}
			if (field[0][2] != ' ' && field[0][2] == field[1][1] && field[0][2] == field[2][0]) {
				return field[0][2];
			}

			return ' ';
		}

		bool is_moves_left() {
			for (const std::array<char, 3>& row : field) {
				for (const char& cell : row) {
					if (cell == ' ') return true;
				}
			}
			return false;
		}

		int32_t minimax(int32_t depth, bool isMaximizing) {
			char winner = check_winner();
			if (winner == enemy_figure) return 10 - depth;
			if (winner == player_figure) return depth - 10;
			if (!is_moves_left()) return 0;

			if (isMaximizing) {
				int32_t best_score = -1'000;

				for (int32_t i = 0; i < 3; ++i) {
					for (int32_t j = 0; j < 3; ++j) {
						if (field[i][j] == ' ') {
							field[i][j] = enemy_figure;
							int32_t score = minimax(depth + 1, false);
							field[i][j] = ' ';
							best_score = std::max(score, best_score);
						}
					}
				}
				return best_score;
			}
			else {
				int32_t best_score = 1'000;

				for (int32_t i = 0; i < 3; ++i) {
					for (int32_t j = 0; j < 3; ++j) {
						if (field[i][j] == ' ') {
							field[i][j] = player_figure;
							int32_t score = minimax(depth + 1, true);
							field[i][j] = ' ';
							best_score = std::min(score, best_score);
						}
					}
				}
				return best_score;
			}
		}

		std::pair<int32_t, int32_t> find_best_move() {
			int32_t best_score = -1'000;
			std::pair<int32_t, int32_t> best_move = {-1, -1};

			for (int32_t i = 0; i < 3; ++i) {
				for (int32_t j = 0; j < 3; ++j) {
					if (field[i][j] == ' ') {
						field[i][j] = enemy_figure;
						int32_t move_score = minimax(0, false);
						field[i][j] = ' ';

						if (move_score > best_score) {
							best_score = move_score;
							best_move = {i, j};
						}
					}
				}
			}

			return best_move;
		}

		void enemy_move() {
			print("Enemy's turn...", 1, 2);
			std::pair<int32_t, int32_t> best_move = find_best_move();

			field[best_move.first][best_move.second] = enemy_figure;
		}

		int32_t game_loop() {
			char winner = check_winner();
			if (winner == player_figure) {
				++player_score;
				return 0;
			}
			else if (winner == enemy_figure) {
				++enemy_score;
				return 1;
			}
			else if (!is_moves_left()) {
				return 2;
			}

			if (player_turn) player_move();
			else enemy_move();
			player_turn = !player_turn;
			return -1;
		}

		void restart() {
			field.fill({' ', ' ', ' '});
			player_figure = 0;
			while (!(player_figure == 'c' or player_figure == 'z')) {
				clear_screen();
				print("Tic Tac Toe!", 1, 1);
				print("Cross or zero? (c / z): ", 1, -1);
				std::cin >> player_figure;
			}
			if (player_figure == 'c') {
				enemy_figure = 'z';
				player_turn = true;
			}
			else enemy_figure = 'c';
		}

		void end_screen(int32_t status) {
			draw_ui();
			draw_field();
			if (status == 0) print("You won!", 1, 2);
			else if (status == 1) print("Enemy won!", 1, 2);
			else print("Draw!", 1, 2);

			while (true) {
				print("Would you like to play again? (y / n): ", 1, -1);
				char answer;
				std::cin >> answer;
				if (answer == 'y') {
					restart();
					break;
				}
				else if (answer == 'n') exit(0);
			}
		}
};

int32_t main() {
	getConsoleSize(COLS, ROWS);
	Game game;

	while (true) {
		getConsoleSize(COLS, ROWS);
		clear_screen();
		game.draw_ui();
		game.draw_field();
		int32_t status = game.game_loop();
		if (status != -1) game.end_screen(status);
	}

	return 0;
}