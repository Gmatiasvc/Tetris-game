#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

/*
DOC: class block
quick info: organizes the current block properties
*/
class block
{
	char shape[4][4];
	int w;
	int h;
	int color;
	int rotation;
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
void drawFrame(int board[12][21])
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
				case 1:
					//? render block border #969696
					printf("\033[38;2;150;150;150m%s",block);
					break;

				case 2:
					//? render block game #ff0000 <-> red
					printf("\033[38;2;255;0;0m%s",block);
					break;
				
				case 3:
					//? render block game #a5500 <-> brown
					printf("\033[38;2;170;85;0m%s",block);
					break;
				
				case 4:
					//? render block game #00ff00 <-> green
					printf("\033[38;2;0;255;0m%s",block);
					break;
				
				case 5:
					//? render block game #00ffff <-> cyan
					printf("\033[38;2;0;255;255m%s",block);
					break;
				
				case 6:
					//? render block game #ffffff <-> white
					printf("\033[38;2;255;255;255m%s",block);
					break;
				
				case 7:
					//? render block game #ff00ff <-> magenta
					printf("\033[38;2;255;0;255m%s",block);
					break;
				
				case 8:
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
DOC: function frame(int matrix board 12x21)
used in: game() <- draws the next frames
quick info: draws frames at a variable fps until a stable state is reached
*/
void frame(int board[12][21]){
	while (checkStable(board) != true)
	{
		setCursor(0,0);
		gravity(board);
		drawFrame(board);
		Sleep(100);
	}
}

/*
DOC: function rowCheck(int matrix board 12x21; bool list rowState |-> len:20)
used libs: stdio.h <- printf() as DEBUG
used in: rowElimination() <- get the number of completed rows
quick info: fills a list with bools to indicate if a row is full
*/
void rowCheck(int board[12][21], bool rowState[20]){
	bool rowComplete;
	for (int i = 0; i < 20; i++)
	{
		rowComplete = true;
		for (int j = 1; j < 11; j++)
		{
			if (board[j][i] == 0)
			{
				rowComplete = false;
				break;
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
void rowElimination(int board[12][21], int &score){
	bool rowState[24];
	rowCheck(board, rowState);
	int eliminations;
	for (int i = 0; i < 20; i++)
	{
		eliminations = 0;

		if (rowState[i] == true)
		{
			eliminations++;
			clearRow(board, i);
			score += 100;
			if (rowState[i+1] == true)
			{
				eliminations++;
				clearRow(board, i);
				score += 200;
				if (rowState[i+2] == true)
				{
					
					eliminations++;
					clearRow(board, i);
					score += 200;
					if (rowState[i+3] == true) // Nested if, with you till end of time <3
					{
						
						eliminations++;
						clearRow(board, i);
						score += 300;
					}
					
				}
				
			}
		}
	}
	frame(board);
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
		printf("\nNuevo record personal alcanzado!!! %i puntos\n", score);
	}
	printf("%i\n",pb);
	
}


/*
DOC: function game(int matrix board 12x21)
quick info: executes the game screen
*/
void game(int board[12][21], int score){

	frame(board);

	rowElimination(board, score);
	
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
		
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // col 0
		{0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 1
		{0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 2
		{0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 1}, // col 3
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 1}, // col 4
		{0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 1}, // col 5
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1}, // col 6
		{0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 1}, // col 7
		{0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1}, // col 8
		{0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 1}, // col 9
		{0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1}, // col 10
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  // col 11
		
	};
	int score = 0;

	//? Start the game
	cls();
	game(board,score);
	
}
