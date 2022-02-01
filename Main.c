#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#include<stdio.h>
#include<SDL.h>
#include<stdbool.h>
#include<stdlib.h> //system("cls");
#include<time.h>
#include<SDL_ttf.h>
#include<string.h>
//-----------------------------------------------GAME-------------------------------------------------------------------
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool** board_table;
int score = 0;
int best_score = 0;
int falling_timer = 0;
TTF_Font* font;
bool quit = false;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT  1080
#define SPEED_OF_FALLING 1
#define SIZE_OF_PRIME_SQUARE 50
#define SIZE_OF_PRIME_SQUARE_FRAME 4
#define BOARD_TABLE_X 16 + 5 //row
#define BOARD_TABLE_Y 10     //col

bool** initBoardTable(int x, int y);
int randInt();
//-----------------------------------------------position_in_window-------------------------------------------------------
typedef struct //position in window
{
	int pos_x;
	int pos_y;
}position;
//-----------------------------------------------board--------------------------------------------------------------------
typedef struct //board
{
	SDL_Rect* left_wall;
	SDL_Rect* right_wall;
	SDL_Rect* bottom_wall;
	SDL_Rect* lose_line;
	int prime_squares_x;
	int prime_squares_y;
}board;

board* initBoard();
void drawBoard(board* board);
void drawingPrimeSquares(board* board, int r, int g, int b);
void removingLine();
//----------------------------------------------prime_square--------------------------------------------------------------
typedef struct // prime_square
{
	int pos_x;
	int pos_y;
}prime_square;

void spawnPrimeSquare(prime_square* sq, int x, int y, board* board); //x,y - coordinates in table
void primeSquareMoveLeft(prime_square* sq);
void primeSquareMoveRight(prime_square* sq);
void primeSquareMoveDown(prime_square* sq);
void primeSquareMoveUp(prime_square* sq);

bool primeSquareCheckLeft(prime_square* sq);
bool primeSquareCheckRight(prime_square* sq);
bool primeSquareCheckDown(prime_square* sq);
bool primeSquareCheckUp(prime_square* sq);
//-------------------------------------------------SQUARE-----------------------------------------------------------------
typedef struct
{
	bool if_active;
	prime_square* left_top;
	prime_square* right_top;
	prime_square* left_bot;
	prime_square* right_bot;
}square;

square* initSquare();
void spawnSquare(square* square, board* board);

void squareMoveLeft(square* sq);
void squareMoveRight(square* sq);
void squareMoveDown(square* sq); 

bool ifSquareBack(square* sq);
//-------------------------------------------------T_shape-----------------------------------------------------------------
typedef struct
{
	int pos;
	bool if_active;
	prime_square* middle_top;
	prime_square* left_bot;
	prime_square* middle_bot;
	prime_square* right_bot;
}t_shape;

t_shape* initT_shape();
void spawnT_shape(t_shape* t_s, board* board);

void t_shapeMoveLeft(t_shape* t_s);
void t_shapeMoveRight(t_shape* t_s);
void t_shapeMoveDown(t_shape* t_s);

void t_shapeTurnLeft(t_shape* t_s);
void t_shapeJustTurnLeft(t_shape* t_s);
void t_shapeTurnRight(t_shape* t_s);

bool ifT_shapeBack(t_shape* t_s);
//-------------------------------------------------I_shape-----------------------------------------------------------------
typedef struct
{
	int pos;
	bool if_active;
	prime_square* top;
	prime_square* middle_top;
	prime_square* middle_bot;
	prime_square* bot;
}i_shape;

i_shape* initI_shape();
void spawnI_shape(i_shape* i_s, board* board);

void i_shapeMoveLeft(i_shape* i_s);
void i_shapeMoveRight(i_shape* i_s);
void i_shapeMoveDown(i_shape* i_s);

void i_shapeTurnLeft(i_shape* i_s);
void i_shapeTurnRight(i_shape* i_s);

bool ifI_shapeBack(i_shape* i_s);
//----------------------------------------------Init and close------------------------------------------------------------
void initWindow();
SDL_Rect* initRectangle(int x, int y, int w, int h);
//--------------------------------------------------others----------------------------------------------------------------
void drawRectangle(SDL_Rect* rectangle, int r, int g, int b);
position getSpawningPoint(board* board);
void updateBoardTableInConsole(int x, int y);
void settingValuesFalse(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void settingValuesTrue(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

void movingObjectLeft(square* sq, t_shape* t_s, i_shape* i_s);
void movingObjectRight(square* sq, t_shape* t_s, i_shape* i_s);
void movingObjectDown(square* sq, t_shape* t_s, i_shape* i_s);

void ifObjectBack(square* sq, t_shape* t_s, i_shape* i_s, board* br);

void turningObjectLeft(t_shape* t_s, i_shape* i_s);
void turningObjectRight(t_shape* t_s, i_shape* i_s);

void falling(square* sq, t_shape* t_s, i_shape* i_s);

void RenderText();
void RenderTextTetris();

int main(int argc, char** argv)
{
	//Initialize SDL, window, renderer, board, board_table
	if (TTF_Init() < 0)
	{
		printf("Error initializing TTF: %s\n", TTF_GetError());
		return false;
	}

	initWindow();
	board* board_ = initBoard();
	board_table = initBoardTable(BOARD_TABLE_X, BOARD_TABLE_Y);

	square* sq;
	sq = initSquare();

	t_shape* t_s;
	t_s = initT_shape();
	spawnT_shape(t_s, board_);

	i_shape* i_s = initI_shape();

	//event handler
	SDL_Event event;

	//game loop
	while (!quit)
	{
		//handle events queue
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDLK_ESCAPE)
			{
				printf("Esc clicked\n");
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					movingObjectLeft(sq, t_s, i_s);
					break;

				case SDLK_RIGHT:
					movingObjectRight(sq, t_s, i_s);
					break;

				case SDLK_DOWN:
					movingObjectDown(sq, t_s, i_s);
					break;

				case SDLK_a:
					turningObjectLeft(t_s, i_s);
					break;

				case SDLK_d:
					turningObjectRight(t_s, i_s);
					break;
				
				default:
					break;
				}
			}
		}

		//clear screen
		SDL_RenderClear(renderer);

		//update
		ifObjectBack(sq, t_s, i_s, board_);
		falling(sq, t_s, i_s);

		//draw
		drawingPrimeSquares(board_, 102, 255, 255);
		drawBoard(board_);
		RenderText();

		//show what was drawn
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;

	return 0;
}
//----------------------------------------------PRIME_SQUARE-----------------------------------------------
void primeSquareMoveUp(prime_square* sq)
{
		board_table[sq->pos_x][sq->pos_y] = false;
		sq->pos_x--;
		board_table[sq->pos_x][sq->pos_y] = true;
}
void primeSquareMoveDown(prime_square* sq)
{
		board_table[sq->pos_x][sq->pos_y] = false;
		sq->pos_x++;
		board_table[sq->pos_x][sq->pos_y] = true;
}

void primeSquareMoveRight(prime_square* sq)
{
		board_table[sq->pos_x][sq->pos_y] = false;
		sq->pos_y++;
		board_table[sq->pos_x][sq->pos_y] = true;
}

void primeSquareMoveLeft(prime_square* sq)
{
		board_table[sq->pos_x][sq->pos_y] = false;
		sq->pos_y--;
		board_table[sq->pos_x][sq->pos_y] = true;
}

bool primeSquareCheckLeft(prime_square* sq)
{
	if (board_table[sq->pos_x ][sq->pos_y - 1] == false) return true;
	return false;
}
bool primeSquareCheckRight(prime_square* sq)
{
	if (board_table[sq->pos_x][sq->pos_y + 1] == false) return true;
	return false;
}
bool primeSquareCheckDown(prime_square* sq)
{
	if (board_table[sq->pos_x + 1][sq->pos_y] == false) return true;
	return false;
}
bool primeSquareCheckUp(prime_square* sq)
{
	if (board_table[sq->pos_x - 1][sq->pos_y] == false) return true;
	return false;
}

void spawnPrimeSquare(prime_square* sq, int x, int y, board* board)
{
	sq->pos_x = x;
	sq->pos_y = y;

	board_table[x][y] = true;
}

void drawingPrimeSquares(board* board, int r, int g, int b)
{
	position pos = getSpawningPoint(board);

	SDL_Rect* black_frame = initRectangle(pos.pos_x, pos.pos_y, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);

	SDL_Rect* color_frame = initRectangle(pos.pos_x + SIZE_OF_PRIME_SQUARE_FRAME, pos.pos_y + SIZE_OF_PRIME_SQUARE_FRAME
		, SIZE_OF_PRIME_SQUARE - 2* SIZE_OF_PRIME_SQUARE_FRAME, SIZE_OF_PRIME_SQUARE - 2* SIZE_OF_PRIME_SQUARE_FRAME);

	SDL_Rect* black_fill = initRectangle(pos.pos_x + 2* SIZE_OF_PRIME_SQUARE_FRAME, pos.pos_y + 2* SIZE_OF_PRIME_SQUARE_FRAME
		, SIZE_OF_PRIME_SQUARE - 4* SIZE_OF_PRIME_SQUARE_FRAME, SIZE_OF_PRIME_SQUARE - 4* SIZE_OF_PRIME_SQUARE_FRAME);

	for (int i = 1; i <= BOARD_TABLE_X; i++)
	{
		for (int j = 1; j <= BOARD_TABLE_Y; j++)
		{
			if (board_table[i][j] == true)
			{
				drawRectangle(black_frame, 0, 0, 0);
				drawRectangle(color_frame, r, g, b);
				drawRectangle(black_fill, 0, 0, 0);
			}
			black_frame->x += SIZE_OF_PRIME_SQUARE;
			color_frame->x += SIZE_OF_PRIME_SQUARE;
			black_fill->x += SIZE_OF_PRIME_SQUARE;
		}
		black_frame->x = pos.pos_x;
		color_frame->x = pos.pos_x + SIZE_OF_PRIME_SQUARE_FRAME;
		black_fill->x = pos.pos_x + 2* SIZE_OF_PRIME_SQUARE_FRAME;

		black_frame->y += SIZE_OF_PRIME_SQUARE;
		color_frame->y += SIZE_OF_PRIME_SQUARE;
		black_fill->y += SIZE_OF_PRIME_SQUARE;
	}
	free(black_frame);
	free(color_frame);
	free(black_fill);
	
}
//------------------------------------------BOARD------------------------------------------------------------
board* initBoard()
{
	int line_thickness = 10;
	board* new_board;
	new_board = (board*)malloc(sizeof(board));

	int left_x = (WINDOW_WIDTH / 2) - (5 * SIZE_OF_PRIME_SQUARE) - line_thickness - SIZE_OF_PRIME_SQUARE_FRAME;
	int left_y = 0;
	int right_x = (WINDOW_WIDTH / 2) + (5 * SIZE_OF_PRIME_SQUARE) + SIZE_OF_PRIME_SQUARE_FRAME;
	int right_y = 0;
	int bottom_x = left_x;
	int bottom_y = WINDOW_HEIGHT - line_thickness;
	int bottom_width = right_x - left_x;

	int lose_line_y = bottom_y;
	if (WINDOW_WIDTH == 1920 && WINDOW_HEIGHT == 1080)
	{
		lose_line_y -= (16 * SIZE_OF_PRIME_SQUARE) + SIZE_OF_PRIME_SQUARE_FRAME;
		new_board->prime_squares_y = 16 + 5;
		new_board->prime_squares_x = 10;
	}
	else
	{
		lose_line_y -= (14 * SIZE_OF_PRIME_SQUARE) + SIZE_OF_PRIME_SQUARE_FRAME;
		new_board->prime_squares_y = 14 + 5;
		new_board->prime_squares_x = 10;
	}
	
	new_board->left_wall = initRectangle(left_x, left_y, line_thickness, WINDOW_HEIGHT);
	new_board->right_wall = initRectangle(right_x, right_y, line_thickness, WINDOW_HEIGHT);
	new_board->lose_line = initRectangle(bottom_x, lose_line_y, bottom_width, 1);
	new_board->bottom_wall = initRectangle(bottom_x, bottom_y, bottom_width, line_thickness);
	return new_board;

}

void drawBoard(board* board)
{
	drawRectangle(board->lose_line, 204, 51, 0);
	drawRectangle(board->left_wall, 255, 255, 255);
	drawRectangle(board->right_wall, 255, 255, 255);
	drawRectangle(board->bottom_wall, 255, 255, 255);
}

void removingLine()
{
	int counter;
	int i;
	for (int i = BOARD_TABLE_X; i >= 6; i--)
	{
		counter = 0;
		for (int j = 1; j <= BOARD_TABLE_Y; j++)
		{
			if (board_table[i][j] == true) counter++;
		}
		if (counter == BOARD_TABLE_Y)
		{
			score++;
			for (int ii = i; ii >= 6; ii--)
			{
				for (int jj = 1; jj <= BOARD_TABLE_Y; jj++)
				{
					board_table[ii][jj] = board_table[ii - 1][jj];
				}
			}
			i++;
		}
	}
	for (int i = 1; i <= BOARD_TABLE_Y; i++)
	{
		if (board_table[5][i] == true)
		{
			quit = true;
		}
	}
}
//-----------------------------------------------OTHERS-------------------------------------------------------
void settingValuesFalse(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	board_table[x1][y1] = false;
	board_table[x2][y2] = false;
	board_table[x3][y3] = false;
	board_table[x4][y4] = false;
}
void settingValuesTrue(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	board_table[x1][y1] = true;
	board_table[x2][y2] = true;
	board_table[x3][y3] = true;
	board_table[x4][y4] = true;
}
SDL_Rect* initRectangle(int x, int y, int w, int h)
{
	SDL_Rect* new_rectanle;
	new_rectanle = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	new_rectanle->x = x;
	new_rectanle->y = y;
	new_rectanle->w = w;
	new_rectanle->h = h;

	return new_rectanle;
}
void initWindow()
{
	font = TTF_OpenFont("Arial.ttf", 30);
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) printf("Error in SDL_CreateWindow: %s\n", SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}
void RenderText()
{
	
	char numtext[100];
	char word[] = "Score: ";
	char asd[100];
	SDL_Rect dest;
	//TTF_Font* font;
	SDL_Surface* text;
	SDL_Texture* texture;
	SDL_Color xd = { 102, 255, 255 };

	snprintf(numtext, 10, "%d", score);
	sprintf(asd, "%s%s", word, numtext);

	//font = TTF_OpenFont("Arial.ttf", 30);
	text = TTF_RenderText_Solid(font, asd, xd);
	texture = SDL_CreateTextureFromSurface(renderer, text);
	dest.x = 0;
	dest.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(renderer, texture, NULL, &dest);

	SDL_FreeSurface(text);
	SDL_DestroyTexture(texture);
}
//------------------------------------------------------------------------------------------------------
position getSpawningPoint(board* board)
{
	position spawning_point;
	spawning_point.pos_x = board->lose_line->x + board->left_wall->w + SIZE_OF_PRIME_SQUARE_FRAME;
	spawning_point.pos_y = board->lose_line->y - (5 * SIZE_OF_PRIME_SQUARE);

	return spawning_point;
}

void drawRectangle(SDL_Rect* rectangle, int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	SDL_RenderFillRect(renderer, rectangle);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void updateBoardTableInConsole(int x, int y)
{
	system("cls");
	for (int i = 0; i < x + 2; i++)
	{
		for (int j = 0; j < y + 2; j++)
		{
			if (i == 0 || i == x+1 || j == 0 || j == y+1)
			{
				printf("# ");
			}
			else if (board_table[i][j] == true)
			{
				printf("1 ");
			}
			else
			{
				printf("0 ");
			}
		}
		printf("\n");
	}
}

//------------------------------------------GAME------------------------------------------------------------
bool** initBoardTable(int x, int y)
{
	bool** new_table = (bool**)malloc((x + 2) * sizeof(bool*));
	for (int i = 0; i < x + 2; i++)
	{
		new_table[i] = (bool*)malloc((y + 2) * sizeof(bool*));
	}
	for (int i = 0; i < x + 2; i++)
	{
		for (int j = 0; j < y + 2; j++)
		{
			if (j == 0 || j == y + 1 || i == x + 1 || i == 0)
			{
				new_table[i][j] = true;
				printf("1 ");
			}

			else
			{
				new_table[i][j] = false;
				printf("0 ");
			}
		}
		printf("\n");
	}
	return new_table;
}

int randInt()
{
	srand(time(NULL));
	return rand() % 3;
}

void movingObjectLeft(square* sq, t_shape* t_s, i_shape* i_s)
{
	if (sq->if_active) squareMoveLeft(sq);
	else if (t_s->if_active) t_shapeMoveLeft(t_s);
	else if (i_s->if_active) i_shapeMoveLeft(i_s); 
}
void movingObjectRight(square* sq, t_shape* t_s, i_shape* i_s)
{
	if (sq->if_active) squareMoveRight(sq);
	else if (t_s->if_active) t_shapeMoveRight(t_s);
	else if (i_s->if_active) i_shapeMoveRight(i_s);
}
void movingObjectDown(square* sq, t_shape* t_s, i_shape* i_s)
{
	if (sq->if_active) squareMoveDown(sq);
	else if (t_s->if_active) t_shapeMoveDown(t_s);
	else if (i_s->if_active) i_shapeMoveDown(i_s);
}

void ifObjectBack(square* sq, t_shape* t_s, i_shape* i_s, board* br)
{
	bool if_backed = false;
	if (ifSquareBack(sq))
	{
		if_backed = true;
		removingLine();
		sq->if_active = false;
	}
	else if (ifT_shapeBack(t_s))
	{
		if_backed = true;
		removingLine();
		t_s->if_active = false;
	}
	else if (ifI_shapeBack(i_s))
	{
		if_backed = true;
		removingLine();
		i_s->if_active = false;
	}
	if (if_backed)
	{
		falling_timer = 0;
		int num = randInt();
		if (num == 0)
		{
			spawnSquare(sq, br);
		}
		else if(num == 1)
		{
			spawnT_shape(t_s, br);
		}
		else
		{
			spawnI_shape(i_s, br);
		}
	}
}

void turningObjectLeft(t_shape* t_s, i_shape* i_s)
{
	if (t_s->if_active) t_shapeTurnLeft(t_s);
	else if (i_s->if_active) i_shapeTurnLeft(i_s);
}

void turningObjectRight(t_shape* t_s, i_shape* i_s)
{
	if (t_s->if_active) t_shapeTurnRight(t_s);
	else if (i_s->if_active) i_shapeTurnLeft(i_s);
}
void falling(square* sq, t_shape* t_s, i_shape* i_s)
{
	falling_timer++;
	if (falling_timer >= 20)
	{
		movingObjectDown(sq, t_s, i_s);
		falling_timer = 0;
	}
}
//-----------------------------------------------SQUARE----------------------------------------------
square* initSquare()
{
	square* res = (square*)malloc(sizeof(square));
	res->if_active = false;
	res->left_top = initRectangle(1, 5, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->right_top = initRectangle(1, 6, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->left_bot = initRectangle(2, 5, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->right_bot = initRectangle(2, 6, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	return res;
}
void spawnSquare(square* square, board* board)
{
	square->if_active = true;
	spawnPrimeSquare(square->left_top, 1, 5, board);
	spawnPrimeSquare(square->right_top, 1, 6, board);
	spawnPrimeSquare(square->left_bot, 2, 5, board);
	spawnPrimeSquare(square->right_bot, 2, 6, board);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void squareMoveLeft(square* sq)
{
	settingValuesFalse(sq->left_top->pos_x, sq->left_top->pos_y,   sq->right_top->pos_x, sq->right_top->pos_y, 
		sq->left_bot->pos_x, sq->left_bot->pos_y, sq->right_bot->pos_x, sq->right_bot->pos_y);

	if (primeSquareCheckLeft(sq->left_top) && primeSquareCheckLeft(sq->left_bot) && primeSquareCheckLeft(sq->right_top) && primeSquareCheckLeft(sq->right_bot))
	{
		primeSquareMoveLeft(sq->left_top);
		primeSquareMoveLeft(sq->left_bot);
		primeSquareMoveLeft(sq->right_top);
		primeSquareMoveLeft(sq->right_bot);
	}
	settingValuesTrue(sq->left_top->pos_x, sq->left_top->pos_y, sq->right_top->pos_x, sq->right_top->pos_y,
		sq->left_bot->pos_x, sq->left_bot->pos_y, sq->right_bot->pos_x, sq->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void squareMoveRight(square* sq)
{
	settingValuesFalse(sq->left_top->pos_x, sq->left_top->pos_y, sq->right_top->pos_x, sq->right_top->pos_y,
		sq->left_bot->pos_x, sq->left_bot->pos_y, sq->right_bot->pos_x, sq->right_bot->pos_y);

	if (primeSquareCheckRight(sq->right_top) && primeSquareCheckRight(sq->right_bot) && primeSquareCheckRight(sq->left_top) && primeSquareCheckRight(sq->left_bot))
	{
		primeSquareMoveRight(sq->right_top);
		primeSquareMoveRight(sq->right_bot);
		primeSquareMoveRight(sq->left_top);
		primeSquareMoveRight(sq->left_bot);
	}
	settingValuesTrue(sq->left_top->pos_x, sq->left_top->pos_y, sq->right_top->pos_x, sq->right_top->pos_y,
		sq->left_bot->pos_x, sq->left_bot->pos_y, sq->right_bot->pos_x, sq->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void squareMoveDown(square* sq)
{
	settingValuesFalse(sq->left_top->pos_x, sq->left_top->pos_y, sq->right_top->pos_x, sq->right_top->pos_y,
		sq->left_bot->pos_x, sq->left_bot->pos_y, sq->right_bot->pos_x, sq->right_bot->pos_y);

	if (primeSquareCheckDown(sq->right_bot) && primeSquareCheckDown(sq->left_bot) && primeSquareCheckDown(sq->right_top) && primeSquareCheckDown(sq->left_top) )
	{
		primeSquareMoveDown(sq->right_bot);
		primeSquareMoveDown(sq->left_bot);
		primeSquareMoveDown(sq->left_top);
		primeSquareMoveDown(sq->right_top);
	}
	settingValuesTrue(sq->left_top->pos_x, sq->left_top->pos_y, sq->right_top->pos_x, sq->right_top->pos_y,
		sq->left_bot->pos_x, sq->left_bot->pos_y, sq->right_bot->pos_x, sq->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}


bool ifSquareBack(square* sq)
{
	if (sq->if_active == false) return false;
	if (primeSquareCheckDown(sq->left_bot) == false || primeSquareCheckDown(sq->right_bot) == false) return true;
	return false;
}

//-----------------------------------------------T_shape----------------------------------------------
t_shape* initT_shape()
{
	t_shape* res = (t_shape*)malloc(sizeof(t_shape));
	res->if_active = false;
	res->middle_top = initRectangle(1, 4, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->left_bot = initRectangle(2, 4, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->middle_bot = initRectangle(2, 3, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->right_bot = initRectangle(2, 5, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	return res;
}

void spawnT_shape(t_shape* t_s, board* board)
{
	t_s->pos = 1;
	t_s->if_active = true;
	spawnPrimeSquare(t_s->middle_top, 1, 5, board);
	spawnPrimeSquare(t_s->left_bot, 2, 4, board);
	spawnPrimeSquare(t_s->middle_bot, 2, 5, board);
	spawnPrimeSquare(t_s->right_bot, 2, 6, board);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}

void t_shapeMoveLeft(t_shape* t_s)
{
	settingValuesFalse(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);

	if (primeSquareCheckLeft(t_s->middle_top) && primeSquareCheckLeft(t_s->left_bot) && primeSquareCheckLeft(t_s->middle_bot) && primeSquareCheckLeft(t_s->right_bot))
	{
		primeSquareMoveLeft(t_s->middle_top);
		primeSquareMoveLeft(t_s->left_bot);
		primeSquareMoveLeft(t_s->middle_bot);
		primeSquareMoveLeft(t_s->right_bot);
	}
	settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void t_shapeMoveRight(t_shape* t_s)
{
	settingValuesFalse(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);

	if (primeSquareCheckRight(t_s->middle_top) && primeSquareCheckRight(t_s->right_bot) && primeSquareCheckRight(t_s->middle_bot) && primeSquareCheckRight(t_s->left_bot))
	{
		primeSquareMoveRight(t_s->middle_top);
		primeSquareMoveRight(t_s->right_bot);
		primeSquareMoveRight(t_s->middle_bot);
		primeSquareMoveRight(t_s->left_bot);
	}
	settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void t_shapeMoveDown(t_shape* t_s)
{
	settingValuesFalse(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);

	if (primeSquareCheckDown(t_s->left_bot) && primeSquareCheckDown(t_s->middle_bot) && primeSquareCheckDown(t_s->right_bot) && primeSquareCheckDown(t_s->middle_top))
	{
		primeSquareMoveDown(t_s->middle_bot);
		primeSquareMoveDown(t_s->left_bot);
		primeSquareMoveDown(t_s->right_bot);
		primeSquareMoveDown(t_s->middle_top);
	}
	settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}

bool ifT_shapeBack(t_shape* t_s)
{
	if (t_s->if_active == false) return false;
	settingValuesFalse(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	if (primeSquareCheckDown(t_s->left_bot) == false || primeSquareCheckDown(t_s->middle_bot) == false || primeSquareCheckDown(t_s->right_bot) == false
		|| primeSquareCheckDown(t_s->middle_top) == false)
	{
		settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
			t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
		return true;
	}
	settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	return false;
}

void t_shapeTurnLeft(t_shape* t_s)
{
	settingValuesFalse(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	int x = t_s->middle_top->pos_x, y = t_s->middle_top->pos_y;
	if (t_s->pos == 1)
	{
		if (board_table[x + 2][y] == false)
		{
			t_s->pos = 4;
			t_s->middle_top->pos_x++; t_s->middle_top->pos_y--;
			t_s->left_bot->pos_x++; t_s->left_bot->pos_y++;
			
			t_s->right_bot->pos_x--; t_s->right_bot->pos_y--;
		}

	}
	else if (t_s->pos == 2)
	{
		if (board_table[x][y - 2] == false)
		{
			t_s->pos--;
			t_s->middle_top->pos_x--;  t_s->middle_top->pos_y--;
			t_s->left_bot->pos_x++;  t_s->left_bot->pos_y--;
			
			t_s->right_bot->pos_x--; t_s->right_bot->pos_y++;

		}
	}
	else if (t_s->pos == 3)
	{
		if (board_table[x - 2][y] == false)
		{
			t_s->pos--;
			t_s->middle_top->pos_x--;  t_s->middle_top->pos_y++;
			t_s->left_bot->pos_x--; t_s->left_bot->pos_y--;
			
			t_s->right_bot->pos_x++; t_s->right_bot->pos_y++; 
		}
	}
	else
	{
		if (board_table[x][y + 2] == false)
		{
			t_s->pos--;
			t_s->middle_top->pos_x++;  t_s->middle_top->pos_y++; 
			t_s->left_bot->pos_x = x; t_s->left_bot->pos_y = y + 2; 
			
			t_s->right_bot->pos_x = x; t_s->right_bot->pos_y = y; 
		}
	}
	settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void t_shapeJustTurnLeft(t_shape* t_s)
{
	settingValuesFalse(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	int x = t_s->middle_top->pos_x, y = t_s->middle_top->pos_y;
	if (t_s->pos == 1)
	{
		if (board_table[x + 2][y] == false)
		{
			t_s->pos = 4;
			t_s->middle_top->pos_x++; t_s->middle_top->pos_y--;
			t_s->left_bot->pos_x++; t_s->left_bot->pos_y++;
			t_s->right_bot->pos_x--; t_s->right_bot->pos_y--;
		}

	}
	else if (t_s->pos == 2)
	{
		if (board_table[x][y - 2] == false)
		{
			t_s->pos--;
			t_s->middle_top->pos_x--;  t_s->middle_top->pos_y--;
			t_s->left_bot->pos_x++;  t_s->left_bot->pos_y--;
			t_s->right_bot->pos_x--; t_s->right_bot->pos_y++;
		}
	}
	else if (t_s->pos == 3)
	{
		if (board_table[x - 2][y] == false)
		{
			t_s->pos--;
			t_s->middle_top->pos_x--;  t_s->middle_top->pos_y++;
			t_s->left_bot->pos_x--; t_s->left_bot->pos_y--;
			t_s->right_bot->pos_x++; t_s->right_bot->pos_y++;
		}
	}
	else
	{
		if (board_table[x][y + 2] == false)
		{
			t_s->pos--;
			t_s->middle_top->pos_x++;  t_s->middle_top->pos_y++;
			t_s->left_bot->pos_x = x; t_s->left_bot->pos_y = y + 2;

			t_s->right_bot->pos_x = x; t_s->right_bot->pos_y = y;
		}
	}
	settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
}
void t_shapeTurnRight(t_shape* t_s)
{
	settingValuesFalse(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	int x = t_s->middle_top->pos_x, y = t_s->middle_top->pos_y;
	if (t_s->pos == 1)
	{
		if (board_table[x + 2][y] == false)
		{
			t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s);
		}

	}
	else if (t_s->pos == 2)
	{
		if (board_table[x][y - 2] == false)
		{
			t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s);
		}
	}
	else if (t_s->pos == 3)
	{
		if (board_table[x - 2][y] == false)
		{
			t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s);
		}
	}
	else
	{
		if (board_table[x][y + 2] == false)
		{
			t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s); t_shapeJustTurnLeft(t_s);
		}
	}
	settingValuesTrue(t_s->middle_top->pos_x, t_s->middle_top->pos_y, t_s->left_bot->pos_x, t_s->left_bot->pos_y,
		t_s->middle_bot->pos_x, t_s->middle_bot->pos_y, t_s->right_bot->pos_x, t_s->right_bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
//-------------------------------------------------I_shape-----------------------------------------------------------------
i_shape* initI_shape()
{
	i_shape* res = (i_shape*)malloc(sizeof(i_shape));
	res->if_active = false;
	res->top = initRectangle(1, 4, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->middle_top = initRectangle(1, 5, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->middle_bot = initRectangle(1, 6, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	res->bot = initRectangle(1, 7, SIZE_OF_PRIME_SQUARE, SIZE_OF_PRIME_SQUARE);
	return res;
}
void spawnI_shape(i_shape* i_s, board* board)
{
	i_s->pos = 1;
	i_s->if_active = true;
	spawnPrimeSquare(i_s->top, 2, 4, board);
	spawnPrimeSquare(i_s->middle_top, 2, 5, board);
	spawnPrimeSquare(i_s->middle_bot, 2, 6, board);
	spawnPrimeSquare(i_s->bot, 2, 7, board);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}

void i_shapeMoveLeft(i_shape* i_s)
{
	settingValuesFalse(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);

	if (primeSquareCheckLeft(i_s->top) && primeSquareCheckLeft(i_s->middle_top) && primeSquareCheckLeft(i_s->middle_bot) && primeSquareCheckLeft(i_s->bot))
	{
		primeSquareMoveLeft(i_s->top);
		primeSquareMoveLeft(i_s->middle_top);
		primeSquareMoveLeft(i_s->middle_bot);
		primeSquareMoveLeft(i_s->bot);
	}
	settingValuesTrue(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void i_shapeMoveRight(i_shape* i_s)
{
	settingValuesFalse(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);

	if (primeSquareCheckRight(i_s->top) && primeSquareCheckRight(i_s->middle_top) && primeSquareCheckRight(i_s->middle_bot) && primeSquareCheckRight(i_s->bot))
	{
		primeSquareMoveRight(i_s->top);
		primeSquareMoveRight(i_s->middle_top);
		primeSquareMoveRight(i_s->middle_bot);
		primeSquareMoveRight(i_s->bot);
	}
	settingValuesTrue(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}
void i_shapeMoveDown(i_shape* i_s)
{
	settingValuesFalse(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);

	if (primeSquareCheckDown(i_s->top) && primeSquareCheckDown(i_s->middle_top) && primeSquareCheckDown(i_s->middle_bot) && primeSquareCheckDown(i_s->bot))
	{
		primeSquareMoveDown(i_s->top);
		primeSquareMoveDown(i_s->middle_top);
		primeSquareMoveDown(i_s->middle_bot);
		primeSquareMoveDown(i_s->bot);
	}
	settingValuesTrue(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
	updateBoardTableInConsole(BOARD_TABLE_X, BOARD_TABLE_Y);
}

bool ifI_shapeBack(i_shape* i_s)
{
	if (i_s->if_active == false) return false;
	settingValuesFalse(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
	if (primeSquareCheckDown(i_s->bot) == false || primeSquareCheckDown(i_s->middle_bot) == false || primeSquareCheckDown(i_s->top) == false
		|| primeSquareCheckDown(i_s->middle_top) == false)
	{
		settingValuesTrue(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
			i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
		return true;
	}
	settingValuesTrue(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
	return false;
}

void i_shapeTurnLeft(i_shape* i_s)
{
	settingValuesFalse(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
	if (i_s->pos == 1)
	{
		if (board_table[i_s->middle_top->pos_x - 1][i_s->middle_top->pos_y] == false && board_table[i_s->middle_top->pos_x - 2][i_s->middle_top->pos_y] == false &&
			board_table[i_s->middle_top->pos_x + 1][i_s->middle_top->pos_y] == false)
		{
			i_s->pos++;
			i_s->top->pos_x++; i_s->top->pos_y++;
			 
			i_s->middle_bot->pos_y--; i_s->middle_bot->pos_x--;
			i_s->bot->pos_x -= 2; i_s->bot->pos_y -= 2;
		}
	}
	else if (i_s->pos == 2)
	{
		if (board_table[i_s->middle_top->pos_x][i_s->middle_top->pos_y - 1] == false && board_table[i_s->middle_top->pos_x][i_s->middle_top->pos_y + 1] == false &&
			board_table[i_s->middle_top->pos_x][i_s->middle_top->pos_y + 2] == false)
		{
			i_s->pos--;
			i_s->top->pos_x--; i_s->top->pos_y--;

			i_s->middle_bot->pos_y++; i_s->middle_bot->pos_x++;
			i_s->bot->pos_x += 2; i_s->bot->pos_y += 2;
		}
	}
	settingValuesTrue(i_s->top->pos_x, i_s->top->pos_y, i_s->middle_top->pos_x, i_s->middle_top->pos_y,
		i_s->middle_bot->pos_x, i_s->middle_bot->pos_y, i_s->bot->pos_x, i_s->bot->pos_y);
}