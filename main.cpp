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
void cls(){
	system("cls");
}

void copyMatrix(int a[4][4], int b[4][4]){
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a[i][j] = b[i][j];
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
	/* 
	DOC: private char block
	quick info: this is the main block used in the drawing function in console mode
	defined str block; len: 2
	*/ 
	char block[3] = {"\xDB\xDB"};

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			// DOC: private int cursor
			// quick info: this variable represents the current selection in board, solves memory problems
			int cursor = board[j][i];

			/*
			DOC: nested double for loop
			quick info: this 2 for loops go through the whole array, reading the values of every entry and converting them in the console
			used libs: stdio.h <- printf()
			*/
			if (j == 11)
			{
				//? render block border #969696
				printf("\033[38;2;150;150;150m%s\n",block);
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
				case 1: case 21:
					//? render block border #969696
					printf("\033[38;2;150;150;150m%s",block);
					break;

				case 2: case 22:
					//? render block game #ff0000 <-> red
					printf("\033[38;2;255;0;0m%s",block);
					break;
				
				case 3: case 23:
					//? render block game #a5500 <-> brown
					printf("\033[38;2;170;85;0m%s",block);
					break;
				
				case 4: case 24:
					//? render block game #00ff00 <-> green
					printf("\033[38;2;0;255;0m%s",block);
					break;
				
				case 5: case 25:
					//? render block game #00ffff <-> cyan
					printf("\033[38;2;0;255;255m%s",block);
					break;
				
				case 6: case 26:
					//? render block game #ffffff <-> white
					printf("\033[38;2;255;255;255m%s",block);
					break;
				
				case 7: case 27:
					//? render block game #ff00ff <-> magenta
					printf("\033[38;2;255;0;255m%s",block);
					break;
				
				case 8: case 28:
					//? render block game ##0000ff <-> blue
					printf("\033[38;2;0;0;255m%s",block);
					break;
				
				default:
					//? debug: error during rendering <-> id out of range
					printf("??");
				}
			}
			
		}
		
	}
	printf("\033[0;37m");

	setCursor(30,1);
	printf("Tetris modo facil");

	setCursor(30,3);
	printf("Puntaje",score);
	setCursor(30,4);
	printf("%i",score);

	setCursor(30,6);
	printf("Filas eliminadas",elimRows);
	setCursor(30,7);
	printf("%i",elimRows);
	
	setCursor(30,9);
	printf("Nivel",lvl);
	setCursor(30,10);
	printf("%i",lvl);
}

void makeAllPermanent(int board[12][21]){
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			if (board[i][j] >20)
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
void gravityTick(int list[], int lim){
	for (int i = lim; i != 0; i--)
	{
		if (i != 1)
		{
			list[i] = list[i-1];
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
bool checkStable(int board[12][21]){
	for (int i = 1; i < 11; i++)
	{
		for (int j = 1; j < 20; j++)
		{
			if (board[i][j+1] == 0 && board[i][j] != 0)
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
void gravity(int board[12][21]) {
	for (int i = 1; i < 11; i++)
	{
		for (int j = 1; j < 20; j++)
		{
			//printf("%i", board[i][j]);
			if (board[i][j+1] == 0 && board[i][j] != 0)
			{
				gravityTick(board[i], j+1);
				//printf("!");
				j++;
			}
			else
			{
				//printf(" ");
			}
			
		}
		//printf("\n");
	}
}


/*
DOC: function rowCheck(int matrix board 12x21; bool list rowState |-> len:20)
used libs: stdio.h <- printf() as DEBUG
used in: rowElimination() <- get the number of completed rows
quick info: fills a list with bools to indicate if a row is full
*/
void rowCheck(int board[12][21], bool rowState[24]){
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
			//printf("Row %i is complete\n",i);
			rowState[i] = true;
		}
		else
		{
			//printf("Row %i is NOT complete\n",i);
			rowState[i] = false;
		}
		
	}
	
}

/*
DOC: function clearRow(int matrix board 12x21; int pos)
used in: rowElimination() <- remove the indicated row
quick info: fills the indicated row with 0
*/
void clearRow(int board[12][21], int pos){
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
void rowElimination(int board[12][21], int &score, int &fps, int &elimRows, int &lvl){
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
			
			if (rowState[i+1] == true)
			{
				eliminations++;
				elimRows++;
				clearRow(board, i+1);
				score += 200;
				rowState[i+1] = false;

				if (rowState[i+2] == true)
				{
					
					eliminations++;
					elimRows++;
					clearRow(board, i+2);
					score += 200;
					rowState[i+2] = false;

					if (rowState[i+3] == true) // Nested if, with you till end of time <3
					{
						
						eliminations++;
						elimRows++;
						clearRow(board, i+3);
						score += 300;
						rowState[i+3] = false;
					}
					
				}
				
			}
		}
	}
}


void UserInput(int &key) 
{ 
	
	if (_kbhit()) { 

		switch (_getch()) { 
		case 'a': 
			key = 1; 
			break; 
		case 'd': 
			key = 2; 
			break; 
		case 'w': 
			key = 4;
			break; 
		case 's': 
			key = 0; 
			break; 
		case 'x': 
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
void saveScore(int score){
	fstream fscore("score.pb", ios::in);
	char pbChar[12];
	int pb;
	fscore.getline(pbChar,12);
	fscore.close();
	pb = atoi(pbChar);

	if (pb < score)
	{
		ofstream fscore("score.pb", ios::trunc);
		fscore << score;
		fscore.close();
		setCursor(30,11);
		printf("\nNuevo record personal alcanzado!!! %i puntos\n", score);
	}
	
}

void spawnBlock(int board[12][21], bool &gameOver, block &selBlock){

	block block_I_0;
	block_I_0.color = 22; 
	block_I_0.h = 4;
	block_I_0.w = 1;
	int shape00[4][4]= { 
		{1,1,1,1},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_I_0.shape, shape00);

	block block_I_1;
	block_I_1.color = 22; 
	block_I_1.h = 1;
	block_I_1.w = 4;
	int shape01[4][4]= { 
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
	};
	copyMatrix(block_I_1.shape, shape01);
	
	block block_J_0;
	block_J_0.color = 26; 
	block_J_0.h = 2;
	block_J_0.w = 3;
	int shape10[4][4]= { 
		{0,1,0,0},
		{0,1,0,0},
		{1,1,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_J_0.shape, shape10);

	
	block block_J_1;
	block_J_0.color = 26; 
	block_J_0.h = 3;
	block_J_0.w = 2;
	int shape11[4][4]= { 
		{1,1,1,0},
		{0,0,1,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_J_0.shape, shape11);
	
	block block_J_2;
	block_J_0.color = 26; 
	block_J_0.h = 2;
	block_J_0.w = 3;
	int shape12[4][4]= { 
		{1,1,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_J_0.shape, shape12);
	
	block block_J_3;
	block_J_0.color = 26; 
	block_J_0.h = 3;
	block_J_0.w = 2;
	int shape13[4][4]= { 
		{1,0,0,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_J_0.shape, shape13);

	block block_L_0;
	block_L_0.color = 27; 
	block_L_0.h = 3;
	block_L_0.w = 2;
	int shape20[4][4]= { 
		{0,0,1,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_L_0.shape, shape20);

	block block_L_1;
	block_L_1.color = 27; 
	block_L_1.h = 2;
	block_L_1.w = 3;
	int shape21[4][4]= { 
		{1,0,0,0},
		{1,0,0,0},
		{1,1,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_L_1.shape, shape21);
	block block_L_2;
	block_L_2.color = 27; 
	block_L_2.h = 3;
	block_L_2.w = 2;
	int shape22[4][4]= { 
		{1,1,1,0},
		{1,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_L_2.shape, shape22);
	block block_L_3;
	block_L_3.color = 27; 
	block_L_3.h = 2;
	block_L_3.w = 3;
	int shape23[4][4]= { 
		{1,1,0,0},
		{0,1,0,0},
		{0,1,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_L_3.shape, shape23);

	block block_O_0;
	block_O_0.color = 28; 
	block_O_0.h = 2;
	block_O_0.w = 2;
	int shape3[4][4]= { 
		{1,1,0,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_O_0.shape, shape3);


	block block_S_0;
	block_S_0.color = 24; 
	block_S_0.h = 2;
	block_S_0.w = 3;
	int shape40[4][4]= { 
		{1,0,0,0},
		{1,1,0,0},
		{0,1,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_S_0.shape, shape40);

	block block_S_1;
	block_S_1.color = 24; 
	block_S_1.h = 3;
	block_S_1.w = 2;
	int shape41[4][4]= { 
		{0,1,1,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_S_1.shape, shape41);

	
	block block_T_0;
	block_T_0.color = 23; 
	block_T_0.h = 2;
	block_T_0.w = 3;
	int shape50[4][4]= { 
		{1,0,0,0},
		{1,1,0,0},
		{1,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_T_0.shape, shape50);

	block block_T_1;
	block_T_1.color = 23; 
	block_T_1.h = 3;
	block_T_1.w = 2;
	int shape51[4][4]= { 
		{0,1,0,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_T_1.shape, shape51);

	block block_T_2;
	block_T_2.color = 23; 
	block_T_2.h = 2;
	block_T_2.w = 3;
	int shape52[4][4]= { 
		{0,1,0,0},
		{1,1,0,0},
		{0,1,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_T_2.shape, shape52);

	block block_T_3;
	block_T_3.color = 23; 
	block_T_3.h = 3;
	block_T_3.w = 2;
	int shape53[4][4]= { 
		{1,1,1,0},
		{0,1,0,0},
		{0,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_T_3.shape, shape53);

	block block_Z_0;
	block_Z_0.color = 25; 
	block_Z_0.h = 2;
	block_Z_0.w = 3;
	int shape60[4][4]= { 
		{0,1,0,0},
		{1,1,0,0},
		{1,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_Z_0.shape, shape60);

	block block_Z_1;
	block_Z_1.color = 25; 
	block_Z_1.h = 2;
	block_Z_1.w = 3;
	int shape61[4][4]= { 
		{0,1,0,0},
		{1,1,0,0},
		{1,0,0,0},
		{0,0,0,0},
	};
	copyMatrix(block_Z_1.shape, shape61);
	
	srand(time(0)); 
	int selBlockId = rand()%18;
		

	if (selBlockId == 0)
	{
		selBlock = block_I_0;
	}
		
	else if (selBlockId == 1)
	{
		selBlock = block_I_1;
	}
	else if (selBlockId == 2)
	{
		selBlock = block_J_0;
	}
	else if (selBlockId == 3)
	{
		selBlock = block_J_1;
	}
	else if (selBlockId == 4)
	{
		selBlock = block_J_2;
	}
	else if (selBlockId == 5)
	{
		selBlock = block_J_3;
	}
	else if (selBlockId == 6)
	{
		selBlock = block_L_0;
	}
	else if (selBlockId == 7)
	{
		selBlock = block_L_1;
	}
	else if (selBlockId == 8)
	{
		selBlock = block_L_2;
	}
	else if (selBlockId == 9)
	{
		selBlock = block_L_3;
	}
	else if (selBlockId == 10)
	{
		selBlock = block_O_0;
	}
	else if (selBlockId == 11)
	{
		selBlock = block_S_0;
	}
	else if (selBlockId == 12)
	{
		selBlock = block_S_1;
	}
	else if (selBlockId == 13)
	{
		selBlock = block_T_0;
	}
	else if (selBlockId == 14)
	{
		selBlock = block_T_1;
	}
	else if (selBlockId == 15)
	{
		selBlock = block_T_2;
	}
	else if (selBlockId == 16)
	{
		selBlock = block_T_3;
	}
	else if (selBlockId == 17)
	{
		selBlock = block_Z_0;
	}
	else
	{
		selBlock = block_Z_1;
	}

	int offset = 0;
	for (int i = 0; i < selBlock.w ; i++)
	{
		for (int j = 0; j < selBlock.h; j++)
		{
			if(board[i+5][j+1-offset] != 0)
			{
				gameOver = true;
				offset = -1;
				
			}
			if (selBlock.shape[i][j] == 1)
			{
				board[i+5][j+1-offset] = selBlock.color;
			}
			}
	}
}

void moveBlock(int board[12][21], int direction, block selBlock, int &xOrigin){

	int temp;

	if (xOrigin > 1 && xOrigin < 11 - selBlock.w )
	{ 
		if (direction == 1)
		{
			for (int i = 1; i < 11; i++)
			{
				for (int j = 1; j < 20; j++)
				{
					if (board[i][j] > 20)
					{
						temp = board[i][j];
						board[i][j] = board[i-1][j];
						board[i-1][j] = temp;
					}
				}
			}
			xOrigin--;
		}

		if (direction == 0)
		{
			for (int i = 10; i > 0; i--)
			{
				for (int j = 1; j < 20; j++)
				{
					if (board[i][j] > 20)
					{
						temp = board[i+1][j];
						board[i+1][j] = board[i][j];
						board[i][j] = temp;
					}
				}
			}
			xOrigin++;
		}
	}	
}

/*
DOC: function game(int matrix board 12x21)
quick info: executes the game screen
*/
void game(int board[12][21], int score, bool &gameOver){
	int fps = 4;
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
			
			if (key == 3)
			{
				gameOver = true;
				break;
			}
			else if (key == 1) //left event
			{
				key = 0;
				moveBlock(board, 1, selBlock, xOrigin);
			}
			else if (key == 2) //right event
			{
				key = 0;
				moveBlock(board, 0, selBlock, xOrigin);
			}
			else if (key == 0) //down event
			{
				fps = 6;
				key = 0;
			}
			else if (key == 4) //up event
			{
				
				fps = 2;
				key = 4;
			}
			
			
			setCursor(0,0);
			gravity(board);
			//moveBlock(board,1,selBlock,xOrigin);
			drawFrame(board,elimRows,lvl,score);
			
			//printf("  | %i | %i |", key,fps);

			Sleep(int(1000/fps));
		}

		rowElimination(board,score, fps, elimRows,lvl);
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
	int board[12][21]={
		
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // col 0-
		{0, 0, 0, 0, 0, 0, 4, 4, 5, 5, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 1-
		{0, 0, 0, 0, 0, 4, 4, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 2-
		{0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 5, 1}, // col 3-
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3, 4, 1}, // col 4-
		{0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 2, 2, 0, 0, 0, 0, 0, 0, 5, 0, 1}, // col 5
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 1, 4, 0, 0, 0, 0, 1}, // col 6
		{0, 0, 0, 0, 0, 0, 7, 0, 0, 2, 2, 2, 0, 0, 0, 4, 4, 0, 0, 0, 1}, // col 7-
		{0, 0, 0, 0, 7, 7, 7, 0, 0, 2, 2, 2, 0, 0, 0, 0, 4, 0, 0, 0, 1}, // col 8-
		{0, 0, 0, 0, 0, 0, 3, 3, 3, 2, 2, 8, 8, 8, 0, 0, 0, 0, 0, 0, 1}, // col 9-
		{0, 0, 0, 0, 0, 0, 0, 3, 0, 5, 5, 5, 0, 8, 0, 0, 0, 0, 0, 0, 1}, // col 10-
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  // col 11-
		
	};
	int score = 0;
	bool gameOver = false;
	//? Start the game
	cls();
	game(board, score, gameOver);
	
}
