
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "print.h"
#include "game_setting.h"

#define BOARD_SIZE 6
#define TRUE 1
#define FALSE 0
// @brief 게임 상태를 나타내는 상수이다. 상수의 값에는 의미가 없다.
#define INIT (-10)	
#define WIN (-20)
#define LOSE (-30)
#define KEEP_ON (-40)
#define GAME_OVER (-50)


typedef struct Point {
	int row;
	int col;
}Point;


Point Get_Board_Position();
int IsOverLimit(int row, int col);
int Board_Update(char control_board[BOARD_SIZE][BOARD_SIZE], char showed_board[BOARD_SIZE][BOARD_SIZE], Point pos);
char IntToChar (int number);
int IsMine(char control_board[BOARD_SIZE][BOARD_SIZE], int row, int col);
int Get_Around_Mine_Number(char control_board[BOARD_SIZE][BOARD_SIZE], Point pos);
int Get_Game_Status(char control_board[BOARD_SIZE][BOARD_SIZE], int game_status);
void gotoXY(Point pos);


int main() {

	char control_board[BOARD_SIZE][BOARD_SIZE];
    char showed_board[BOARD_SIZE][BOARD_SIZE];

    int game_status = INIT;
	Point pos;

    Display_Welcome_Message();

	Sleep(1000);
    
	while (game_status == INIT || game_status == KEEP_ON) {
		if (game_status == INIT) {
			system("cls");
            Init_Game(control_board, showed_board);
			Print_Board(showed_board);
			printf("\n\nSelect a row: ");
			printf("\nSelect a collumn: ");
        }
		
		
		pos = Get_Board_Position();
		game_status = Board_Update(control_board, showed_board, pos);
		game_status = Get_Game_Status(control_board, game_status);
    }

    return 0;
}


Point Get_Board_Position() {
	
	Point pos;
	Point rpos, cpos, opos;
	int isOverLimit = FALSE;
	
	const int INPUT_ROW_X = 14;
	const int INPUT_ROW_Y = 13;
	const int INPUT_COL_X = 18;
	const int INPUT_COL_Y = 14;
	const int OVER_MESSAGE_X = 0;
	const int OVER_MESSAGE_Y = 17;

	rpos.col = INPUT_ROW_X;
	rpos.row = INPUT_ROW_Y;
	cpos.col = INPUT_COL_X;
	cpos.row = INPUT_COL_Y;
	opos.col = OVER_MESSAGE_X;
	opos.row = OVER_MESSAGE_Y;

    while (1) {
		gotoXY(rpos);
        scanf(" %d", &pos.row);
		gotoXY(cpos);
        scanf(" %d", &pos.col);

		gotoXY(rpos);
		printf(" ");
		gotoXY(cpos);
		printf(" ");
		gotoXY(opos);

        isOverLimit = IsOverLimit(pos.row, pos.col);
        if (isOverLimit) {
            printf("Value too big. Should go from 0 to %d. Try again", BOARD_SIZE);
			continue;
        }

		else {
			printf("r%d c%d is opened.                                   ",pos.row, pos.col);
			
			return pos;
		}
    }

	return pos;
}

int IsOverLimit(int row, int col) {

	if (row >= BOARD_SIZE)	return TRUE;
	if (col >= BOARD_SIZE)	return TRUE;
	else	return FALSE;
}

int Board_Update(char control_board[BOARD_SIZE][BOARD_SIZE], char showed_board[BOARD_SIZE][BOARD_SIZE], Point pos) {
   
    int mine_checker_feedback = Get_Around_Mine_Number(control_board, pos);
	Point cursor;

    const char OPENED = 'x';
	const int TOP_OF_BOARD = 5;
	const int LEFT_OF_BOARD = 1;
	const int NUM_BLANK = 3;

    if (mine_checker_feedback == LOSE) {

        return LOSE;
    }

    else {

        control_board[pos.row][pos.col] = OPENED;
        
        showed_board[pos.row][pos.col] = IntToChar(mine_checker_feedback);

		cursor.col = pos.col * NUM_BLANK + LEFT_OF_BOARD;
		cursor.row = pos.row + TOP_OF_BOARD;
		
		gotoXY(cursor);
        printf("%c", showed_board[pos.row][pos.col]);
		
		return KEEP_ON;
    }
}

char IntToChar (int number) {

	const int ASCII = 48;

	return number + ASCII;
}

int IsMine(char control_board[BOARD_SIZE][BOARD_SIZE], int row, int col) {
	char const MINE = '*';
	
	if (control_board[row][col] == MINE)	
		return TRUE;
	else	
		return FALSE;
}

int Get_Around_Mine_Number(char control_board[BOARD_SIZE][BOARD_SIZE], Point pos) {
	/*
	 * @brief   주위 폭탄 개수 리턴시, 사용자가 선택한 위치가 보드의 가장자리라면 내부 칸의 폭탄만 검사한다
	 * @params	control_board : 지뢰가 저장된 보드, row : 사용자가 선택한 보드 행, col : 사용자가 선택한 보드 열
	 * @return	사용자가 선택한 보드 위치에 지뢰가 있다면 -1 리턴, 그렇지 않다면 주변 폭탄 개수 리턴
	 */
	const char MINE = '*';
	const int BOARD_START_POINT = 0;

    int number_of_bomb = 0;
	int start_row_index = -1;
	int end_row_index = 1;
	int start_col_index = -1;
	int end_col_index = 1;
	int row_index;
	int col_index;

    if (control_board[pos.row][pos.col] == MINE) {
        return LOSE;
    }

    else {
		if( pos.row == BOARD_START_POINT ) start_row_index = 0;
		if( pos.col == BOARD_START_POINT ) start_col_index = 0;
		if( pos.row == BOARD_SIZE - 1) end_row_index = 0;
		if( pos.col == BOARD_SIZE - 1) end_col_index = 0;

		for(row_index = start_row_index; row_index <= end_row_index; row_index++) {
			for(col_index = start_col_index; col_index <= end_col_index; col_index++) {
				if (IsMine(control_board, pos.row+row_index, pos.col+col_index ))
					number_of_bomb ++;
			}
		}
    }

    return number_of_bomb;
}

int Get_Game_Status(char control_board[BOARD_SIZE][BOARD_SIZE], int game_status) {
    /*
	 * @brief	현재 게임 상태에 따라서 다음 게임 상태를 리턴하는 함수
	 * @param   control_board[][] : 사용자에게 보이지 않는 게임판
	 *			game_status : 게임의 상태
	 *			next_status : 다음 게임 상태를 나타내는 변수, 
	 *		                 현재 게임 상태가 WIN이나 LOSE가 아닐 경우 게임을 계속 진행해야 하기 때문에 KEEP_ON으로 초기화
	 * @return	게임 상태가 WIN이나 LOSE일 경우 Input_Replay_Game()에서 받은 리턴 값(INIT, GAMEOVER),
	 *			게임 상태가 WIN이나 LOSE가 아닐 경우 KEEP_ON
	 *			(INIT, GAMEOVER, KEEP_ON은 game_status가 가질 상수)
	 */

	int next_status = KEEP_ON;

	if (game_status == WIN) {
		system("cls");
        printf("\n\nYou did it! You cleared the board. Congratulations!!!\n\n");
		next_status = Input_Replay_Game(control_board);
	}

	else if (game_status == LOSE) {
		system("cls");
		printf("\n\nOh no! You hit a mine! ¯\\_(ツ)_/¯ \n\n");
		next_status = Input_Replay_Game(control_board);
	}

	else 
		return next_status;

	return next_status;
}

void gotoXY(Point pos){
   COORD cursor;

   cursor.X = pos.col;
   cursor.Y = pos.row;
   
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}