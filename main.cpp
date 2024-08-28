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
CREDITOS: ---

~0% generado por IA!!!
*/


/*
DOC: class block
quick info: organizes the current block properties

ES
DOC: clase block
info rapida: organiza las propiedades del bloque seleccionado
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

ES
DOC: funcion cls()
libs usadas: windows.h <- system()
usado en: main() <- limpia la pantalla
info rapida: limpia todo lo dibujado en la consola
*/
void cls()
{
	system("cls");
}


/*
DOC: function copyMatriz(int matrix a 4x4, int matrix b 4x4)
used in: spawnBlock() <- sets the selected block shape
quick info: copies the data from the matrix b into the matrix a

ES
DOC: funcion copyMatrix(matriz entero a 4x4, matriz entero b 4x4)
usado en: spawnBlock() <- asigna la forma del bloque seleccionado
info rapida: copia los datos de la matriz b a la matriz a
*/
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


/*
DOC: function copyBoard(int matrix target 12x21, int matrix origin 12x21)
used in: moveBlock() <- copies the information of the board to a buffer back and forth
quick info: copies the data from the origin matrix to the target matrix

ES
DOC: funcion copyBoard(matriz entero targer 12x21, matriz entero origin 12x21)
usado en: moveBlock <- copia la informacion de la matriz board a la matriz buffer o viceversa
info rapida: copia los datos de la matriz origin a la matriz targer
*/
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
DOC: function setCursor(int x, int y)
used in: game(), drawframe(), startScreen(), endScreen() <- sets the cursor for next draw
quick info: places de cursor in the indicated coordinated

ES
DOC: funcion setCursor(entero x, entero y)
usado en: game(), drawframe(), startScreen(), endScreen() <- coloca el cursor en posicion para iniciar el dibujado de pantalla
info rapida: coloca el cursor en las coordenadas indicadas
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
DOC: function drawFrame(int matrix board 12x21, int ref elimRows, int ref lvl, int ref score, int fps)
used libs: stdio.h <- printf()
used in: game() <- draw the current state of the board
quick info: draws the board matrix

ES
DOC: funcion drawFrame(matriz entero board 12x21, entero referencia elimRows, entero referencia lvl, entero referencia score, entero fps)
libs usadas: stdio.h <- printf()
usado en: game() <- dibuja la matriz board
info rapida: dibuja la matriz board
*/
void drawFrame(int board[12][21], int &elimRows, int &lvl, int &score, int fps)
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
	// printf ("[target DPS: %i]",fps);
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


/*
DOC: function makeAllPermanent(int matrix board 12x21)
used in: game() <- makes blocks unmovable
quick info: reduces the data of a point in the board by 20, only if the point has more than 20 in it's data. This makes the block not to move with moveBlock()

ES
DOC: funcion makeAllPermanent(matriz entero board 12x21)
usado en: game <- hace que los bloques no puedan ser movidos por el jugador
info rapida: reduce el dato de un punto en la matriz board en 20, solo si el punto tiene mas de 20 en su data. Esto hace que el bloque no pueda ser movido con moveBlock()
*/
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
DOC: function gravityTick(int list column |-> ?, int lim)
used in: gravity() <- in the for loops
quick info: makes entries fall until the limit is reached, int lim delimitates the floor

ES
DOC: funcion gravityTick(entero lisa column |-> ?, entero lim)
usado en: gravoty <- dentro de los bucles for
info rapida: hace que las entradas "caigan" hasta que lim es alcanzado, lim indica donde esta el suelo
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
DOC: function checkStable(int matrix board 12x21) ->> bool
used in: frame() <- allow next draw
quick info: checks if the board does not have falling blocks

ES
DOC: funcion checkStable(matriz entero board 12x21) ->> booleano
usado en: frame() <- permite el siguiente dibujo de la matriz board
info rapida: revisa si la matriz board tiene bloques que pueden caer
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

ES
DOC: funcion gravity(matriz entero board 12x21)
libs usadas: stdio.h <- printf() usado en DEBUG
usado en: frame() <- hace que los bloques caigan
info rapida: hace los bloqeus caer todo lo que puedan
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
DOC: function rowCheck(int matrix board 12x21, bool list rowState |-> len:24)
used libs: stdio.h <- printf() as DEBUG
used in: rowElimination() <- get the number of completed rows
quick info: fills a list with bools to indicate if a row is full

ES
DOC: funcion rowCheck(matriz entero board 12x21, booleano lista rowState |-> len:24)
libs usadas: stdio.h <- printf() usado en DEBUG
usado en: rowElimination() <- obtiene el numero de filas llenas
info rapida: llena una lista con boolenaos que indican si una fila esta completa
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

ES
DOC: funcion clearRow(matriz entero board 12x21; entero pos)
usado en: rowElimination() <- elimina la fila indicada
info rapida: llena la fina indicada con 0
*/
void clearRow(int board[12][21], int pos)
{
	for (int i = 1; i < 11; i++)
	{
		board[i][pos] = 0;
	}
}


/*
DOC: function rowElimination(int matrix board 12x21, int ref score, int ref fps, int ref elimRows, int ref lvl)
used in game() <- eliminates full rows and assigns the respective points
quick info: removes rows that are full, with priority of 4 line combos, next 3 line and so on. After that assings the respective score
? 1 rows <- 100 pts
? 2 rows <- 300 pts
? 3 rows <- 500 pts
? 4 rows <- 800 pts

ES
DOC: funcion rowElimination(matriz entero board 12x21, entero referencia score, entero referencia fps, entero referencia elimRows, entero referencia lvl)
usado en: game() <- elimina las filas llenas y asigna los puntos respectivos
info rapida: elimina las filas que estan llenas, con prioridad a combos de 4 lines, luego 3 lineas y si sucesivamente. Luego asigna los puntos respectivos
? 1 fila <- 100 pts
? 2 fila <- 300 pts
? 3 fila <- 500 pts
? 4 fila <- 800 pts
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


/*
DOC: function UserInput(int ref key)
used libs: conio.h <- _kbhit() & _getch()
used in: game() <- registers a key press
quick info: registers a key press and assings a value to key as corresponds.
? s, S <- 0
? a, A <- 1
? d, D <- 2
? x, X <- 3
? w, W <- 4

ES
DOC: funcion UserInput(entero referencia key)
libs usadas: conio.h <- _kbhit() & _getch()
usado en: game() <- registra una tecla presionada
info rapida: registra cuando se presiona una tecla y se asigna un valor segun corresponda
? s, S <- 0
? a, A <- 1
? d, D <- 2
? x, X <- 3
? w, W <- 4
*/
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

ES
DOC:
libs usadas:
usado en:
info rapida:
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


/*
DOC: function spawnBlock(int matrix board 12x21, bool ref gameOver, block ref selBlock)
used in: game() <- spawns a new block or finishes the game
quick info: selects a random block from the original tetris with a predetermined rotation. Then it spawns it with +20 data in its points, to make it movable. If the block cannot be spawned withouth colliding with other blocks, the function will signal the game over.

ES
DOC: funcion spawnBlock(matriz entero board 12x21, booleano referencia gameOver, block referencia selBlock)
usado en: game() <- genera un nuevo bloque o termina el juego
info rapida: selecciona un bloque aleatorio del tetris original con una rotacion predeterminada. Luego lo genera con +20 en sus puntos, para hacerlo movible con las teclas que el usuario presiona. Si el bloque no puede ser generado sin colisionar con otros, la funcion indica que el juego ha terminado.
*/
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


/*
DOC: function moveBlock(int matrix board 12x21, int direction, block selBlock, int ref xOrigin)
used in: game() <- processes a moveBlock event
quick info: moves all the points in the board that are marked as movable. It copies the board to a buffer, then tries to execute the operation, if the operation fails, as in trying to switch with a non 0 point, the buffer will be discarted and not passed as the board thus not drawing it. 

ES
DOC: funcion moveBlock(matriz entero board 12x21, entero direccion, block selBlock, entero referencia xOrigin)
usado en: game() <- procesa el evento de movimiento de bloques
info rapida: mueve todos los puntos en la matriz board que estan marcados como movibles. Copía board a un buffer y luego intenta mover los bloques, si esto falla, en intentnando mover los bloques en un lugar ya ocupado, el buffer se descarta, sino se copia a la matriz.
*/
void moveBlock(int board[12][21], int direction, block selBlock, int &xOrigin)
{
	int buffer[12][21];
	int temp;
	int yPos;
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

						if (buffer[i - 1][j] > 1 && buffer[i - 1][j] < 19)
						{
							return;
						}
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
						if (buffer[i + 1][j] > 1 && buffer[i + 1][j] < 19)
						{
							return;
						}
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


/*
DOC: function startScreen() ->> bool
used libs: fstream <- type fstream ; conio.h <- _getch()
used in: main() <- shows an start screen and signals the start or end of the game
quick info: shows a start screen. Shows the title, options and current high score
? x, X <- exit
? [enter] <- start

ES
DOC: funcion startScreen() ->> bool
libs usadas: fstream <- type fstream ; conio.h <- _getch()
usado en: main() <- muestra una pantalla de entrada y señala si el juego inicia o termina
info rapida: muestra una pantalla de entrada, muestra el titulo, opciones y mayor puntaje
? x, X <- salir
? [enter] <- entrar
*/
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

	while (true)
	{
		sel = _getch();
		if (sel == 13)
			return true;

		else if (sel == 120 || sel == 88)
		{
			return false;
		}
		else
		{
			printf("\a");
		}
	}
}


/*
DOC: function endScreen() ->> bool
used libs: fstream <- type fstream ; conio.h <- _getch()
used in: main() <- shows a game over screen and signals the restart or exit of the game
quick info: shows a game over screen. Shows achieved score, options and if there is a new personal best
? x, X <- exit
? [enter] <- restart

ES
DOC: funcion endScreen() ->> bool
libs usadas: fstream <- type fstream ; conio.h <- _getch()
usado en: main() <- muestra una pantalla de juego terminado y señala si el juego se reinicia o sale
info rapida: muestra una pantalla de juego terminado, muestra el puntaje alcanzado, opciones y si se ha alcanzado un nuevo mayor puntaje
? x, X <- salir
? [enter] <- reiniciar

*/
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
	setCursor(6, 6);
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
	while (true)
	{
		sel = _getch();
		if (sel == 13)
			return true;

		else if (sel == 120 || sel == 88)
		{
			return false;
		}
		else
		{
			printf("\a");
		}
	}
}


/*
DOC: function levelCheck(int ref lvl, int elimRows, int ref fpsBaseline)
used in: game() <- checks the current level and scales difficulty
quick info: updates the level by +1 when 10 rows are cleared, also speeds the game by +2 fps

ES
DOC: funcion levelCheck(entero referencia lvl, entero elimRows, entero referencia fpsBaseline)
usado en: game() <- revisa el nivel actual y escala la dificultad
info rapida: actualiza el nivel +1 cuando 10 filas han sido eliminadas, tambien hace que el juego se acelece con +2 fps
*/
void levelCheck(int &lvl, int elimRows, int &fpsBaseline)
{
	if (elimRows >= 10 * (lvl + 1))
	{
		lvl++;
		fpsBaseline += 2;
	}
}


/*
DOC: function waitTime(int fps, clock_t start, clock_t end, int base)
used libs: time.h <- type clocl_t
used in: game() <- gets the time to wait
quick info: calculates the time left to complete the fps cycle, keeping the waiting period between presses constant

ES
DOC: funcion waitTime(entero fps, clock_t start, clock_t end, entero base)
libs usadas: time.h <- type clocl_t
usado en: game() <- calcula el tiempo que tiene que esperar el programa
info rapida: clacula el tiempo restante para completar el ciclo de fps, manteniendo un tiempo de espera constante entre actualizaciones
*/
int waitTime(int fps, clock_t start, clock_t end, int base)
{
	clock_t innerStart = clock();
	double timeSpent = (double)(end - start) / (clock_t)1;
	clock_t innerEnd = clock();
	double innterTimeSpent = (double)(innerEnd - innerStart) / (clock_t)1;
	int finalWaitTime = int(base / fps) - timeSpent - innterTimeSpent;
	if (finalWaitTime < 1)
	{
		finalWaitTime = 1;
	}

	return finalWaitTime;
}


/*
DOC: function game(int matrix board 12x21, int ref score, int ref gameOver)
quick info: executes the game screen

ES
DOC: funcion game(matriz entero board 12x21, entero referencia score, entero referencia gameOver)
info rapida: ejecuta la pantalla del juego
*/
void game(int board[12][21], int &score, bool &gameOver)
{
	int fps = 4;
	int fpsBaseline = 4;
	int elimRows = 0;
	int lvl = 0;
	int key = -1;
	int xOrigin;

	clock_t start;
	clock_t end;
	double sleepTime;

	block selBlock;

	while (!gameOver)
	{
		xOrigin = 5;
		while (checkStable(board) != true)
		{
			start = clock();

			UserInput(key);

			if (key == 3) // x key event
			{
				gameOver = true;
				break;
			}
			else if (key == 1) // left event
			{
				key = -1;
				moveBlock(board, 1, selBlock, xOrigin);
			}
			else if (key == 2) // right event
			{
				key = -1;
				moveBlock(board, 0, selBlock, xOrigin);
			}
			else if (key == 0) // down event
			{
				fps = fpsBaseline * 2;
				key = -1;
			}
			else if (key == 4) // up event
			{

				fps = fpsBaseline / 2;
				key = -1;
			}
			else if (key == -1)
			{
				fps = fpsBaseline;
			}

			setCursor(0, 0);
			gravity(board);
			// moveBlock(board,1,selBlock,xOrigin);
			drawFrame(board, elimRows, lvl, score, fps);
			// printf("  | %i | %i |", key,fps);
			end = clock();

			sleepTime = waitTime(fps, start, end, 1000);
			Sleep(sleepTime);
		}
		// drawFrame(board, elimRows, lvl, score);

		rowElimination(board, score, fps, elimRows, lvl);
		levelCheck(lvl, elimRows, fpsBaseline);
		while (checkStable(board) != true)
		{
			start = clock();
			gravity(board);
			drawFrame(board, elimRows, lvl, score, fps);
			end = clock();
			sleepTime = waitTime(fps, start, end, 500);
			Sleep(sleepTime);
		}

		makeAllPermanent(board);
		spawnBlock(board, gameOver, selBlock);
	}

	saveScore(score);
}


/*
DOC: function main()
quick info: this is the main function, self explanatory

ES
DOC: funcion main()
info rapida: esta es la funcin main, se explica solo
*/
int main()
{
	//defined int score; defined bool gameOver flag; defined bool gameStart flag 
	int score = 0;
	bool gameOver = false;
	bool gameStart = false;

// defined point board
board:;
	/*
	DOC: private int board:
	quick info: this is the matrix where the game status will be saved
	defined matrix board; size: 12x21; type: int
	*/
	int board[12][21] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // col 0
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 1
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 2
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 3
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 4
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 5
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 6
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 7
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 8
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 9
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // col 10
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}	 // col 11
	};
	if (gameStart == true)
	{
		gameOver = false;
		score = 0;
		goto game; //? goes to point: game
	}

	gameStart = startScreen();

	if (gameStart == false)
		goto end; //? goes to point: end

//defined point game
game:;
	cls();
	game(board, score, gameOver);

	gameStart = endScreen(score);
	if (gameStart == true)
		goto board; //? goes to point: board

//defined point end
end:;
}