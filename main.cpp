#include <stdio.h>
using namespace std;


/* 
DOC: function drawFrame(int matrix board 12x21)
used libs: stdio.h <- printf()
used in: main() <- start of the main loop
quick info:
*/
void drawFrame(int board[21][12])
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
			int cursor = board[i][j];

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
	int board[21][12]= {
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 1},
		{1, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 1},
		{1, 0, 0, 0, 4, 4, 0, 0, 8, 8, 0, 1},
		{1, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
		};
	
	drawFrame(board);

	//TODO: REMOVE THIS PART
	char* a;  //! TEMP; remove when not needed
	scanf(a); //! TEMP; remove when not needed
	return 0;
}
