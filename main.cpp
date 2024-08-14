#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <algorithm>
#include <random>
#include <time.h>

using namespace std;

/*
DOC: class block
quick info: organizes the current block properties
*/
class block
{
public:
	int shape[4][4];
	int w;
	int h;
	int color;
};

/*
DOC: function cls()
used libs: windows.h <- system()
used in: main() <- clears the screen
quick info: wipes the console output
*/
void cls()
{
	system("cls");
}

void copyMatrix(int a[4][4], int b[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i][j] = b[i][j];
		}
	}
}

void copyBoard(int target[12][21], int origin[12][21])
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			target[i][j] = origin[i][j];
		}
	}
}

/*
DOC: function setCursor(int x; int y)
used in: game() <- sets the cursor for next draw
quick info: places de cursor in the indicated coordinated
*/
void setCursor(int x, int y)
{
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}

/*
DOC: function drawFrame(int matrix board 12x21)
used libs: stdio.h <- printf()
used in: game() <- draw the current state of the board
quick info: draws the board matrix
*/
void drawFrame(int board[12][21], int &elimRows, int &lvl, int &score)
{
	setCursor(0, 0);
	/*
	DOC: private char block
	quick info: this is the main block used in the drawing function in console mode
	defined str block; len: 2
	*/
	char block[3] = {"\xDB\xDB"}; // ██
	int cursor;
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			// DOC: private int cursor
			// quick info: this variable represents the current selection in board, solves memory problems
			cursor = board[j][i];

			/*
			DOC: nested double for loop
			quick info: this 2 for loops go through the whole array, reading the values of every entry and converting them in the console
			used libs: stdio.h <- printf()
			*/
			if (j == 11)
			{
				//? render block border #969696
				printf("\033[38;2;150;150;150m%s\n", block);
			}
			else if (cursor == 0)
			{
				printf("  ");
			}
			else
			{
				/*
				DOC: block render switch
				quick info: this switch takes the id selected and renders it in the console screen
				*/
				switch (cursor)
				{
				case 1:
				case 21:
					//? render block border #969696
					printf("\033[38;2;150;150;150m%s", block);
					break;

				case 2:
				case 22:
					//? render block game #ff0000 <-> red
					printf("\033[38;2;255;0;0m%s", block);
					break;

				case 3:
				case 23:
					//? render block game #a5500 <-> brown
					printf("\033[38;2;170;85;0m%s", block);
					break;

				case 4:
				case 24:
					//? render block game #00ff00 <-> green
					printf("\033[38;2;0;255;0m%s", block);
					break;

				case 5:
				case 25:
					//? render block game #00ffff <-> cyan
					printf("\033[38;2;0;255;255m%s", block);
					break;

				case 6:
				case 26:
					//? render block game #ffffff <-> white
					printf("\033[38;2;255;255;255m%s", block);
					break;

				case 7:
				case 27:
					//? render block game #ff00ff <-> magenta
					printf("\033[38;2;255;0;255m%s", block);
					break;

				case 8:
				case 28:
					//? render block game ##0000ff <-> blue
					printf("\033[38;2;0;0;255m%s", block);
					break;

				default:
					//? debug: error during rendering <-> id out of range
					printf("??");
				}
			}
		}
	}
	printf("\033[0;37m");

	setCursor(30, 1);
	printf("Tetris modo facil");

	setCursor(30, 3);
	printf("Puntaje", score);
	setCursor(30, 4);
	printf("%i", score);

	setCursor(30, 6);
	printf("Filas eliminadas", elimRows);
	setCursor(30, 7);
	printf("%i", elimRows);

	setCursor(30, 9);
	printf("Nivel", lvl);
	setCursor(30, 10);
	printf("%i", lvl);
}

void makeAllPermanent(int board[12][21])
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			if (board[i][j] > 20)
			{
				board[i][j] -= 20;
			}
		}
	}
}

/*
DOC: function gravityTick(int column |-> ?; int lim)
used in: gravity() <- in the for loops
quick info: makes entries fall until the limit is reached, int lim delimitates the floor
*/
void gravityTick(int list[], int lim)
{
	for (int i = lim; i != 0; i--)
	{
		if (i != 1)
		{
			list[i] = list[i - 1];
		}
		else
		{
			list[i] = 0;
		}
	}
}

/*
DOC: function checkStable(int matrix board 12x21)
used in: frame() <- allow next draw
quick info: checks if the board does not have falling blocks
*/
bool checkStable(int board[12][21])
{
	for (int i = 1; i < 11; i++)
	{
		for (int j = 1; j < 20; j++)
		{
			if (board[i][j + 1] == 0 && board[i][j] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

/*
DOC: function gravity(int matrix board 12x21)
used libs: stdio.h <- printf() as DEBUG
used in: frame() <- makes the blocks fall
quick info: makes the blocks fall to the bottom of the stack
*/
void gravity(int board[12][21])
{
	for (int i = 1; i < 11; i++)
	{
		for (int j = 1; j < 20; j++)
		{
			// printf("%i", board[i][j]);
			if (board[i][j + 1] == 0 && board[i][j] != 0)
			{
				gravityTick(board[i], j + 1);
				// printf("!");
				j++;
			}
			else
			{
				// printf(" ");
			}
		}
		// printf("\n");
	}
}

/*
DOC: function rowCheck(int matrix board 12x21; bool list rowState |-> len:20)
used libs: stdio.h <- printf() as DEBUG
used in: rowElimination() <- get the number of completed rows
quick info: fills a list with bools to indicate if a row is full
*/
void rowCheck(int board[12][21], bool rowState[24])
{
	bool rowComplete;
	for (int i = 0; i < 20; i++)
	{
		rowComplete = true;
		for (int j = 1; j < 11; j++)
		{
			if (board[j][i] == 0)
			{
				rowComplete = false;
			}
		}
		if (rowComplete)
		{
			// printf("Row %i is complete\n",i);
			rowState[i] = true;
		}
		else
		{
			// printf("Row %i is NOT complete\n",i);
			rowState[i] = false;
		}
	}
}

/*
DOC: function clearRow(int matrix board 12x21; int pos)
used in: rowElimination() <- remove the indicated row
quick info: fills the indicated row with 0
*/
void clearRow(int board[12][21], int pos)
{
	for (int i = 1; i < 11; i++)
	{
		board[i][pos] = 0;
	}
}

/*
DOC: function rowElimination(int matrix board 12x21; ref >-> int score)
used in game() <- eliminates full rows and assigns the respective points
quick info: removes rows that are full, with priority of 4 line combos, next 3 line and so on. After that assings the respective score
? 1 rows <- 100 pts
? 2 rows <- 300 pts
? 3 rows <- 500 pts
? 4 rows <- 800 pts
*/
void rowElimination(int board[12][21], int &score, int &fps, int &elimRows, int &lvl)
{
	bool rowState[24] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
	rowCheck(board, rowState);
	int eliminations;
	for (int i = 0; i < 20; i++)
	{
		eliminations = 0;

		if (rowState[i] == true)
		{
			eliminations++;
			elimRows++;
			clearRow(board, i);
			score += 100;
			rowState[i] = false;

			if (rowState[i + 1] == true)
			{
				eliminations++;
				elimRows++;
				clearRow(board, i + 1);
				score += 200;
				rowState[i + 1] = false;

				if (rowState[i + 2] == true)
				{

					eliminations++;
					elimRows++;
					clearRow(board, i + 2);
					score += 200;
					rowState[i + 2] = false;

					if (rowState[i + 3] == true) // Nested if, with you till end of time <3
					{

						eliminations++;
						elimRows++;
						clearRow(board, i + 3);
						score += 300;
						rowState[i + 3] = false;
					}
				}
			}
		}
	}
}

void UserInput(int &key)
{

	if (_kbhit())
	{

		switch (_getch())
		{
		case 'a':
		case 'A':
			key = 1;
			break;
		case 'd':
		case 'D':
			key = 2;
			break;
		case 'w':
		case 'W':
			key = 4;
			break;
		case 's':
		case 'S':
			key = 0;
			break;
		case 'x':
		case 'X':
			key = 3;
			break;
		}
	}
}

/*
DOC: function saveScore(int score)
used libs: fstream <- class fscore | stdio.h <- printf()
used in game() <- save the personal best
quick info: saves the personal best in the file score.pb. if the reached score is greater than the pb, it replaces it, oterwise, skips
*/
void saveScore(int score)
{
	fstream fscore("score.pb", ios::in);
	char pbChar[12];
	int pb;
	fscore.getline(pbChar, 12);
	fscore.close();
	pb = atoi(pbChar);

	if (pb < score)
	{
		ofstream fscore("score.pb", ios::trunc);
		fscore << score;
		fscore.close();
		setCursor(30, 11);
		printf("Nuevo record personal alcanzado!!! %i puntos", score);
	}
}

void spawnBlock(int board[12][21], bool &gameOver, block &selBlock)
{

	srand(time(0));
	int selBlockId = rand() % 18;

	if (selBlockId == 0)
	{

		selBlock.color = 22;
		selBlock.h = 4;
		selBlock.w = 1;
		int shape00[4][4] = {
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape00);
	}

	else if (selBlockId == 1)
	{

		selBlock.color = 22;
		selBlock.h = 1;
		selBlock.w = 4;
		int shape01[4][4] = {
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape01);
	}

	else if (selBlockId == 2)
	{

		selBlock.color = 26;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape10[4][4] = {
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape10);
	}

	else if (selBlockId == 3)
	{

		selBlock.color = 26;
		selBlock.h = 3;
		selBlock.w = 2;
		int shape11[4][4] = {
			{1, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape11);
	}

	else if (selBlockId == 4)
	{

		selBlock.color = 26;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape12[4][4] = {
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape12);
	}

	else if (selBlockId == 5)
	{

		selBlock.color = 26;
		selBlock.h = 3;
		selBlock.w = 2;
		int shape13[4][4] = {
			{1, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape13);
	}

	else if (selBlockId == 6)
	{

		selBlock.color = 27;
		selBlock.h = 3;
		selBlock.w = 2;
		int shape20[4][4] = {
			{0, 0, 1, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape20);
	}

	else if (selBlockId == 7)
	{

		selBlock.color = 27;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape21[4][4] = {
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape21);
	}

	else if (selBlockId == 8)
	{

		selBlock.color = 27;
		selBlock.h = 3;
		selBlock.w = 2;
		int shape22[4][4] = {
			{1, 1, 1, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape22);
	}
	else if (selBlockId == 9)
	{

		selBlock.color = 27;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape23[4][4] = {
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape23);
	}

	else if (selBlockId == 10)
	{

		selBlock.color = 28;
		selBlock.h = 2;
		selBlock.w = 2;
		int shape3[4][4] = {
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape3);
	}

	else if (selBlockId == 11)
	{

		selBlock.color = 24;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape40[4][4] = {
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape40);
	}

	else if (selBlockId == 12)
	{

		selBlock.color = 24;
		selBlock.h = 3;
		selBlock.w = 2;
		int shape41[4][4] = {
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape41);
	}

	else if (selBlockId == 13)
	{

		selBlock.color = 23;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape50[4][4] = {
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape50);
	}

	else if (selBlockId == 14)
	{

		selBlock.color = 23;
		selBlock.h = 3;
		selBlock.w = 2;
		int shape51[4][4] = {
			{0, 1, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape51);
	}

	else if (selBlockId == 15)
	{

		selBlock.color = 23;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape52[4][4] = {
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape52);
	}

	else if (selBlockId == 16)
	{

		selBlock.color = 23;
		selBlock.h = 3;
		selBlock.w = 2;
		int shape53[4][4] = {
			{1, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape53);
	}

	else if (selBlockId == 17)
	{

		selBlock.color = 25;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape60[4][4] = {
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape60);
	}

	else
	{

		selBlock.color = 25;
		selBlock.h = 2;
		selBlock.w = 3;
		int shape61[4][4] = {
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
		};
		copyMatrix(selBlock.shape, shape61);
	}

	int offset = 0;
	for (int i = 0; i < selBlock.w; i++)
	{
		for (int j = 0; j < selBlock.h; j++)
		{
			if (board[i + 5][j + 1 - offset] != 0)
			{
				gameOver = true;
				offset = -1;
			}
			if (selBlock.shape[i][j] == 1)
			{
				board[i + 5][j + 1 - offset] = selBlock.color;
			}
		}
	}
}

void moveBlock(int board[12][21], int direction, block selBlock, int &xOrigin)
{
	int buffer[12][21];
	int temp;
	copyBoard(buffer, board);
	if (xOrigin > 0 && xOrigin < 12 - selBlock.w)
	{
		if (direction == 1)
		{
			for (int i = 1; i < 11; i++)
			{
				for (int j = 1; j < 20; j++)
				{
					if (buffer[i][j] > 20)
					{
						temp = buffer[i][j];
						buffer[i][j] = buffer[i - 1][j];
						buffer[i - 1][j] = temp;
					}
					if (buffer[i][j] == 1)
					{
						return;
					}
				}
			}
			xOrigin--;
			copyBoard(board, buffer);
		}

		if (direction == 0)
		{
			for (int i = 10; i > 0; i--)
			{
				for (int j = 1; j < 20; j++)
				{
					if (buffer[i][j] > 20)
					{
						temp = buffer[i + 1][j];
						buffer[i + 1][j] = buffer[i][j];
						buffer[i][j] = temp;
					}
					if (buffer[i][j] == 1)
					{
						return;
					}
				}
			}
			xOrigin++;
			copyBoard(board, buffer);
		}
	}
}

bool startScreen()
{

	cls();
	fstream fscore("score.pb", ios::in);
	char pbChar[12];
	int pb;
	int sel;
	fscore.getline(pbChar, 12);
	fscore.close();
	pb = atoi(pbChar);

	setCursor(13, 1);
	printf("\033[1;4mTetris facil\033[0m");
	setCursor(5, 4);
	printf("Presiona \033[5;7mENTER\033[0m para iniciar");
	setCursor(8, 6);
	printf("Presiona \033[5mX\033[0m para salir");
	setCursor(8, 8);
	printf("\033[3mMejor puntaje: %i\033[0m", pb);

	sel = _getch();

	if (sel == 13)
		return true;

	else if (sel == 120 || sel == 88)
	{
		return false;
	}
}

bool endScreen(int score)
{
	cls();
	fstream fscore("score.pb", ios::in);
	char pbChar[12];
	int pb;
	int sel;
	fscore.getline(pbChar, 12);
	fscore.close();
	pb = atoi(pbChar);

	setCursor(8, 1);
	printf("\033[1mJuego terminado\033[0m");
	setCursor(5, 4);
	printf("Puntaje alcanzado: %i", score);
	setCursor(5, 6);
	printf("Mayor puntaje: %i", pb);
	if (score == pb)
	{
		setCursor(6, 8);
		printf("\033[38;2;253;220;92m Nuevo Record!!!\033[0m");
	}

	setCursor(6, 14);
	printf("REINTENTAR [\033[5;7mENTER\033[0m]");
	setCursor(9, 16);
	printf("SALIR [\033[5mX\033[0m]");

	sel = _getch();

	if (sel == 13)
		return true;

	else if (sel == 120 || sel == 88)
	{
		return false;
	}
}

/*
DOC: function game(int matrix board 12x21)
quick info: executes the game screen
*/
void game(int board[12][21], int score, bool &gameOver)
{
	int fps = 6;
	int elimRows = 0;
	int lvl = 0;
	int key = -1;
	int xOrigin;
	block selBlock;

	while (!gameOver)
	{
		xOrigin = 5;
		while (checkStable(board) != true)
		{
			UserInput(key);

			if (key == 3) // x key event
			{
				gameOver = true;
				break;
			}
			else if (key == 1) // left event
			{
				key = 0;
				moveBlock(board, 1, selBlock, xOrigin);
			}
			else if (key == 2) // right event
			{
				key = 0;
				moveBlock(board, 0, selBlock, xOrigin);
			}
			else if (key == 0) // down event
			{
				fps = 6;
				key = 0;
			}
			else if (key == 4) // up event
			{

				fps = 2;
				key = 4;
			}

			setCursor(0, 0);
			gravity(board);
			// moveBlock(board,1,selBlock,xOrigin);
			drawFrame(board, elimRows, lvl, score);
			// printf("  | %i | %i |", key,fps);

			Sleep(int(1000 / fps));
		}
		// drawFrame(board, elimRows, lvl, score);

		rowElimination(board, score, fps, elimRows, lvl);
		while (checkStable(board) != true)
		{
			gravity(board);
			drawFrame(board, elimRows, lvl, score);
			Sleep(int(500 / fps));
		}

		makeAllPermanent(board);
		spawnBlock(board, gameOver, selBlock);
	}

	saveScore(score);
}

/*
DOC: function main
quick info: this is the main function, self explanatory
*/
int main()
{
	/*
	DOC: private int board:
	quick info: this is the matrix where the game status will be saved
	defined matrix board; size: 12x21; type: int
	*/
	int score = 0;
	bool gameOver = false;
	bool gameStart = false;

board:;
	int board[12][21] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // col 0-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 1-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 2-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 3-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 4-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 5
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 6
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 7-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 8-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 9-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 10-
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}	 // col 11-
	};
	if (gameStart == true)
	{
		gameOver = false;
		goto game;
	}

	gameStart = startScreen();

	if (gameStart == false)
		goto end;

game:;
	cls();
	game(board, score, gameOver);

	gameStart = endScreen(score);
	if (gameStart == true)
		goto board;
end:;
	//? Start the game
}