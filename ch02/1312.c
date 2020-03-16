//--------헤더파일---------
#include<stdio.h>
#include <Windows.h>                             
#include <conio.h>    
#include<stdlib.h>
#pragma warning(disable:4996)

//-----가중치-------
#define Gard22 5
#define Block22 15
#define ABlock33 100
#define Gard33 600
#define Block33 500
#define Gard44 150000
#define Gard_43 17000
#define Gard333 350
#define Attack22 1000
#define Attack33 5000
#define Attack44 200000
#define Attack43 10000
#define Attack33 8000
//----------------

//----------------------------------------------------------함수---------------------------------------------------------------------
int board[20][20] = { 0 }; //오목판 : 0:아무것도 없음 1:흰돌 2:흑돌 3:가장자리(가장자리에서 33,44를 허용하기위해 만듬) 
void gotoxy(int x, int y); //커서(출력위치)를 x,y좌표로 옮겨줌 
void intro(); //게임 설명
void draw_board(); //보드판을 그리는 함수
void draw_narrow_board(); //좁은 보드판을 그리는 함수
void draw_stone(int board[][20]);
void clear_chat(); //채팅로그를 지워주는 함수 
void weight_m1(int weight_board[][20], int board[][20], int x, int y); //흑돌 주위의 가중치 -=1
void weight_p1(int weight_board[][20], int board[][20], int x, int y); //백돌 주위의 가중치 +=1
void AI(int board[][20], int weight_board[][20], int *turn); //AI 인공지능 가중치의 절댓값이 큰 곳에 흰돌을 놓도록 함
int Gard_22(int board[][20], int weight_board[][20], int Q); // 흑이 연속으로 두개 있을 때 그 양쪽에 아무것도 없을 경우 가중치를 -Gard22로 설정
void Blocked_Gard_22(int board[][20], int weight_board[][20], int find22, int Q); //흑이 연속으로 두개 있으나 그 한곳이 막혀있을 경우 가중치를 -Gard22로 설정 find22가 1일 때 가중치+=block22로 함
int Gard_33(int board[][20], int weight_board[][20], int Q);
int Attack_33(int board[][20], int weight_board[][20], int Q);
void Blocked_Gard_33(int board[][20], int weight_board[][20], int find33, int Q);
void Blocked_Attack_33(int board[][20], int weight_board[][20], int Q);
int Gard_44(int board[][20], int weight_board[][20], int Q);
void Gard43(int board[][20], int weight_board[][20]);
int Attack_44(int board[][20], int weight_board[][20], int Q);
int check_33(int x, int y); //33을 확인하는 함수 
int check_44(int x, int y); //44를 확인하는 함수
int win(int board[][20]); //승리조건을 확인하는 함수
void Attack_43(int board[][20], int weight_board[][20], int find33);
//----------------------------------------------------------------------------------------------------------------------------------

int main(void)
{
	int x = 18, y = 8, i, j, x33 = 0, x44 = 0, turn = 1, w; //x=18,y=8은 보드판의 중심위치임, turn = 1 플레이어 차례, turn = 2 컴퓨터 차례 
	int *p_turn; //turn을 가리키는 포인터
	int weight_board[20][20] = { 0 }; //보드의 가중치를 0으로 초기화
	char key; //키보드로 입력받은 값          
	p_turn = &turn;
	intro();
	draw_board(); //보드판 그림
	for (i = 0; i<20; i++) //보드값에 0을 대입
		for (j = 0; j<20; j++)
		{
			board[i][j] = 0;
		}
	gotoxy(x, y); //커서를 판의 가운데로 옮김
	for (;;) //커서를 움직이고 돌을 놓게 해줌
	{
		for (j = 0; j < 20; j += 18) //가장자리 값을 3으로 대입
		{
			for (i = 0; i < 20; i++)
			{
				if (board[i][j] == 2 && j == 18)
					board[i][j - 1] = 3;
				if (board[i][j] == 2 && j == 0)
					board[i][j / 2 + 1] = 3;
				if (board[i][j] == 0)
					board[i][j] = 3;
			}
		}
		for (i = 0; i < 20; i += 18) //가장자리에서 33 44를 허용하기 위함
		{
			for (j = 0; j < 20; j++)
			{
				if (board[i][j] == 2 && i == 18)
					board[i - 1][j] = 3;
				if (board[i][j] == 2 && i == 0)
					board[i + 1][j] = 3;
				if (board[i][j] == 0)
					board[i][j] = 3;
			}
		}
		x33 = 0, x44 = 0;
		j = 0;
		if (turn == 1)
		{
			gotoxy(x, y);
			key = getch(); //키를 입력받음
			clear_chat();
			gotoxy(x, y);

			switch (key)
			{
			case 43:
				gotoxy(0, 0);
				for (i = 0; i < 20; i++)
				{
					printf("                                                                            \n");
				}
				gotoxy(0, 0);
				draw_board();
				draw_stone(board);
				gotoxy(x, y);
				break;			
			case 61:
					gotoxy(0, 0);
					for (i = 0; i < 20; i++)
					{
						printf("                                                                            \n");
					}
					gotoxy(0, 0);
					draw_board();
					draw_stone(board);
					gotoxy(x, y);
					break;

			case 45:
				gotoxy(0, 0);
				for (i = 0; i < 20; i++)
				{
					printf("                                                                            \n");
				}
				gotoxy(0, 0);
				draw_narrow_board();
				draw_stone(board);
				gotoxy(x, y);
				break;
			case 72: //방향키
				if (y - 1 < 0)
					break;
				else
					y -= 1;
				break;
			case 80: //방향키
				if (y + 1 > 18)
					break;
				else
					y += 1;
				break;
			case 75: //방향키
				if (x - 2 < 0)
					break;
				else
					x -= 2;
				break;
			case 77: //방향키
				if (x + 2 > 36)
					break;
				else
					x += 2;
				break;
			case 's': //s를 누를 경우 돌을 대입
				for (i = -1; i < 2; i++) //j가 0일경우 33과 44가 있는지 확인
					if (board[y][x / 2 + i] == 3 || board[y + i][x / 2] == 3 || board[y + i][x / 2 + i] == 3 || board[y + i][x / 2 - i] == 3)
						j = 1;
				if (j == 0)
				{
					for (i = 0; i < 20; i++)
						for (j = 0; j < 20; j++)
						{
							if (board[i][j] == 3)
								board[i][j] = 0;
						}

					x33 = check_33(x, y);
					x44 = check_44(x, y); //33,44확인
				}

				gotoxy(50, 3);
				printf("%d %d", x33, x44);
				gotoxy(x, y);
				if ((x33 == 1) || (x44 == 1))
				{
					gotoxy(50, 3);
					printf("33 이거나 44 입니다.");
					gotoxy(x, y);
				}
				else if (board[y][x / 2] == 0 || board[y][x / 2] == 3)
				{
					printf("○");
					board[y][x / 2] = 2; //흑이 2임
					weight_board[y][x / 2] = 999;
					weight_m1(weight_board, board, x, y);
					w = win(board); //승리 체크
					if (w == 0)
						*p_turn = 2; //AI에게 차례를 넘김
					else
						*p_turn = 3;
				}
				else if ((board[y][x / 2] == 1) || (board[y][x / 2] == 2))
				{
					gotoxy(50, 3);
					printf("이미 돌이 놓여져있습니다!");
					gotoxy(x, y);
				}
				break;
			}

			gotoxy(50, 0);
			printf("x : %d y : %d          ", x, y);
			//printf("현재 보드 값 : %d x : %d y : %d   3x3 : %d  4x4 : %d       ", board[y][x / 2], x, y, x33, x44);
			gotoxy(x, y);

			win(board); //승리 체크
			gotoxy(x, y);
		}
		if (turn == 2)
			AI(board, weight_board, &turn);
	}

	return 0;
}

void intro()
{
	gotoxy(10, 2);
	printf("오목 ai 조작키: ← → ↑ ↓ 착수 : s 오목판 크기 조정 : +,-");
	gotoxy(10, 4);
	printf("***주의!! 착수한 후 오목판의 크기를 변경하면 오목돌이 깨집니다!!***");
	gotoxy(10, 6);
	printf("렌주룰이 적용되었습니다. (흑 33,44,6목 불가)");
	gotoxy(10, 9);
	printf("엔터키를 누르면 시작합니다.");
	getchar();
	gotoxy(10, 2);
	printf("                                                                   ");
	gotoxy(10, 4);
	printf("                                                                   ");
	gotoxy(10, 6);
	printf("                                                                   ");
	gotoxy(10, 9);
	printf("                                                                   ");
	gotoxy(0, 0);
}
void draw_board()
{
	int i, j;
	printf("┌");
	for (i = 0; i< 17; i++) printf("-┬");
	printf("-┐\n");

	for (i = 0; i < 17; i++) {
		printf("├");
		for (j = 0; j < 17; j++)
			printf("-┼");
		printf("-┤\n");
	}
	printf("└");
	for (i = 0; i < 17; i++)printf("-┴");
	printf("-┘");
}
void draw_narrow_board()
{
	int i, j;
	printf("┌");
	for (i = 0; i< 17; i++) printf("┬");
	printf("┐\n");

	for (i = 0; i < 17; i++) {
		printf("├");
		for (j = 0; j < 17; j++)
			printf("┼");
		printf("┤\n");
	}
	printf("└");
	for (i = 0; i < 17; i++)printf("┴");
	printf("┘");
}

void draw_stone(int board[][20])
{
	gotoxy(0, 0);
	int i, j;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[i][j / 2] == 1) { gotoxy(j, i);  printf("●"); }
			if (board[i][j / 2] == 2) { gotoxy(j, i);  printf("○"); }
		}
	}
}

void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void clear_chat()
{
	gotoxy(50, 3);
	printf("                            ");
}
int win(int board[][20])
{
	int i;
	int j;
	int w = 0;
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 2] == 1 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 1)
			{
				gotoxy(50, 10);
				printf("*** ●님이 승리하셨습니다 ***");
				w = 1;
			}
			else if (board[j][i - 2] == 2 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 2)
			{
				gotoxy(50, 10);
				printf("*** ○님이 승리하셨습니다 ***");
				w = -1;
			}
			else if (board[i - 2][j] == 1 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1) {
				gotoxy(50, 10);
				printf("*** ●님이 승리하셨습니다 ***");
				w = 1;
			}
			else if (board[i - 2][j] == 2 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 2)
			{
				gotoxy(50, 10);
				printf("*** ○님이 승리하셨습니다 ***");
				w = -1;
			}
		}
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 1)
			{
				gotoxy(50, 10);
				printf("*** ●님이 승리하셨습니다 ***");
				w = 1;
			}
			else if (board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 2)
			{
				gotoxy(50, 10);
				printf("*** ○님이 승리하셨습니다 ***");
				w = -1;
			}
			else if (board[j + 2][i - 2] == 1 && board[j + 1][i - 1] == 1 && board[j][i] == 1 && board[j - 1][i + 1] == 1 && board[j - 2][i + 2] == 1)
			{
				gotoxy(50, 10);
				printf("*** ●님이 승리하셨습니다 ***");
				w = 1;
			}
			else if (board[j + 2][i - 2] == 2 && board[j + 1][i - 1] == 2 && board[j][i] == 2 && board[j - 1][i + 1] == 2 && board[j - 2][i + 2] == 2)
			{
				gotoxy(50, 10);
				printf("*** ○님이 승리하셨습니다 ***");
				w = -1;
			}
		}
	}
	return w;
}
int Gard_44(int board[][20], int weight_board[][20], int Q)
{
	int i, j, k, gard44, zero = 0, two = 2, temp;
	(Q == 1) ? (gard44 = Gard44) : (gard44 = -Gard44);
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (i = 2; i < 17; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------------가로
			/*122220*/if (board[j][i - 2] == 1 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 2 && board[j][i + 3] == 0)
			{
				(weight_board[j][i + 3] -= gard44);
			}
			/*022221*/else if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 2 && board[j][i + 3] == 1)
			{
				(weight_board[j][i - 2] -= gard44);
			}
			//-------------------------------------세로
			/*122220*/else if (board[i - 2][j] == 1 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 2 && board[i + 3][j] == 0)
			{
				(weight_board[i + 3][j] -= gard44);
			}
			/*022221*/else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 2 && board[i + 3][j] == 1)
			{
				(weight_board[i - 2][j] -= gard44);
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				//-------------------------------가로
				if (board[j][i - 2] == 2 && board[j][i - 1] == zero && board[j][i] == 2 && board[j][i + 1] == two && board[j][i + 2] == 2)
				{
					if (k == 0)
						weight_board[j][i - 1] -= gard44;
					if (k == 1)
						weight_board[j][i + 1] -= gard44;
				}
				//------------------------------세로
				else if (board[i - 2][j] == 2 && board[i - 1][j] == zero && board[i][j] == 2 && board[i + 1][j] == two && board[i + 2][j] == 2)
				{
					if (k == 0)
						weight_board[i - 1][j] -= gard44;
					if (k == 1)
						weight_board[i + 1][j] -= gard44;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------가로
			if (board[j][i - 2] == 2 && board[j][i - 1] == 2 && board[j][i] == 0 && board[j][i + 1] == 2 && board[j][i + 2] == 2)
			{
				weight_board[j][i] -= gard44;
			}
			//------------------------------세로
			else if (board[i - 2][j] == 2 && board[i - 1][j] == 2 && board[i][j] == 0 && board[i + 1][j] == 2 && board[i + 2][j] == 2)
			{
				weight_board[i][j] -= gard44;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 17; i++)
	{
		for (j = 2; j < 17; j++)
		{
			//-------------------------------------대각
			/*122220*/if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 0)
			{
				(weight_board[j + 3][i + 3] -= gard44);
			}
			/*022221*/else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 1)
			{
				(weight_board[j - 2][i - 2] -= gard44);
			}
			//-------------------------------------반대각
			/*122220*/else if (board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 2 && board[j + 3][i - 3] == 0)
			{
				(weight_board[j + 3][i - 3] -= gard44);
			}
			/*022221*/else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 2 && board[j + 3][i - 3] == 1)
			{
				(weight_board[j - 2][i + 2] -= gard44);
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				//-------------------------------대각
				if (board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == zero && board[j][i] == 2 && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 2)
				{
					if (k == 0)
						weight_board[j - 1][i - 1] -= gard44;
					if (k == 1)
						weight_board[j + 1][i + 1] -= gard44;
				}
				//------------------------------반대각
				else if (board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == zero && board[j][i] == 2 && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 2)
				{
					if (k == 0)
						weight_board[j - 1][i + 1] -= gard44;
					if (k == 1)
						weight_board[j + 1][i - 1] -= gard44;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------대각
			if (board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 2 && board[j][i] == 0 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 2)
			{
				weight_board[j][i] -= gard44;
			}
			//------------------------------반대각
			else if (board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == 2 && board[j][i] == 0 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 2)
			{
				weight_board[j][i] -= gard44;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------------------------
}
int Gard_33(int board[][20], int weight_board[][20], int Q)
{
	int i, j, k, find33 = 0, gard33, zero = 0, two = 2, temp;
	(Q == 1) ? (gard33 = Gard33) : (gard33 = -Gard33);
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				(weight_board[j][i - 2] -= gard33);
				(weight_board[j][i + 2] -= gard33);
				find33++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 0)
			{
				(weight_board[i + 2][j] -= gard33);
				(weight_board[i - 2][j] -= gard33);
				find33++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 2 && board[j][i + 3] == 0)
				{
					(weight_board[j][i - 2] -= gard33);
					(weight_board[j][i + 3] -= gard33);
					find33++;
				}
				else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == zero && board[i + 1][j] == two && board[i + 2][j] == 2 && board[i + 3][j] == 0)
				{
					(weight_board[i + 3][j] -= gard33);
					(weight_board[i - 2][j] -= gard33);
					find33++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				(weight_board[j - 2][i - 2] -= gard33);
				(weight_board[j + 2][i + 2] -= gard33);
				find33++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				(weight_board[j + 2][i - 2] -= gard33);
				(weight_board[j - 2][i + 2] -= gard33);
				find33++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 0)
				{
					(weight_board[j - 2][i - 2] -= gard33);
					(weight_board[j + 3][i + 3] -= gard33);
					find33++;
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 2 && board[i + 3][j - 3] == 0)
				{
					(weight_board[j + 3][i - 3] -= gard33);
					(weight_board[j - 2][i + 2] -= gard33);
					find33++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	if (find33 > 0) return find33;
	else return find33;

}
int Attack_33(int board[][20], int weight_board[][20], int Q)
{
	int i, j, k, find33 = 0, gard33, zero = 0, two = 1, temp;
	(Q == 1) ? (gard33 = Attack33 + 5000) : (gard33 = -Attack33);
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 0)
			{
				(weight_board[j][i - 2] -= gard33);
				(weight_board[j][i + 2] -= gard33);
				find33++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
			{
				(weight_board[i + 2][j] -= gard33);
				(weight_board[i - 2][j] -= gard33);
				find33++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 1 && board[j][i + 3] == 0)
				{
					if (board[j][i] == 0)
						(weight_board[j][i] -= gard33);
					if (board[j][i + 1] == 0)
						(weight_board[j][i + 1] -= gard33);
					find33++;
				}
				else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == zero && board[i + 1][j] == two && board[i + 2][j] == 1 && board[i + 3][j] == 0)
				{
					if (board[i][j] == 0)
						(weight_board[i][j] -= gard33);
					if (board[i + 1][j])
						(weight_board[i + 1][j] -= gard33);
					find33++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0)
			{
				(weight_board[j - 2][i - 2] -= gard33);
				(weight_board[j + 2][i + 2] -= gard33);
				find33++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0)
			{
				(weight_board[j + 2][i - 2] -= gard33);
				(weight_board[j - 2][i + 2] -= gard33);
				find33++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == 0)
				{
					if (board[j][i] == 0)
						(weight_board[j][i] -= gard33);
					if (board[j + 1][i + 1] == 0)
						(weight_board[j + 1][i + 1] -= gard33);
					find33++;
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 1 && board[i + 3][j - 3] == 0)
				{
					if (board[j][i] == 0)
						(weight_board[j][i] -= gard33);
					if (board[j + 1][i - 1] == 0)
						(weight_board[j + 1][i - 1] -= gard33);
					find33++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------

}
void Blocked_Gard_33(int board[][20], int weight_board[][20], int find33, int Q)
{
	int i, j, k, gard33, block33, zero = 0, two = 2, temp;
	(Q == 1) ? (gard33 = Gard33) : (gard33 = -Gard33);
	(Q == 1) ? (block33 = +Block33) : (block33 = -Block33);
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------------------------------가로----------------------------------------------------------------
			if (board[j][i - 2] == 1 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				(weight_board[j][i + 2] -= gard33);
				if (find33 == 1)
				{
					(weight_board[j][i + 2] += block33);
				}
			}
			else if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 1)
			{
				(weight_board[j][i - 2] -= gard33);
				if (find33 == 1)
				{
					(weight_board[j][i - 2] += block33);
				}
			}
			//-----------------------------------------------------세로------------------------------------------------------------------
			else if (board[i - 2][j] == 1 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 0)
			{
				(weight_board[i + 2][j] -= gard33);
				if (find33 == 1)
				{
					(weight_board[i + 2][j] += block33);
				}
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 1)
			{
				(weight_board[i - 2][j] -= gard33);
				if (find33 == 1)
				{
					(weight_board[i - 2][j] += block33);
				}
			}
		}
	}
	//-------------------------------------------------------------가로 & 세로 2------------------------------------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 1 && board[j][i - 1] == 2 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 2 && board[j][i + 3] == 0)//120220
				{
					(weight_board[j][i + 3] -= gard33);
					if (find33 == 1)
					{
						(weight_board[j][i + 3] += block33);
					}
				}
				else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == zero && board[i + 1][j] == two && board[i + 2][j] == 2 && board[i + 3][j] == 1)//020221
				{
					(weight_board[i - 2][j] -= gard33);
					if (find33 == 1)
					{
						(weight_board[i - 2][j] += block33);
					}
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				(weight_board[j + 2][i + 2] -= gard33);
				if (find33 == 1)
				{
					(weight_board[j + 2][i + 2] += block33);
				}
			}
			else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 1)
			{
				(weight_board[j - 2][i - 2] -= gard33);
				if (find33 == 1)
				{
					(weight_board[j - 2][i - 2] += block33);
				}
			}
			//-----
			else if (board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				(weight_board[j + 2][i - 2] -= gard33);
				if (find33 == 1)
				{
					(weight_board[j + 2][i - 2] += block33);
				}
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 1)
			{
				(weight_board[j - 2][i + 2] -= gard33);
				if (find33 == 1)
				{
					(weight_board[j - 2][i + 2] += block33);
				}
			}
		}
	}
	//--------------------------------------------------------------대각 & 반대각 2--------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 2 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 0)
				{
					(weight_board[j + 3][i + 3] -= gard33);
					if (find33 == 1)
					{
						(weight_board[j + 3][i + 3] += block33);
					}
				}
				else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 1)
				{
					(weight_board[j - 2][i - 2] -= gard33);
					if (find33 == 1)
					{
						(weight_board[j - 2][i - 2] += block33);
					}
				}
				//----------------------
				else if (board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 2 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 2 && board[i + 3][j - 3] == 0)
				{
					(weight_board[j + 3][i - 3] -= gard33);
					if (find33 == 1)
					{
						(weight_board[j + 3][i - 3] += block33);
					}
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 2 && board[i + 3][j - 3] == 1)
				{
					(weight_board[j - 2][i + 2] -= gard33);
					if (find33 == 1)
					{
						(weight_board[j - 2][i + 2] += block33);
					}
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
}
void Blocked_Attack_33(int board[][20], int weight_board[][20], int Q)
{
	int i, j, k, gard33, zero = 0, two = 1, temp;
	(Q == 1) ? (gard33 = ABlock33) : (gard33 = -ABlock33);
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------------------------------가로----------------------------------------------------------------
			if (board[j][i - 2] == 2 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 0)
			{
				(weight_board[j][i + 2] -= gard33);
			}
			else if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 2)
			{
				(weight_board[j][i - 2] -= gard33);
			}
			//-----------------------------------------------------세로------------------------------------------------------------------
			else if (board[i - 2][j] == 2 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
			{
				(weight_board[i + 2][j] -= gard33);
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 2)
			{
				(weight_board[i - 2][j] -= gard33);
			}
		}
	}
	//-------------------------------------------------------------가로 & 세로 2------------------------------------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 2 && board[j][i - 1] == 1 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 1 && board[j][i + 3] == 0)//120220
				{
					if (board[j][i] == 0)
						(weight_board[j][i] -= gard33);
					if (board[j][i + 1] == 0)
						weight_board[j][i + 1] -= gard33;
				}
				else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == zero && board[i + 1][j] == two && board[i + 2][j] == 1 && board[i + 3][j] == 2)//020221
				{
					if (board[i][j] == 0)
						weight_board[i][j] -= gard33;
					if (board[i + 1][j] == 0)
						weight_board[i + 1][j] -= gard33;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0)
			{
				(weight_board[j + 2][i + 2] -= gard33);
			}
			else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 2)
			{
				(weight_board[j - 2][i - 2] -= gard33);
			}
			//-----
			else if (board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0)
			{
				(weight_board[j + 2][i - 2] -= gard33);
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 2)
			{
				(weight_board[j - 2][i + 2] -= gard33);
			}
		}
	}
	//--------------------------------------------------------------대각 & 반대각 2--------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 1 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == 0)
				{
					if (board[j][i] == 0)
						weight_board[j][i] -= gard33;
					if (board[j + 1][i + 1] == 0)
						weight_board[j + 1][i + 1] -= gard33;
				}
				else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == 2)
				{
					if (board[j][i] == 0)
						weight_board[j][i] -= gard33;
					if (board[j + 1][i + 1] == 0)
						weight_board[j + 1][i + 1] -= gard33;
				}
				//----------------------
				else if (board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == 1 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 1 && board[i + 3][j - 3] == 0)
				{
					if (board[j][i] == 0)
						weight_board[j][i] -= gard33;
					if (board[j + 1][i - 1] == 0)
						weight_board[j + 1][i - 1] -= gard33;
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 1 && board[i + 3][j - 3] == 2)
				{
					if (board[j][i] == 0)
						weight_board[j][i] -= gard33;
					if (board[j + 1][i - 1] == 0)
						weight_board[j + 1][i - 1] -= gard33;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
}
int Attack_44(int board[][20], int weight_board[][20], int Q)
{
	int i, j, k, attack44, zero = 0, two = 1, temp, twice = 2;
	(Q == 1) ? (attack44 = Attack44) : (attack44 = -Attack44);
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				//-------------------------------------가로
				/*122220*/if (board[j][i - 2] == twice && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 1 && board[j][i + 3] == 0)
				{
					(weight_board[j][i + 3] -= attack44);
				}
				/*022221*/else if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 1 && board[j][i + 3] == twice)
				{
					(weight_board[j][i - 2] -= attack44);
				}
				//-------------------------------------세로
				/*122220*/else if (board[i - 2][j] == twice && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 0)
				{
					(weight_board[i + 3][j] -= attack44);
				}
				/*022221*/else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == twice)
				{
					(weight_board[i - 2][j] -= attack44);
				}
			}
		}
		twice = 0;
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				//-------------------------------가로
				if (board[j][i - 2] == 1 && board[j][i - 1] == zero && board[j][i] == 1 && board[j][i + 1] == two && board[j][i + 2] == 1)
				{
					if (board[j][i - 1] == 0)
						weight_board[j][i - 1] -= attack44;
					if (board[j][i + 1] = 0)
						weight_board[j][i + 1] -= attack44;
				}
				//------------------------------세로
				else if (board[i - 2][j] == 1 && board[i - 1][j] == zero && board[i][j] == 1 && board[i + 1][j] == two && board[i + 2][j] == 1)
				{
					if (board[i - 1][j] == 0)
						weight_board[i - 1][j] -= attack44;
					if (board[i + 1][j] == 0)
						weight_board[i + 1][j] -= attack44;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------가로
			if (board[j][i - 2] == 1 && board[j][i - 1] == 1 && board[j][i] == 0 && board[j][i + 1] == 1 && board[j][i + 2] == 1)
			{
				weight_board[j][i] -= attack44;
			}
			//------------------------------세로
			else if (board[i - 2][j] == 1 && board[i - 1][j] == 1 && board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 1)
			{
				weight_board[i][j] -= attack44;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	twice = 2;
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				//-------------------------------------대각
				/*122220*/if (board[j - 2][i - 2] == twice && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == 0)
				{
					(weight_board[j + 3][i + 3] -= attack44);
				}
				/*022221*/else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == twice)
				{
					(weight_board[j - 2][i - 2] -= attack44);
				}
				//-------------------------------------반대각
				/*122220*/else if (board[j - 2][i + 2] == twice && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 1 && board[j + 3][i - 3] == 0)
				{
					(weight_board[j + 3][i - 3] -= attack44);
				}
				/*022221*/else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 1 && board[j + 3][i - 3] == twice)
				{
					(weight_board[j - 2][i + 2] -= attack44);
				}
			}
		}
		twice = 0;
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				//-------------------------------대각
				if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == zero && board[j][i] == 1 && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 1)
				{
					if (board[j - 1][i - 1] == 0)
						weight_board[j - 1][i - 1] -= attack44;
					if (board[j + 1][i + 1] == 0)
						weight_board[j + 1][i + 1] -= attack44;
				}
				//------------------------------반대각
				else if (board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == zero && board[j][i] == 1 && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 1)
				{
					if (board[j - 1][i + 1] == 0)
						weight_board[j - 1][i + 1] -= attack44;
					if (board[j - 1][i - 1] == 0)
						weight_board[j + 1][i - 1] -= attack44;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------대각
			if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 1 && board[j][i] == 0 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 1)
			{
				weight_board[j][i] -= attack44;
			}
			//------------------------------반대각
			else if (board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 1 && board[j][i] == 0 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 1)
			{
				weight_board[j][i] -= attack44;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------------------------
}
int Gard_22(int board[][20], int weight_board[][20], int Q)
{
	int i, j, find22 = 0, gard22;
	(Q == 1) ? (gard22 = Gard22) : (gard22 = -Gard22);

	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 1] == 0 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				(weight_board[j][i - 1] -= gard22);
				(weight_board[j][i + 2] -= gard22);
				find22++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 0)
			{
				(weight_board[i + 1][j] -= gard22);
				(weight_board[i - 2][j] -= gard22);
				find22++;
			}
		}
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 1][i - 1] == 0 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				(weight_board[j - 1][i - 1] -= gard22);
				(weight_board[j + 2][i + 2] -= gard22);
				find22++;
			}
			else if (board[j - 1][i + 1] == 0 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				(weight_board[j - 1][i + 1] -= gard22);
				(weight_board[j + 2][i - 2] -= gard22);
				find22++;
			}
		}
	}
	if (find22 > 0) return 1;
	return 0;
}
void Blocked_Gard_22(int board[][20], int weight_board[][20], int find22, int Q)
{
	int i, gard22, block22;
	int j;
	(Q == 1) ? (gard22 = +Gard22) : (gard22 = -Gard22);
	(Q == 1) ? (block22 = +Block22) : (block22 = -Block22);
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 1] == 1 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				weight_board[j][i + 2] -= gard22;
				if (find22 == 1)
				{
					weight_board[j][i + 2] += block22;
				}
			}
			else if (board[j][i - 1] == 0 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 1)
			{
				weight_board[j][i - 1] -= gard22;
				if (find22 == 1)
				{
					weight_board[j][i - 1] += block22;
				}
			}
			else if (board[i - 2][j] == 1 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 0)
			{
				weight_board[i + 1][j] -= gard22;
				if (find22 == 1)
				{
					weight_board[i + 1][j] += block22;
				}
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 1)
			{
				weight_board[i - 2][j] -= gard22;
				if (find22 == 1)
				{
					weight_board[i - 2][j] += block22;
				}
			}
		}
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 1][i - 1] == 1 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				weight_board[j + 2][i + 2] -= gard22;
				if (find22 == 1)
				{
					weight_board[j + 2][i + 2] += block22;
				}
			}
			else if (board[j - 1][i - 1] == 0 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 1)
			{
				weight_board[j - 1][i - 1] -= gard22;
				if (find22 == 1)
				{
					weight_board[j - 1][i - 1] += block22;
				}
			}
			else if (board[j - 1][i + 1] == 1 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				weight_board[j + 2][i - 2] -= gard22;
				if (find22 == 1)
				{
					weight_board[j + 2][i - 2] += block22;
				}
			}
			else if (board[j - 1][i + 1] == 0 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 1)
			{
				weight_board[j - 1][i + 1] -= gard22;
				if (find22 == 1)
				{
					weight_board[j - 1][i + 1] += block22;
				}
			}
		}
	}
}
int check_33(int x, int y)
{
	int count = 0;
	// 가로체크. 
	if (
		(board[y][x / 2 - 3] == 0 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 0) || //011?0
		(board[y][x / 2 - 2] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == 0) || //01?10
		(board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 3] == 0) || //0?110
		(board[y][x / 2 - 4] == 0 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 0) ||//0110?0
		(board[y][x / 2 - 2] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 0 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 3] == 0) ||//01?010
		(board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == 0 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 4] == 0) ||//0?1010
		(board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 0 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 4] == 0) ||//0?0110
		(board[y][x / 2 - 3] == 0 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == 0) ||//010?10
		(board[y][x / 2 - 3] == 0 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 0) //0101?0
		)
		count++;
	// 세로체크. 
	if (
		(board[y - 3][x / 2] == 0 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 0) ||
		(board[y - 2][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == 0) ||
		(board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == 2 && board[y + 3][x / 2] == 0) ||
		(board[y - 4][x / 2] == 0 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 0) ||//0110?0
		(board[y - 2][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 0 && board[y + 2][x / 2] == 2 && board[y + 3][x / 2] == 0) ||//01?010
		(board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == 0 && board[y + 3][x / 2] == 2 && board[y + 4][x / 2] == 0) ||//0?1010
		(board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 0 && board[y + 2][x / 2] == 2 && board[y + 3][x / 2] == 2 && board[y + 4][x / 2] == 0) ||//0?0110
		(board[y - 3][x / 2] == 0 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == 0) ||//010?10
		(board[y - 4][x / 2] == 0 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 0) //0101?0
		)
		count++;
	// 대각선체크. 
	if (
		(board[y - 3][x / 2 - 3] == 0 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 0) ||
		(board[y - 2][x / 2 - 2] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == 0) ||
		(board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == 2 && board[y + 3][x / 2 + 3] == 0) ||
		(board[y - 4][x / 2 - 4] == 0 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 0) ||//0110?0
		(board[y - 2][x / 2 - 2] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 0 && board[y + 2][x / 2 + 2] == 2 && board[y + 3][x / 2 + 3] == 0) ||//01?010
		(board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == 0 && board[y + 3][x / 2 + 3] == 2 && board[y + 4][x / 2 + 4] == 0) ||//0?1010
		(board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 0 && board[y + 2][x / 2 + 2] == 2 && board[y + 3][x / 2 + 3] == 2 && board[y + 4][x / 2 + 4] == 0) ||//0?0110
		(board[y - 3][x / 2 - 3] == 0 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == 0) ||//010?10
		(board[y - 4][x / 2 - 4] == 0 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 0) //010?10
		)
		count++;
	// 반대 대각선체크. y부호 그대로
	if (
		(board[y - 3][x / 2 + 3] == 0 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 0) ||
		(board[y - 2][x / 2 + 2] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == 0) ||
		(board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == 2 && board[y + 3][x / 2 - 3] == 0) ||
		(board[y - 4][x / 2 + 4] == 0 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 0) ||//0110?0
		(board[y - 2][x / 2 + 2] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 0 && board[y + 2][x / 2 - 2] == 2 && board[y + 3][x / 2 - 3] == 0) ||//01?010
		(board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == 0 && board[y + 3][x / 2 - 3] == 2 && board[y + 4][x / 2 - 4] == 0) ||//0?1010
		(board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == 0 && board[y + 2][x / 2 - 2] == 2 && board[y + 3][x / 2 - 3] == 2 && board[y + 4][x / 2 - 4] == 0) ||//0?0110
		(board[y - 3][x / 2 + 3] == 0 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == 0) ||//010?10
		(board[y - 4][x / 2 + 4] == 0 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 0) //010?10
		)
		count++;
	if (count > 1) return 1;
	else return 0;
}
int check_44(int x, int y)
{
	int count = 0;
	int Bcount = 0, zero = 0, one = 1, k;
	// 가로체크. 
	if (
		(board[y][x / 2 - 4] == 0 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 0) || //0222?0
		(board[y][x / 2 + 4] == 0 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == 0) || //0?2220
		(board[y][x / 2 - 3] == 0 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == 0) || //022?20
		(board[y][x / 2 + 3] == 0 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 - 2] == 0) || //02?220
		(board[y][x / 2 - 5] == 0 && board[y][x / 2 - 4] == 2 && board[y][x / 2 - 3] == 0 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 0) ||
		(board[y][x / 2 + 5] == 0 && board[y][x / 2 + 4] == 2 && board[y][x / 2 + 3] == 0 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == 0) ||
		(board[y][x / 2 - 5] == 0 && board[y][x / 2 - 4] == 2 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 0) ||
		(board[y][x / 2 + 5] == 0 && board[y][x / 2 + 4] == 2 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == 0) ||
		(board[y][x / 2 - 5] == 0 && board[y][x / 2 - 4] == 2 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 0) ||
		(board[y][x / 2 + 5] == 0 && board[y][x / 2 + 4] == 2 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 0 && board[y][x / 2 - 1] == 0) ||
		(board[y][x / 2 - 4] == 0 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == 0) ||
		(board[y][x / 2 + 4] == 0 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 - 2] == 0) ||
		(board[y][x / 2 - 4] == 0 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == 0) ||
		(board[y][x / 2 + 4] == 0 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 - 2] == 0)
		)
		count++;
	// 세로체크. 
	if (
		(board[y - 4][x / 2] == 0 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 0) ||
		(board[y + 4][x / 2] == 0 && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == 0) ||
		(board[y - 3][x / 2] == 0 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == 0) ||
		(board[y + 3][x / 2] == 0 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y - 2][x / 2] == 0) ||
		(board[y - 5][x / 2] == 0 && board[y - 4][x / 2] == 2 && board[y - 3][x / 2] == 0 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 0) ||
		(board[y + 5][x / 2] == 0 && board[y + 4][x / 2] == 2 && board[y + 3][x / 2] == 0 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == 0) ||
		(board[y - 5][x / 2] == 0 && board[y - 4][x / 2] == 2 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 0) ||
		(board[y + 5][x / 2] == 0 && board[y + 4][x / 2] == 2 && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == 0) ||
		(board[y - 5][x / 2] == 0 && board[y - 4][x / 2] == 2 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 0) ||
		(board[y + 5][x / 2] == 0 && board[y + 4][x / 2] == 2 && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 0 && board[y - 1][x / 2] == 0) ||
		(board[y - 4][x / 2] == 0 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == 0) ||
		(board[y + 4][x / 2] == 0 && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y - 2][x / 2] == 0) ||
		(board[y - 4][x / 2] == 0 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == 0) ||
		(board[y + 4][x / 2] == 0 && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y - 2][x / 2] == 0)
		)
		count++;
	// 대각선체크. 
	if (
		(board[y - 4][x / 2 - 4] == 0 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 0) ||
		(board[y + 4][x / 2 + 4] == 0 && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == 0) ||
		(board[y - 3][x / 2 - 3] == 0 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == 0) ||
		(board[y + 3][x / 2 + 3] == 0 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y - 2][x / 2 - 2] == 0) ||
		(board[y - 5][x / 2 - 5] == 0 && board[y - 4][x / 2 - 4] == 2 && board[y - 3][x / 2 - 3] == 0 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 0) ||
		(board[y + 5][x / 2 + 5] == 0 && board[y + 4][x / 2 + 4] == 2 && board[y + 3][x / 2 + 3] == 0 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == 0) ||
		(board[y - 5][x / 2 - 5] == 0 && board[y - 4][x / 2 - 4] == 2 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 0) ||
		(board[y + 5][x / 2 + 5] == 0 && board[y + 4][x / 2 + 4] == 2 && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == 0) ||
		(board[y - 5][x / 2 - 5] == 0 && board[y - 4][x / 2 - 4] == 2 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 0) ||
		(board[y + 5][x / 2 + 5] == 0 && board[y + 4][x / 2 + 4] == 2 && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 0 && board[y - 1][x / 2 - 1] == 0) ||
		(board[y - 4][x / 2 - 4] == 0 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == 0) ||
		(board[y + 4][x / 2 + 4] == 0 && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y - 2][x / 2 - 2] == 0) ||
		(board[y - 4][x / 2 - 4] == 0 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == 0) ||
		(board[y + 4][x / 2 + 4] == 0 && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y - 2][x / 2 - 2] == 0)
		)
		count++;
	// 반대 대각선체크. 
	if (
		(board[y - 4][x / 2 + 4] == 0 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 0) ||
		(board[y + 4][x / 2 - 4] == 0 && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == 0) ||
		(board[y - 3][x / 2 + 3] == 0 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == 0) ||
		(board[y + 3][x / 2 - 3] == 0 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y - 2][x / 2 + 2] == 0) ||
		(board[y - 5][x / 2 + 5] == 0 && board[y - 4][x / 2 + 4] == 2 && board[y - 3][x / 2 + 3] == 0 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 0) ||
		(board[y + 5][x / 2 - 5] == 0 && board[y + 4][x / 2 - 4] == 2 && board[y + 3][x / 2 - 3] == 0 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == 0) ||
		(board[y - 5][x / 2 + 5] == 0 && board[y - 4][x / 2 + 4] == 2 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 0) ||
		(board[y + 5][x / 2 - 5] == 0 && board[y + 4][x / 2 - 4] == 2 && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == 0) ||
		(board[y - 5][x / 2 - 5] == 0 && board[y - 4][x / 2 + 4] == 2 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == 0) ||
		(board[y + 5][x / 2 + 5] == 0 && board[y + 4][x / 2 - 4] == 2 && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 0 && board[y - 1][x / 2 + 1] == 0) ||
		(board[y - 4][x / 2 + 4] == 0 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == 0) ||
		(board[y + 4][x / 2 - 4] == 0 && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y - 2][x / 2 + 2] == 0) ||
		(board[y - 4][x / 2 + 4] == 0 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == 0) ||
		(board[y + 4][x / 2 - 4] == 0 && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y - 2][x / 2 + 2] == 0)
		)
		count++;
	///////////////////
	// 가로체크. 
	for (k = 0; k < 2; k++)
	{
		if (
			(board[y][x / 2 - 4] == zero && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == one) || //0222?0
			(board[y][x / 2 + 4] == zero && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == one) || //0?2220
			(board[y][x / 2 - 3] == zero && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == one) || //022?20
			(board[y][x / 2 + 3] == zero && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 - 2] == one) || //02?220
			(board[y][x / 2 - 5] == zero && board[y][x / 2 - 4] == 2 && board[y][x / 2 - 3] == 0 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == one) ||
			(board[y][x / 2 + 5] == zero && board[y][x / 2 + 4] == 2 && board[y][x / 2 + 3] == 0 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == one) ||
			(board[y][x / 2 - 5] == zero && board[y][x / 2 - 4] == 2 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == one) ||
			(board[y][x / 2 + 5] == zero && board[y][x / 2 + 4] == 2 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == one) ||
			(board[y][x / 2 - 5] == zero && board[y][x / 2 - 4] == 2 && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == one) ||
			(board[y][x / 2 + 5] == zero && board[y][x / 2 + 4] == 2 && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 0 && board[y][x / 2 - 1] == one) ||
			(board[y][x / 2 - 4] == zero && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 2 && board[y][x / 2 - 1] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == one) ||
			(board[y][x / 2 + 4] == zero && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 2 && board[y][x / 2 + 1] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 - 2] == one) ||
			(board[y][x / 2 - 4] == zero && board[y][x / 2 - 3] == 2 && board[y][x / 2 - 2] == 0 && board[y][x / 2 - 1] == 2 && board[y][x / 2 + 1] == 2 && board[y][x / 2 + 2] == one) ||
			(board[y][x / 2 + 4] == zero && board[y][x / 2 + 3] == 2 && board[y][x / 2 + 2] == 0 && board[y][x / 2 + 1] == 2 && board[y][x / 2 - 1] == 2 && board[y][x / 2 - 2] == one)
			)
			Bcount++;
		// 세로체크. 
		if (
			(board[y - 4][x / 2] == zero && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == one) ||
			(board[y + 4][x / 2] == zero && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == one) ||
			(board[y - 3][x / 2] == zero && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == one) ||
			(board[y + 3][x / 2] == zero && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y - 2][x / 2] == one) ||
			(board[y - 5][x / 2] == zero && board[y - 4][x / 2] == 2 && board[y - 3][x / 2] == 0 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == one) ||
			(board[y + 5][x / 2] == zero && board[y + 4][x / 2] == 2 && board[y + 3][x / 2] == 0 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == one) ||
			(board[y - 5][x / 2] == zero && board[y - 4][x / 2] == 2 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == one) ||
			(board[y + 5][x / 2] == zero && board[y + 4][x / 2] == 2 && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == one) ||
			(board[y - 5][x / 2] == zero && board[y - 4][x / 2] == 2 && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == one) ||
			(board[y + 5][x / 2] == zero && board[y + 4][x / 2] == 2 && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 0 && board[y - 1][x / 2] == one) ||
			(board[y - 4][x / 2] == zero && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 2 && board[y - 1][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == one) ||
			(board[y + 4][x / 2] == zero && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 2 && board[y + 1][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y - 2][x / 2] == one) ||
			(board[y - 4][x / 2] == zero && board[y - 3][x / 2] == 2 && board[y - 2][x / 2] == 0 && board[y - 1][x / 2] == 2 && board[y + 1][x / 2] == 2 && board[y + 2][x / 2] == one) ||
			(board[y + 4][x / 2] == zero && board[y + 3][x / 2] == 2 && board[y + 2][x / 2] == 0 && board[y + 1][x / 2] == 2 && board[y - 1][x / 2] == 2 && board[y - 2][x / 2] == one)
			)
			Bcount++;
		// 대각선체크. 
		if (
			(board[y - 4][x / 2 - 4] == zero && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == one) ||
			(board[y + 4][x / 2 + 4] == zero && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == one) ||
			(board[y - 3][x / 2 - 3] == zero && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == one) ||
			(board[y + 3][x / 2 + 3] == zero && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y - 2][x / 2 - 2] == one) ||
			(board[y - 5][x / 2 - 5] == zero && board[y - 4][x / 2 - 4] == 2 && board[y - 3][x / 2 - 3] == 0 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == one) ||
			(board[y + 5][x / 2 + 5] == zero && board[y + 4][x / 2 + 4] == 2 && board[y + 3][x / 2 + 3] == 0 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == one) ||
			(board[y - 5][x / 2 - 5] == zero && board[y - 4][x / 2 - 4] == 2 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == one) ||
			(board[y + 5][x / 2 + 5] == zero && board[y + 4][x / 2 + 4] == 2 && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == one) ||
			(board[y - 5][x / 2 - 5] == zero && board[y - 4][x / 2 - 4] == 2 && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == one) ||
			(board[y + 5][x / 2 + 5] == zero && board[y + 4][x / 2 + 4] == 2 && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 0 && board[y - 1][x / 2 - 1] == one) ||
			(board[y - 4][x / 2 - 4] == zero && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 2 && board[y - 1][x / 2 - 1] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == one) ||
			(board[y + 4][x / 2 + 4] == zero && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 2 && board[y + 1][x / 2 + 1] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y - 2][x / 2 - 2] == one) ||
			(board[y - 4][x / 2 - 4] == zero && board[y - 3][x / 2 - 3] == 2 && board[y - 2][x / 2 - 2] == 0 && board[y - 1][x / 2 - 1] == 2 && board[y + 1][x / 2 + 1] == 2 && board[y + 2][x / 2 + 2] == one) ||
			(board[y + 4][x / 2 + 4] == zero && board[y + 3][x / 2 + 3] == 2 && board[y + 2][x / 2 + 2] == 0 && board[y + 1][x / 2 + 1] == 2 && board[y - 1][x / 2 - 1] == 2 && board[y - 2][x / 2 - 2] == one)
			)
			Bcount++;
		// 반대 대각선체크. 
		if (
			(board[y - 4][x / 2 + 4] == zero && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == one) ||
			(board[y + 4][x / 2 - 4] == zero && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == one) ||
			(board[y - 3][x / 2 + 3] == zero && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == one) ||
			(board[y + 3][x / 2 - 3] == zero && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y - 2][x / 2 + 2] == one) ||
			(board[y - 5][x / 2 + 5] == zero && board[y - 4][x / 2 + 4] == 2 && board[y - 3][x / 2 + 3] == 0 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == one) ||
			(board[y + 5][x / 2 - 5] == zero && board[y + 4][x / 2 - 4] == 2 && board[y + 3][x / 2 - 3] == 0 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == one) ||
			(board[y - 5][x / 2 + 5] == zero && board[y - 4][x / 2 + 4] == 2 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == one) ||
			(board[y + 5][x / 2 - 5] == zero && board[y + 4][x / 2 - 4] == 2 && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == one) ||
			(board[y - 5][x / 2 - 5] == zero && board[y - 4][x / 2 + 4] == 2 && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == one) ||
			(board[y + 5][x / 2 + 5] == zero && board[y + 4][x / 2 - 4] == 2 && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 0 && board[y - 1][x / 2 + 1] == one) ||
			(board[y - 4][x / 2 + 4] == zero && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 2 && board[y - 1][x / 2 + 1] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == one) ||
			(board[y + 4][x / 2 - 4] == zero && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 2 && board[y + 1][x / 2 - 1] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y - 2][x / 2 + 2] == one) ||
			(board[y - 4][x / 2 + 4] == zero && board[y - 3][x / 2 + 3] == 2 && board[y - 2][x / 2 + 2] == 0 && board[y - 1][x / 2 + 1] == 2 && board[y + 1][x / 2 - 1] == 2 && board[y + 2][x / 2 - 2] == one) ||
			(board[y + 4][x / 2 - 4] == zero && board[y + 3][x / 2 - 3] == 2 && board[y + 2][x / 2 - 2] == 0 && board[y + 1][x / 2 - 1] == 2 && board[y - 1][x / 2 + 1] == 2 && board[y - 2][x / 2 + 2] == one)
			)
			Bcount++;
		zero = 1;
		one = 0;
	}
	if (Bcount > 1) return 1;
	if (count > 1) return 1;
	else return 0;
}
void Gard43(int board[][20], int weight_board[][20])
{
	int i, j, k, zero = 0, two = 2, temp, zero2 = 0, one = 1, x = 0, y = 0, x2 = 0, y2 = 0;
	int find33[20][20], Bfind33[20][20];
	int find22[20][20], Bfind22[20][20];
	for (i = 0; i<20; i++)
		for (j = 0; j < 20; j++)
		{
			find33[i][j] = 0;
			Bfind33[i][j] = 0;
			find22[i][j] = 0;
			Bfind22[i][j] = 0;
		}
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				find33[j][i - 2]++;
				find33[j][i + 2]++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 0)
			{
				find33[i - 2][j]++;
				find33[i + 2][j]++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 2 && board[j][i + 3] == 0)
				{
					find33[j][i - 2]++;
					find33[j][i + 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j][i + 1] == 0) find33[j][i + 1]++;
				}
				else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == zero && board[i + 1][j] == two && board[i + 2][j] == 2 && board[i + 3][j] == 0)
				{
					find33[i - 2][j]++;
					find33[i + 3][j]++;
					if (board[i][j] == 0) find33[i][j]++;
					if (board[i + 1][j] == 0) find33[i + 1][j]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				find33[j - 2][i - 2]++;
				find33[j + 2][i + 2]++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				find33[j - 2][i + 2]++;
				find33[j + 2][i - 2]++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 0)
				{
					find33[j - 2][i - 2]++;
					find33[j + 3][i + 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i + 1] == 0) find33[j + 1][i + 1]++;
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 2 && board[i + 3][j - 3] == 0)
				{
					find33[j - 2][i + 2]++;
					find33[j + 3][i - 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i - 1] == 0) find33[j + 1][i - 1]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------------------------------가로----------------------------------------------------------------
			if (board[j][i - 2] == 1 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				Bfind33[j][i + 2]++;
				Bfind33[j][i + 3]++;
			}
			else if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 2 && board[j][i + 1] == 2 && board[j][i + 2] == 1)
			{
				Bfind33[j][i - 2]++;
				Bfind33[j][i - 3]++;
			}
			//-----------------------------------------------------세로------------------------------------------------------------------
			else if (board[i - 2][j] == 1 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 0)
			{
				Bfind33[i + 2][j]++;
				Bfind33[i + 3][j]++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 1)
			{
				Bfind33[i - 2][j]++;
				Bfind33[i - 3][j]++;
			}
		}
	}
	//-------------------------------------------------------------가로 & 세로 2------------------------------------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 1 && board[j][i - 1] == 2 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 2 && board[j][i + 3] == 0)//120220
				{
					Bfind33[j][i + 3]++;
					if (board[j][i] == 0) Bfind33[j][i]++;
					if (board[j][i + 1] == 0) Bfind33[j][i + 1]++;
				}
				else if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 2 && board[j][i + 3] == 1)//020221
				{
					Bfind33[j][i - 2]++;
					if (board[j][i] == 0) Bfind33[j][i]++;
					if (board[j][i + 1] == 0) Bfind33[j][i + 1]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				Bfind33[j + 2][i + 2]++;
				Bfind33[j + 3][i + 3]++;
			}
			else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 2 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 1)
			{
				Bfind33[j - 2][i - 2]++;
				Bfind33[j - 3][i - 3]++;
			}
			//-----
			else if (board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				Bfind33[j + 2][i - 2]++;
				Bfind33[j + 3][i - 3]++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 2 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 1)
			{
				Bfind33[j - 2][i + 2]++;
				Bfind33[j - 3][i + 3]++;
			}
		}
	}
	//--------------------------------------------------------------대각 & 반대각 2--------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 2 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 0)
				{
					Bfind33[j + 3][i + 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i + 1] == 0) find33[j + 1][i + 1]++;
				}
				else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 2 && board[j + 3][i + 3] == 1)
				{
					Bfind33[j - 2][i - 2]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i + 1] == 0) find33[j + 1][i + 1]++;
				}
				//----------------------
				else if (board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 2 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 2 && board[i + 3][j - 3] == 0)
				{
					Bfind33[j + 3][i - 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i - 1] == 0) find33[j + 1][i - 1]++;
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 2 && board[i + 3][j - 3] == 1)
				{
					Bfind33[j - 2][i + 2]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i - 1] == 0) find33[j + 1][i - 1]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	// 000220 022000
	zero = 0, two = 2;
	for (k = 0; k < 2; k++)
	{
		for (i = 3; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 3] == 0 && board[j][i - 2] == zero && board[j][i - 1] == zero && board[j][i] == two && board[j][i + 1] == two && board[j][i + 2] == 0)
				{
					if (k == 0)
					{
						find22[j][i - 1]++;
						find22[j][i - 2]++;
					}
					if (k == 1)
					{
						find22[j][i]++;
						find22[j][i + 1]++;
					}
				}
				else if (board[i - 3][j] == 0 && board[i - 2][j] == zero && board[i - 1][j] == zero && board[i][j] == two && board[i + 1][j] == two && board[i + 2][j] == 0)
				{
					if (k == 0)
					{
						find22[i - 2][j]++;
						find22[i - 1][j]++;
					}
					if (k == 1)
					{
						find22[i][j]++;
						find22[i + 1][j]++;
					}
				}
			}
		}
		for (i = 3; i < 18; i++)
		{
			for (j = 3; j < 18; j++)
			{
				if (board[j - 3][i - 3] == 0 && board[j - 2][i - 2] == zero && board[j - 1][i - 1] == zero && board[j][i] == two && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 0)
				{
					if (k == 0)
					{
						find22[j - 1][i - 1]++;
						find22[j - 2][i - 2]++;
					}
					if (k == 1)
					{
						find22[j][i]++;
						find22[j + 1][i + 1]++;
					}
				}
				else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == zero && board[j - 1][i + 1] == zero && board[j][i] == two && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 0)
				{
					if (k == 0)
					{
						find22[j - 1][i + 1]++;
						find22[j - 2][i + 2]++;
					}
					if (k == 1)
					{
						find22[j][i]++;
						find22[j + 1][i - 1]++;
					}
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//-------------------------------------02020----------------------------------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 0 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				find22[j][i]++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 0 && board[i + 1][j] == 2 && board[i + 2][j] == 0)
			{
				find22[i][j]++;
			}
		}
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 0 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				find22[j][i]++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 0 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				find22[j][i]++;
			}
		}
	}
	//-----------------------020020
	for (i = 3; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 3] == 0 && board[j][i - 2] == 2 && board[j][i - 1] == 0 && board[j][i] == 0 && board[j][i + 1] == 2 && board[j][i + 2] == 0)
			{
				find22[j][i]++;
				find22[j][i - 1]++;
			}
			else if (board[i - 3][j] == 0 && board[i - 2][j] == 2 && board[i - 1][j] == 0 && board[i][j] == 0 && board[i + 1][j] == 2 && board[i + 2][j] == 0)
			{
				find22[i][j]++;
				find22[i - 1][j]++;
			}
		}
	}
	for (i = 3; i < 18; i++)
	{
		for (j = 3; j < 18; j++)
		{
			if (board[j - 3][i - 3] == 0 && board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 0 && board[j][i] == 0 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0)
			{
				find22[j][i]++;
				find22[j - 1][i - 1]++;
			}
			else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == 0 && board[j][i] == 0 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0)
			{
				find22[j][i]++;
				find22[j - 1][i + 1]++;
			}
		}
	}
	//0020200
	for (i = 3; i < 17; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 3] == 0 && board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 0 && board[j][i + 1] == 2 && board[j][i + 2] == 0 && board[j][i + 3] == 0)
			{
				find22[j][i - 2]++;
				find22[j][i + 2]++;
			}
			else if (board[i - 3][j] == 0 && board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 0 && board[i + 1][j] == 2 && board[i + 2][j] == 0 && board[i + 3][j] == 0)
			{
				find22[i - 2][j]++;
				find22[i + 2][j]++;
			}
		}
	}
	for (i = 3; i < 17; i++)
	{
		for (j = 3; j < 17; j++)
		{
			if (board[j - 3][i - 3] == 0 && board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 0 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0 && board[j + 3][i + 3] == 0)
			{
				find22[j - 2][i - 2]++;
				find22[j + 2][i + 2]++;
			}
			else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 0 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0 && board[j + 3][i - 3] == 0)
			{
				find22[j - 2][i + 2]++;
				find22[j + 2][i - 2]++;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////
	zero = 0, two = 2, x = 0;
	for (x = 0; x < 2; x++)
	{
		for (k = 0; k < 2; k++)
		{
			for (i = 3; i < 18; i++)
			{
				for (j = 0; j < 20; j++)
				{
					if (board[j][i - 3] == zero2 && board[j][i - 2] == zero && board[j][i - 1] == zero && board[j][i] == two && board[j][i + 1] == two && board[j][i + 2] == one)
					{
						if (k == 0)
						{
							Bfind22[j][i - 1]++;
							Bfind22[j][i - 2]++;
						}
						if (k == 1)
						{
							Bfind22[j][i]++;
							Bfind22[j][i + 1]++;
						}
					}
					else if (board[i - 3][j] == zero2 && board[i - 2][j] == zero && board[i - 1][j] == zero && board[i][j] == two && board[i + 1][j] == two && board[i + 2][j] == one)
					{
						if (k == 0)
						{
							Bfind22[i - 2][j]++;
							Bfind22[i - 1][j]++;
						}
						if (k == 1)
						{
							Bfind22[i][j]++;
							Bfind22[i + 1][j]++;
						}
					}
				}
			}
			for (i = 3; i < 18; i++)
			{
				for (j = 3; j < 18; j++)
				{
					if (board[j - 3][i - 3] == zero2 && board[j - 2][i - 2] == zero && board[j - 1][i - 1] == zero && board[j][i] == two && board[j + 1][i + 1] == two && board[j + 2][i + 2] == one)
					{
						if (k == 0)
						{
							Bfind22[j - 1][i - 1]++;
							Bfind22[j - 2][i - 2]++;
						}
						if (k == 1)
						{
							Bfind22[j][i]++;
							Bfind22[j + 1][i + 1]++;
						}
					}
					else if (board[j - 3][i + 3] == zero2 && board[j - 2][i + 2] == zero && board[j - 1][i + 1] == zero && board[j][i] == two && board[j + 1][i - 1] == two && board[j + 2][i - 2] == one)
					{
						if (k == 0)
						{
							Bfind22[j - 1][i + 1]++;
							Bfind22[j - 2][i + 2]++;
						}
						if (k == 1)
						{
							Bfind22[j][i]++;
							Bfind22[j + 1][i - 1]++;
						}
					}
				}
			}
			temp = zero;
			zero = two;
			two = temp;
		}
		//-------------------------------------02020----------------------------------------------------------------------------------------------------
		for (i = 2; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == zero2 && board[j][i - 1] == 2 && board[j][i] == 0 && board[j][i + 1] == 2 && board[j][i + 2] == one)
				{
					Bfind22[j][i]++;
				}
				else if (board[i - 2][j] == zero2 && board[i - 1][j] == 2 && board[i][j] == 0 && board[i + 1][j] == 2 && board[i + 2][j] == one)
				{
					Bfind22[i][j]++;
				}
			}
		}
		for (i = 2; i < 18; i++)
		{
			for (j = 2; j < 18; j++)
			{
				if (board[j - 2][i - 2] == zero2 && board[j - 1][i - 1] == 2 && board[j][i] == 0 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == one)
				{
					Bfind22[j][i]++;
				}
				else if (board[j - 2][i + 2] == zero2 && board[j - 1][i + 1] == 2 && board[j][i] == 0 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == one)
				{
					Bfind22[j][i]++;
				}
			}
		}
		//-----------------------020020
		for (i = 3; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 3] == zero2 && board[j][i - 2] == 2 && board[j][i - 1] == 0 && board[j][i] == 0 && board[j][i + 1] == 2 && board[j][i + 2] == one)
				{
					Bfind22[j][i]++;
					Bfind22[j][i - 1]++;
				}
				else if (board[i - 3][j] == zero2 && board[i - 2][j] == 2 && board[i - 1][j] == 0 && board[i][j] == 0 && board[i + 1][j] == 2 && board[i + 2][j] == one)
				{
					Bfind22[i][j]++;
					Bfind22[i - 1][j]++;
				}
			}
		}
		for (i = 3; i < 18; i++)
		{
			for (j = 3; j < 18; j++)
			{
				if (board[j - 3][i - 3] == zero2 && board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 0 && board[j][i] == 0 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == one)
				{
					Bfind22[j][i]++;
					Bfind22[j - 1][i - 1]++;
				}
				else if (board[j - 3][i + 3] == zero2 && board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == 0 && board[j][i] == 0 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == one)
				{
					Bfind22[j][i]++;
					Bfind22[j - 1][i + 1]++;
				}
			}
		}
		//0020200
		for (i = 3; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 3] == zero2 && board[j][i - 2] == 0 && board[j][i - 1] == 2 && board[j][i] == 0 && board[j][i + 1] == 2 && board[j][i + 2] == 0 && board[j][i + 3] == one)
				{
					Bfind22[j][i - 2]++;
					Bfind22[j][i + 2]++;
				}
				else if (board[i - 3][j] == zero2 && board[i - 2][j] == 0 && board[i - 1][j] == 2 && board[i][j] == 0 && board[i + 1][j] == 2 && board[i + 2][j] == 0 && board[i + 3][j] == one)
				{
					Bfind22[i - 2][j]++;
					Bfind22[i + 2][j]++;
				}
			}
		}
		for (i = 3; i < 17; i++)
		{
			for (j = 3; j < 17; j++)
			{
				if (board[j - 3][i - 3] == zero2 && board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 2 && board[j][i] == 0 && board[j + 1][i + 1] == 2 && board[j + 2][i + 2] == 0 && board[j + 3][i + 3] == one)
				{
					Bfind22[j - 2][i - 2]++;
					Bfind22[j + 2][i + 2]++;
				}
				else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 2 && board[j][i] == 0 && board[j + 1][i - 1] == 2 && board[j + 2][i - 2] == 0 && board[j + 3][i - 3] == one)
				{
					Bfind22[j - 2][i + 2]++;
					Bfind22[j + 2][i - 2]++;
				}
			}
		}
		temp = zero2;
		zero2 = one;
		one = temp;
	}
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (((Bfind33[i][j] > 0) && (find22[i][j] > 0)) || ((find33[i][j] > 0) && (Bfind22[i][j] > 0)) || ((find33[i][j] > 0) && (find22[i][j] > 0)))
			{
				x = j;
				y = i;
			}
			if (find22[i][j] > 1)
			{
				x2 = j;
				y2 = i;
			}
		}
	}
	if (x != 0 && y != 0)
	{
		weight_board[y][x] -= Gard_43;
	}
	if (x2 != 0 && y2 != 0)
	{
		weight_board[y2][x2] -= Gard333;
	}
}
void Attack_43(int board[][20], int weight_board[][20], int find3)
{
	int i, j, k, zero = 0, two = 1, temp, zero2 = 0, one = 2, x = 0, y = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
	int find33[20][20], Bfind33[20][20];
	int find22[20][20], Bfind22[20][20];
	for (i = 0; i<20; i++)
		for (j = 0; j < 20; j++)
		{
			find33[i][j] = 0;
			Bfind33[i][j] = 0;
			find22[i][j] = 0;
			Bfind22[i][j] = 0;
		}
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 0)
			{
				find33[j][i - 2]++;
				find33[j][i + 2]++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
			{
				find33[i - 2][j]++;
				find33[i + 2][j]++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 1 && board[j][i + 3] == 0)
				{
					find33[j][i - 2]++;
					find33[j][i + 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j][i + 1] == 0) find33[j][i + 1]++;
				}
				else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == zero && board[i + 1][j] == two && board[i + 2][j] == 1 && board[i + 3][j] == 0)
				{
					find33[i - 2][j]++;
					find33[i + 3][j]++;
					if (board[i][j] == 0) find33[i][j]++;
					if (board[i + 1][j] == 0) find33[i + 1][j]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0)
			{
				find33[j - 2][i - 2]++;
				find33[j + 2][i + 2]++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0)
			{
				find33[j - 2][i + 2]++;
				find33[j + 2][i - 2]++;
			}
		}
	}
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == 0)
				{
					find33[j - 2][i - 2]++;
					find33[j + 3][i + 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i + 1] == 0) find33[j + 1][i + 1]++;
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 1 && board[i + 3][j - 3] == 0)
				{
					find33[j - 2][i + 2]++;
					find33[j + 3][i - 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i - 1] == 0) find33[j + 1][i - 1]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------가로 & 세로----------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			//-------------------------------------------------------가로----------------------------------------------------------------
			if (board[j][i - 2] == 2 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 0)
			{
				Bfind33[j][i + 2]++;
				Bfind33[j][i + 3]++;
			}
			else if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 1 && board[j][i + 1] == 1 && board[j][i + 2] == 2)
			{
				Bfind33[j][i - 2]++;
				Bfind33[j][i - 3]++;
			}
			//-----------------------------------------------------세로------------------------------------------------------------------
			else if (board[i - 2][j] == 2 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
			{
				Bfind33[i + 2][j]++;
				Bfind33[i + 3][j]++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 2)
			{
				Bfind33[i - 2][j]++;
				Bfind33[i - 3][j]++;
			}
		}
	}
	//-------------------------------------------------------------가로 & 세로 2------------------------------------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == 2 && board[j][i - 1] == 1 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 1 && board[j][i + 3] == 0)//120220
				{
					Bfind33[j][i + 3]++;
					if (board[j][i] == 0) Bfind33[j][i]++;
					if (board[j][i + 1] == 0) Bfind33[j][i + 1]++;
				}
				else if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == zero && board[j][i + 1] == two && board[j][i + 2] == 1 && board[j][i + 3] == 2)//020221
				{
					Bfind33[j][i - 2]++;
					if (board[j][i] == 0) Bfind33[j][i]++;
					if (board[j][i + 1] == 0) Bfind33[j][i + 1]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------대각 & 반대각-------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0)
			{
				Bfind33[j + 2][i + 2]++;
				Bfind33[j + 3][i + 3]++;
			}
			else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 1 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 2)
			{
				Bfind33[j - 2][i - 2]++;
				Bfind33[j - 3][i - 3]++;
			}
			//-----
			else if (board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0)
			{
				Bfind33[j + 2][i - 2]++;
				Bfind33[j + 3][i - 3]++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 1 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 2)
			{
				Bfind33[j - 2][i + 2]++;
				Bfind33[j - 3][i + 3]++;
			}
		}
	}
	//--------------------------------------------------------------대각 & 반대각 2--------------------------------------------------------------------------
	for (k = 0; k < 2; k++)
	{
		for (i = 2; i < 17; i++)
		{
			for (j = 2; j < 17; j++)
			{
				if (board[j - 2][i - 2] == 2 && board[j - 1][i - 1] == 1 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == 0)
				{
					Bfind33[j + 3][i + 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i + 1] == 0) find33[j + 1][i + 1]++;
				}
				else if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == zero && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 1 && board[j + 3][i + 3] == 2)
				{
					Bfind33[j - 2][i - 2]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i + 1] == 0) find33[j + 1][i + 1]++;
				}
				//----------------------
				else if (board[j - 2][i + 2] == 2 && board[j - 1][i + 1] == 1 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 1 && board[i + 3][j - 3] == 0)
				{
					Bfind33[j + 3][i - 3]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i - 1] == 0) find33[j + 1][i - 1]++;
				}
				else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == zero && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 1 && board[i + 3][j - 3] == 2)
				{
					Bfind33[j - 2][i + 2]++;
					if (board[j][i] == 0) find33[j][i]++;
					if (board[j + 1][i - 1] == 0) find33[j + 1][i - 1]++;
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	// 000220 022000
	zero = 0, two = 1;
	for (k = 0; k < 2; k++)
	{
		for (i = 3; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 3] == 0 && board[j][i - 2] == zero && board[j][i - 1] == zero && board[j][i] == two && board[j][i + 1] == two && board[j][i + 2] == 0)
				{
					if (k == 0)
					{
						find22[j][i - 1]++;
						find22[j][i - 2]++;
					}
					if (k == 1)
					{
						find22[j][i]++;
						find22[j][i + 1]++;
					}
				}
				else if (board[i - 3][j] == 0 && board[i - 2][j] == zero && board[i - 1][j] == zero && board[i][j] == two && board[i + 1][j] == two && board[i + 2][j] == 0)
				{
					if (k == 0)
					{
						find22[i - 2][j]++;
						find22[i - 1][j]++;
					}
					if (k == 1)
					{
						find22[i][j]++;
						find22[i + 1][j]++;
					}
				}
			}
		}
		for (i = 3; i < 18; i++)
		{
			for (j = 3; j < 18; j++)
			{
				if (board[j - 3][i - 3] == 0 && board[j - 2][i - 2] == zero && board[j - 1][i - 1] == zero && board[j][i] == two && board[j + 1][i + 1] == two && board[j + 2][i + 2] == 0)
				{
					if (k == 0)
					{
						find22[j - 1][i - 1]++;
						find22[j - 2][i - 2]++;
					}
					if (k == 1)
					{
						find22[j][i]++;
						find22[j + 1][i + 1]++;
					}
				}
				else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == zero && board[j - 1][i + 1] == zero && board[j][i] == two && board[j + 1][i - 1] == two && board[j + 2][i - 2] == 0)
				{
					if (k == 0)
					{
						find22[j - 1][i + 1]++;
						find22[j - 2][i + 2]++;
					}
					if (k == 1)
					{
						find22[j][i]++;
						find22[j + 1][i - 1]++;
					}
				}
			}
		}
		temp = zero;
		zero = two;
		two = temp;
	}
	//-------------------------------------02020----------------------------------------------------------------------------------------------------
	for (i = 2; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 0 && board[j][i + 1] == 1 && board[j][i + 2] == 0)
			{
				find22[j][i]++;
			}
			else if (board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
			{
				find22[i][j]++;
			}
		}
	}
	for (i = 2; i < 18; i++)
	{
		for (j = 2; j < 18; j++)
		{
			if (board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 0 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0)
			{
				find22[j][i]++;
			}
			else if (board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 0 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0)
			{
				find22[j][i]++;
			}
		}
	}
	//-----------------------020020
	for (i = 3; i < 18; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 3] == 0 && board[j][i - 2] == 1 && board[j][i - 1] == 0 && board[j][i] == 0 && board[j][i + 1] == 1 && board[j][i + 2] == 0)
			{
				find22[j][i]++;
				find22[j][i - 1]++;
			}
			else if (board[i - 3][j] == 0 && board[i - 2][j] == 1 && board[i - 1][j] == 0 && board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
			{
				find22[i][j]++;
				find22[i - 1][j]++;
			}
		}
	}
	for (i = 3; i < 18; i++)
	{
		for (j = 3; j < 18; j++)
		{
			if (board[j - 3][i - 3] == 0 && board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 0 && board[j][i] == 0 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0)
			{
				find22[j][i]++;
				find22[j - 1][i - 1]++;
			}
			else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 0 && board[j][i] == 0 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0)
			{
				find22[j][i]++;
				find22[j - 1][i + 1]++;
			}
		}
	}
	//0020200
	for (i = 3; i < 17; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[j][i - 3] == 0 && board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 0 && board[j][i + 1] == 1 && board[j][i + 2] == 0 && board[j][i + 3] == 0)
			{
				find22[j][i - 2]++;
				find22[j][i + 2]++;
			}
			else if (board[i - 3][j] == 0 && board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == 0)
			{
				find22[i - 2][j]++;
				find22[i + 2][j]++;
			}
		}
	}
	for (i = 3; i < 17; i++)
	{
		for (j = 3; j < 17; j++)
		{
			if (board[j - 3][i - 3] == 0 && board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 0 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0 && board[j + 3][i + 3] == 0)
			{
				find22[j - 2][i - 2]++;
				find22[j + 2][i + 2]++;
			}
			else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 0 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0 && board[j + 3][i - 3] == 0)
			{
				find22[j - 2][i + 2]++;
				find22[j + 2][i - 2]++;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////
	zero = 0, two = 1, x = 0;
	for (x = 0; x < 2; x++)
	{
		for (k = 0; k < 2; k++)
		{
			for (i = 3; i < 18; i++)
			{
				for (j = 0; j < 20; j++)
				{
					if (board[j][i - 3] == zero2 && board[j][i - 2] == zero && board[j][i - 1] == zero && board[j][i] == two && board[j][i + 1] == two && board[j][i + 2] == one)
					{
						if (k == 0)
						{
							Bfind22[j][i - 1]++;
							Bfind22[j][i - 2]++;
						}
						if (k == 1)
						{
							Bfind22[j][i]++;
							Bfind22[j][i + 1]++;
						}
					}
					else if (board[i - 3][j] == zero2 && board[i - 2][j] == zero && board[i - 1][j] == zero && board[i][j] == two && board[i + 1][j] == two && board[i + 2][j] == one)
					{
						if (k == 0)
						{
							Bfind22[i - 2][j]++;
							Bfind22[i - 1][j]++;
						}
						if (k == 1)
						{
							Bfind22[i][j]++;
							Bfind22[i + 1][j]++;
						}
					}
				}
			}
			for (i = 3; i < 18; i++)
			{
				for (j = 3; j < 18; j++)
				{
					if (board[j - 3][i - 3] == zero2 && board[j - 2][i - 2] == zero && board[j - 1][i - 1] == zero && board[j][i] == two && board[j + 1][i + 1] == two && board[j + 2][i + 2] == one)
					{
						if (k == 0)
						{
							Bfind22[j - 1][i - 1]++;
							Bfind22[j - 2][i - 2]++;
						}
						if (k == 1)
						{
							Bfind22[j][i]++;
							Bfind22[j + 1][i + 1]++;
						}
					}
					else if (board[j - 3][i + 3] == zero2 && board[j - 2][i + 2] == zero && board[j - 1][i + 1] == zero && board[j][i] == two && board[j + 1][i - 1] == two && board[j + 2][i - 2] == one)
					{
						if (k == 0)
						{
							Bfind22[j - 1][i + 1]++;
							Bfind22[j - 2][i + 2]++;
						}
						if (k == 1)
						{
							Bfind22[j][i]++;
							Bfind22[j + 1][i - 1]++;
						}
					}
				}
			}
			temp = zero;
			zero = two;
			two = temp;
		}
		//-------------------------------------02020----------------------------------------------------------------------------------------------------
		for (i = 2; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 2] == zero2 && board[j][i - 1] == 1 && board[j][i] == 0 && board[j][i + 1] == 1 && board[j][i + 2] == one)
				{
					Bfind22[j][i]++;
				}
				else if (board[i - 2][j] == zero2 && board[i - 1][j] == 1 && board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == one)
				{
					Bfind22[i][j]++;
				}
			}
		}
		for (i = 2; i < 18; i++)
		{
			for (j = 2; j < 18; j++)
			{
				if (board[j - 2][i - 2] == zero2 && board[j - 1][i - 1] == 1 && board[j][i] == 0 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == one)
				{
					Bfind22[j][i]++;
				}
				else if (board[j - 2][i + 2] == zero2 && board[j - 1][i + 1] == 1 && board[j][i] == 0 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == one)
				{
					Bfind22[j][i]++;
				}
			}
		}
		//-----------------------020020
		for (i = 3; i < 18; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 3] == zero2 && board[j][i - 2] == 1 && board[j][i - 1] == 0 && board[j][i] == 0 && board[j][i + 1] == 1 && board[j][i + 2] == one)
				{
					Bfind22[j][i]++;
					Bfind22[j][i - 1]++;
				}
				else if (board[i - 3][j] == zero2 && board[i - 2][j] == 1 && board[i - 1][j] == 0 && board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == one)
				{
					Bfind22[i][j]++;
					Bfind22[i - 1][j]++;
				}
			}
		}
		for (i = 3; i < 18; i++)
		{
			for (j = 3; j < 18; j++)
			{
				if (board[j - 3][i - 3] == zero2 && board[j - 2][i - 2] == 1 && board[j - 1][i - 1] == 0 && board[j][i] == 0 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == one)
				{
					Bfind22[j][i]++;
					Bfind22[j - 1][i - 1]++;
				}
				else if (board[j - 3][i + 3] == zero2 && board[j - 2][i + 2] == 1 && board[j - 1][i + 1] == 0 && board[j][i] == 0 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == one)
				{
					Bfind22[j][i]++;
					Bfind22[j - 1][i + 1]++;
				}
			}
		}
		//0020200
		for (i = 3; i < 17; i++)
		{
			for (j = 0; j < 20; j++)
			{
				if (board[j][i - 3] == zero2 && board[j][i - 2] == 0 && board[j][i - 1] == 1 && board[j][i] == 0 && board[j][i + 1] == 1 && board[j][i + 2] == 0 && board[j][i + 3] == one)
				{
					Bfind22[j][i - 2]++;
					Bfind22[j][i + 2]++;
				}
				else if (board[i - 3][j] == zero2 && board[i - 2][j] == 0 && board[i - 1][j] == 1 && board[i][j] == 0 && board[i + 1][j] == 1 && board[i + 2][j] == 0 && board[i + 3][j] == one)
				{
					Bfind22[i - 2][j]++;
					Bfind22[i + 2][j]++;
				}
			}
		}
		for (i = 3; i < 17; i++)
		{
			for (j = 3; j < 17; j++)
			{
				if (board[j - 3][i - 3] == zero2 && board[j - 2][i - 2] == 0 && board[j - 1][i - 1] == 1 && board[j][i] == 0 && board[j + 1][i + 1] == 1 && board[j + 2][i + 2] == 0 && board[j + 3][i + 3] == one)
				{
					Bfind22[j - 2][i - 2]++;
					Bfind22[j + 2][i + 2]++;
				}
				else if (board[j - 3][i + 3] == 0 && board[j - 2][i + 2] == 0 && board[j - 1][i + 1] == 1 && board[j][i] == 0 && board[j + 1][i - 1] == 1 && board[j + 2][i - 2] == 0 && board[j + 3][i - 3] == one)
				{
					Bfind22[j - 2][i + 2]++;
					Bfind22[j + 2][i - 2]++;
				}
			}
		}
		temp = zero2;
		zero2 = one;
		one = temp;
	}
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (((Bfind33[i][j] > 0) && (find22[i][j] > 0)) || ((find33[i][j] > 0) && (Bfind22[i][j] > 0)) || ((find33[i][j] > 0) && (find22[i][j] > 0)))
			{
				x = j;
				y = i;
			}
			if (find22[i][j] > 1)
			{
				x2 = j;
				y2 = i;
			}
			if (find33[i][j] > 1)
			{
				x3 = j;
				y3 = i;
			}

		}
	}
	if (x != 0 && y != 0)
	{
		weight_board[y][x] -= Attack43;
	}
	if (x2 != 0 && y2 != 0 && find3 == 0)
	{
		weight_board[y2][x2] -= Attack22;
	}
	if (x3 != 0 && y3 != 0)
	{
		weight_board[y3][x3] -= Attack33;
	}
}
void AI(int board[][20], int weight_board[][20], int *turn)
{
	int find22, find33, w;
	int i, j, min_wei, find = 0, x, y, x2 = 0, y2 = 0;
	for (j = 0; j < 20; j += 18)
	{
		for (i = 0; i < 20; i++)
		{
			if (board[i][j] == 3)
				board[i][j] = 0;
		}
	}
	for (i = 0; i < 20; i += 18)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[i][j] == 3)
				board[i][j] = 0;
		}
	}
	find22 = Gard_22(board, weight_board, 1);
	Blocked_Gard_22(board, weight_board, find22, 1);
	find33 = Gard_33(board, weight_board, 1);
	Attack_43(board, weight_board, find33);
	Blocked_Gard_33(board, weight_board, find33, 1);
	Gard_44(board, weight_board, 1);
	Attack_33(board, weight_board, 1);
	Blocked_Attack_33(board, weight_board, 1);
	Attack_44(board, weight_board, 1);
	Gard43(board, weight_board);
	min_wei = weight_board[0][0];
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (min_wei > weight_board[i][j])
			{
				min_wei = weight_board[i][j];
				x = j;
				y = i;
			}
		}
	}
	gotoxy(x * 2, y);
	printf("●");
	weight_p1(weight_board, board, x * 2, y);
	find22 = Gard_22(board, weight_board, 0);
	Blocked_Gard_22(board, weight_board, find22, 0);
	Gard_33(board, weight_board, 0);
	Blocked_Gard_33(board, weight_board, find33, 0);
	board[y][x] = 1;
	weight_board[y][x] = 999;
	w = win(board);
	gotoxy(50, 20);
	printf("최소 가중치 : %d 위치 x : %d y : %d         ", min_wei, x * 2, y);
	//printf("최소 가중치 : %d 위치 x : %d y : %d  x2: %d y2: %d       ", min_wei, x * 2, y, x2 * 2, y2);
	if (w == 0)
		*turn = 1;
	if (w == 1)
		*turn = 3;
}
void weight_m1(int weight_board[][20], int board[][20], int x, int y)
{
	int i, j;
	for (i = -1; i<2; i++)
		for (j = -1; j < 2; j++)
		{
			if ((board[y + i][x / 2 + j] == 0 || board[y + i][x / 2 + j] == 3) && board[y + i][x / 2 + j] != 999)
				weight_board[y + i][x / 2 + j]--;
		}
}
void weight_p1(int weight_board[][20], int board[][20], int x, int y)
{
	int i, j;
	for (i = -1; i<2; i++)
		for (j = -1; j < 2; j++)
		{
			if ((board[y + i][x / 2 + j] == 0 || board[y + i][x / 2 + j] == 3) && board[y + i][x / 2 + j] != 999)
				weight_board[y + i][x / 2 + j]++;
		}
}
