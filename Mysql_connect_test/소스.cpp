#include <my_global.h>
#include <iostream>
#include <mysql.h>
#include<Windows.h>
#include<stdio.h>
#include<time.h>
#include <conio.h>
#include<stdlib.h>
#pragma commnet(lib, "libmySQL.lib");
#define DB_HOST ""
#define DB_USER ""
#define DB_PASS ""
#define DB_NAME ""
#define CHOP(x) x[strlen(x) - 1] = ' '

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32
#define ENTER 13
#define X 4
#define Y -2

#define RED     4
#define GREEN   2
#define YELLOW  6
#define TURQUOISE    3
#define BLUE 1
#define PINK    13
#define WHITE   7
#define ORANGE 12

typedef struct blocks {
	int x;
	int y;
	int prx;
	int pry;
	int turn;
	int shape;
	int color;
	int nextShape;
}blocks;

blocks test = { X,Y,X,Y,0,rand() % 7,0,rand() % 7 };

int choose();
int selectD(int x, int y);
void color(int i, int j, int(*map)[12]);
void colornext(int i, int j, int(*map)[5]);
void logo();
void printMap(int(*pmap)[12]);
void cleanMap(int(*pmap)[12]);
void gotoxy(int x, int y);
int moveBlock(int dir, int(*pmap)[12], int(*psq)[4][4][4],int lineinsert);
void block(int(*pmap)[12], int(*psq)[4][4][4], int x, int y);
void deleteLine(int l, int(*pmap)[12]);
void lineMulti(int(*pmap)[12]);
void lineSingle(int(*pmap)[12]);
void block(int(*pmap)[12], int(*psq)[4][4][4], int x, int y);
int moveCheck(int(*pmap)[12], int(*psq)[4][4][4], int x, int y);
void blockClean(int(*pmap)[12], int(*psq)[4][4][4], int x, int y);
int turnBlock(int(*pmap)[12], int(*psq)[4][4][4]);
void addLine(int(*pmap)[12]);

double period = 700;		//블럭이 내려오는 속도


int next[6][5]{
	{ 8,8,8,8,8 },
	{ 0,0,0,0,8 },
	{ 0,0,0,0,8 },
	{ 0,0,0,0,8 },
	{ 0,0,0,0,8 },
	{ 8,8,8,8,8 },
};

char name[12];
char query[255];
MYSQL* connection = NULL, conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;
int query_stat;
int lineInt = 0;				//내가 지운 라인
int mynumber = 0;					//내 이름이 저장된 위치

int main()											//처음 들어가면 2 늦게 들어가면 1
{
	int difficulty;
	int friendor;
	int loginor;
	int multior;
	char playerNumber[12];		//1 : 첫번째 플레이어 2 : 두번째 플레이어

	char opponent[12] = "0";		//상대 플레이어 아이디
			//사용자 입력 아이디
	char name2[12];		//서버의 아이디
	char address[80];	//사용자 입력 비밀번호
	char address2[80];	//서버의 비밀번호
	char signUp[12] = "new";
	char tel[12];

	char key;

	int loginstat = 0;

	time_t start, end;



	int map[21][12]{
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8},
		{8,8,8,8,8,8,8,8,8,8,8,8},
	};
	int(*pmap)[12] = map;
	int sq[8][4][4][4] = {	//[shape][turn][y][x]
	{
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,1},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
	},
	{
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
	},
	{
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
	},
	{
		{
			{0,0,0,0},
			{0,0,1,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,0,1,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
	},
	{
		{
			{0,0,0,0},
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
	},
	{
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	}

	};
	int(*psq)[4][4][4] = sq;
//selectD(10, 7);

	//CHOP(signUp);
	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST,
		DB_USER, DB_PASS,
		DB_NAME, 3306,
		(char*)NULL, 0);

	if (connection == NULL)
	{
		printf("wifi에 연결됬는지 확인 하세요.");
		return 1;
	}
	else {
		printf("연결됨");
	}
	//printf("로그인 하시려면 아래쪽 화살표를 회원가입을 하시려면 위쪽 화살표를 눌러 주세요\n");
	//로그인 하기			//로그인 하기
	gotoxy(17, 10);
	printf("로그인");
	gotoxy(18, 12);
	printf("하기");
	gotoxy(57, 10);
	printf("회원가입");
	gotoxy(58, 12);
	printf("하기");
	printf("\n\n\n\n\n\n\n\n\n");
	
	loginor = choose();
	
	switch (loginor) {
	case 0:
		system("cls");
	loginAgain:
		printf("login\n");
		printf("이름 :");
		fgets(name, 12, stdin);
		CHOP(name);

		printf("비밀번호 :");
		fgets(address, 80, stdin);
		CHOP(address);

		sprintf_s(query, "select * from login where id="
			"'%s'",
			name);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query SELECT : %s", mysql_error(&conn));
			return 1;
		}
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			strcpy_s(name2, sql_row[0]);
			strcpy_s(address2, sql_row[1]);
		}

		if (strcmp(name2, name) == 0 && strcmp(address2, address) == 0) {
			printf("로그인 성공\n");
		}
		else {
			system("cls");
			printf("로그인 실패 다시 입력\n");
			goto loginAgain;
		}
		loginstat = 1; //로그인 해서 루프 빠져 나감
		break;
	case 1:
		system("cls");
		printf("회원 가입을 합니다. 이름과 비밀번호를 입력해 주세요.\n");
		printf("이름 :");
		fgets(name, 12, stdin);
		CHOP(name);

		printf("비밀번호 :");
		fgets(address, 80, stdin);
		CHOP(address);
		sprintf_s(query, "insert into login values"
			"('%s','%s')",
			name, address);

		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		}
		printf("입력");
		break;
	}

	//mysql_free_result(sql_result);
	//로그인 하기
menu:
	system("cls");
	gotoxy(17, 10);
	printf("single");
	gotoxy(18, 12);
	printf("mode");
	gotoxy(55, 10);
	printf("multy play");
	gotoxy(58, 12);
	printf("mode");
	printf("\n\n\n\n\n\n\n\n\n");


	
	multior = choose();
	cleanMap(map);
	
	switch (multior)
	{
	case 1:
		system("cls");
		gotoxy(15, 10);
		printf("play with");
		gotoxy(18, 12);
		printf("anyone");
		gotoxy(53, 10);
		printf("play with");
		gotoxy(58, 12);
		printf("friends");
		printf("\n\n\n\n\n\n\n\n\n");
		friendor = choose();
		cleanMap(map);
		switch (friendor)
		{
		case 0:

			system("cls");
			printf("\n\n\n\t\t--Press SPACE to Start--");
			while (1) {
				if (_kbhit()) {
					key = _getch();
					if (key == SPACEBAR) {
						break;

					}
				}
			}


			query_stat = mysql_query(connection, "select * from test where player = 1");			// 한명인 곳 찾기
			if (query_stat != 0)
			{
				fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
				return 1;
			}

			sql_result = mysql_store_result(connection);

			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("sd");
				strcpy_s(playerNumber, sql_row[0]);
				//strcpy_s(opponent, sql_row[1]);
				//strcpy_s(opponent, sql_row[2]);
				//strcpy_s(name, sql_row[3]);
				strcpy_s(opponent, sql_row[4]);		//playerName2 가 남
				printf("%s, %s, %s", playerNumber, sql_row[3], opponent);

				break;
			}

			printf("dsfsdfsd\n");

			if (strcmp(playerNumber, "1") != 0) {									//만약 빈 곳이 없을 경우 만들기
				mynumber = 2;

				printf("created\n");
				sprintf_s(query, "insert into test (player,line1,line2,playerName1,playerName2,endGame) values (1,0,0,'모름',"
					"'%s',0)",
					name);

				query_stat = mysql_query(connection, query);
				if (query_stat != 0)
				{
					fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
					return 1;
				}

				while (1) {
					sprintf_s(query, "select * from test where playerName2 = ""'%s'", name);				//매칭 하기 여기서 부터 수정

					query_stat = mysql_query(connection, query);

					if (query_stat != 0)
					{
						fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
						return 1;
					}

					sql_result = mysql_store_result(connection);

					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\n", sql_row[0]);
						strcpy_s(playerNumber, sql_row[0]);
						//printf("%s\n", sql_row[1]);
						//printf("%s\n", sql_row[2]);
						//printf("%s\n", sql_row[3]);
						//printf("%s\n", sql_row[4]);

						break;
					}
					printf("%s\n", playerNumber);
					if (strcmp(playerNumber, "2") == 0) {
						//printf("sdfasdfsdafasdfdsfsadfsdafsadfsda\n");
						break;
					}
				}

			}
			else {												//빈곳에 들어가서 내이름 입력하고 플레이어 수 2명으로 수정
				mynumber = 1;
				CHOP(name);
				CHOP(opponent);
				sprintf_s(query, "UPDATE test SET player = 2 WHERE playerName2 = ""'%s'", opponent);
				printf("업데이트 %s \n", query);
				query_stat = mysql_query(connection, query);
				if (query_stat != 0)
				{
					fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
					return 1;
				}
				sprintf_s(query, "UPDATE test SET playerName1 = ""'%s'"" WHERE playerName2 = ""'%s'", name, opponent);
				printf("업데이트 %s \n", query);
				query_stat = mysql_query(connection, query);
				if (query_stat != 0)
				{
					fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
					return 1;
				}

			}

			//printf("sdkfjsdlf");


			system("cls");
			logo();//여기서 부터 테트리스
			start = clock();
			printMap(pmap);
			while (1) {
				if (mynumber == 2) {
					sprintf_s(query, "select * from test where playerName2 = ""'%s'", name);

					query_stat = mysql_query(connection, query);

					if (query_stat != 0)
					{
						fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
						return 1;
					}

					sql_result = mysql_store_result(connection);

					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						//printf("%s\n", sql_row[1]);
						if (strcmp(sql_row[5], "1") == 0) {
							goto end;
							break;
						}

						break;
					}
				}
				else {
					sprintf_s(query, "select * from test where playerName1 = ""'%s'", name);

					query_stat = mysql_query(connection, query);

					if (query_stat != 0)
					{
						fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
						return 1;
					}

					sql_result = mysql_store_result(connection);

					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						//printf("%s\n", sql_row[1]);
						if (strcmp(sql_row[5], "1") == 0) {
							goto end;
							break;
						}

						break;
					}
				}


				period = 350;
				if (_kbhit()) {
					key = _getch();
					switch (key) {
					case DOWN:
						period = 10;
						break;

					case SPACEBAR: if (moveBlock(3, map, sq, multior) == 2) {
						break;
					}
								   printMap(map); break;

					case LEFT: moveBlock(2, map, sq, multior); printMap(map); break;
					case RIGHT: moveBlock(1, map, sq, multior); printMap(map); break;
					case ENTER: addLine(map); break;
					}
					if (key == UP) {
						turnBlock(map, sq);

					}

				}


				end = clock();
				if ((double)(end - start) >= period) {
					start = clock();
					if (moveBlock(3, map, sq, multior) == 2) {

						if (mynumber == 2) {
							sprintf_s(query, "UPDATE test SET endGame = 1 WHERE playerName2 = ""'%s'", name);

							query_stat = mysql_query(connection, query);

							if (query_stat != 0)
							{
								fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
								return 1;
							}

							sql_result = mysql_store_result(connection);

						}
						else {
							sprintf_s(query, "UPDATE test SET endGame = 1 WHERE playerName1 = ""'%s'", name);

							query_stat = mysql_query(connection, query);

							if (query_stat != 0)
							{
								fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
								return 1;
							}

							sql_result = mysql_store_result(connection);

						}
					end:
						printf("return2");
						break;
					}

					if (mynumber == 2) {
						sprintf_s(query, "select * from test where playerName2 = ""'%s'", name);				//라인 추가 실험

						query_stat = mysql_query(connection, query);

						if (query_stat != 0)
						{
							fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
							return 1;
						}

						sql_result = mysql_store_result(connection);

						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s\n", sql_row[1]);
							if (strcmp(sql_row[1], "1") == 0) {
								addLine(map);
								break;
							}
							else if (strcmp(sql_row[1], "2") == 0) {
								addLine(map);
								addLine(map);
								break;
							}
							else if (strcmp(sql_row[1], "3") == 0) {
								addLine(map);
								addLine(map);
								addLine(map);
								break;
							}
							else if (strcmp(sql_row[1], "4") == 0) {
								addLine(map);
								addLine(map);
								addLine(map);
								addLine(map);
								break;
							}
							//printf("%s\n", sql_row[1]);
							//printf("%s\n", sql_row[2]);
							//printf("%s\n", sql_row[3]);
							//printf("%s\n", sql_row[4]);

							break;
						}
						sprintf_s(query, "UPDATE test SET line1 = 0 WHERE playerName2 = ""'%s'", name);
						//printf("업데이트 %s \n", query);
						query_stat = mysql_query(connection, query);
						if (query_stat != 0)
						{
							fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
							return 1;
						}

					}			//상대가 없엔 라인 추가하기
					else {
						sprintf_s(query, "select * from test where playerName1 = ""'%s'", name);				//라인 추가 실험

						query_stat = mysql_query(connection, query);

						if (query_stat != 0)
						{
							fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
							return 1;
						}

						sql_result = mysql_store_result(connection);

						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s\n", sql_row[2]);
							if (strcmp(sql_row[2], "1") == 0) {
								addLine(map);
								break;
							}
							else if (strcmp(sql_row[2], "2") == 0) {
								addLine(map);
								addLine(map);
								break;
							}
							else if (strcmp(sql_row[2], "3") == 0) {
								addLine(map);
								addLine(map);
								addLine(map);
								break;
							}
							else if (strcmp(sql_row[2], "4") == 0) {
								addLine(map);
								addLine(map);
								addLine(map);
								addLine(map);
								break;
							}
							//printf("%s\n", sql_row[1]);
							//printf("%s\n", sql_row[2]);
							//printf("%s\n", sql_row[3]);
							//printf("%s\n", sql_row[4]);

							break;
						}
						sprintf_s(query, "UPDATE test SET line2 = 0 WHERE playerName1 = ""'%s'", name);
						//printf("업데이트 %s \n", query);
						query_stat = mysql_query(connection, query);
						if (query_stat != 0)
						{
							fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
							return 1;
						}

					}
				}
			}
			cleanMap(map);
			start = clock();
			system("cls");
			logo();
			gotoxy(15, 15);
			printf("Game Over!!!");
			while (1) {
				end = clock();
				if ((double)(end - start) >= 3000) {
					break;
				}
			}
			sprintf_s(query, "DELETE FROM test WHERE playerName2 = ""'%s'", name);							//지우기
			printf("딜리트 %s \n", query);
			query_stat = mysql_query(connection, query);
			if (query_stat != 0)
			{
				fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
				return 1;
			}
			goto menu;


			break;


			case 1:

				system("cls");
				printf("\n\n\n\t\t--Press SPACE to Start--");
				while (1) {
					if (_kbhit()) {
						key = _getch();
						if (key == SPACEBAR) {
							break;

						}
					}
				}

				sprintf_s(query, "select * from test where player = 1 and playerName1 = ""'%s'", name);
				query_stat = mysql_query(connection, query);			// 한명인 곳 찾기
				if (query_stat != 0)
				{
					fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
					return 1;
				}

				sql_result = mysql_store_result(connection);

				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("sd");
					strcpy_s(playerNumber, sql_row[0]);
					//strcpy_s(opponent, sql_row[1]);
					//strcpy_s(opponent, sql_row[2]);
					//strcpy_s(name, sql_row[3]);
					strcpy_s(opponent, sql_row[4]);		//playerName2 가 남
					printf("%s, %s, %s", playerNumber, sql_row[3], opponent);

					break;
				}

				printf("dsfsdfsd\n");

				if (strcmp(playerNumber, "1") != 0) {									//만약 빈 곳이 없을 경우 만들기
					mynumber = 2;
					printf("친구의 이름을 입력하세요 :");
					scanf_s("%s", name2);

					printf("created\n");
					sprintf_s(query, "insert into test (player,line1,line2,playerName1,playerName2,endGame) values (1,0,0,""'%s',"
						"'%s',0)",
						name2, name);

					query_stat = mysql_query(connection, query);
					if (query_stat != 0)
					{
						fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
						return 1;
					}

					while (1) {
						sprintf_s(query, "select * from test where playerName2 = ""'%s'", name);				//매칭 하기 여기서 부터 수정

						query_stat = mysql_query(connection, query);

						if (query_stat != 0)
						{
							fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
							return 1;
						}

						sql_result = mysql_store_result(connection);

						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s\n", sql_row[0]);
							strcpy_s(playerNumber, sql_row[0]);
							//printf("%s\n", sql_row[1]);
							//printf("%s\n", sql_row[2]);
							//printf("%s\n", sql_row[3]);
							//printf("%s\n", sql_row[4]);

							break;
						}
						printf("%s\n", playerNumber);
						if (strcmp(playerNumber, "2") == 0) {
							//printf("sdfasdfsdafasdfdsfsadfsdafsadfsda\n");
							break;
						}
					}

				}
				else {												//빈곳에 들어가서 내이름 입력하고 플레이어 수 2명으로 수정
					mynumber = 1;
					CHOP(name);
					CHOP(opponent);
					sprintf_s(query, "UPDATE test SET player = 2 WHERE playerName2 = ""'%s'", opponent);
					printf("업데이트 %s \n", query);
					query_stat = mysql_query(connection, query);
					if (query_stat != 0)
					{
						fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
						return 1;
					}
					sprintf_s(query, "UPDATE test SET playerName1 = ""'%s'"" WHERE playerName2 = ""'%s'", name, opponent);
					printf("업데이트 %s \n", query);
					query_stat = mysql_query(connection, query);
					if (query_stat != 0)
					{
						fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
						return 1;
					}

				}

				//printf("sdkfjsdlf");


				system("cls");
				logo();//여기서 부터 테트리스
				start = clock();
				printMap(pmap);
				while (1) {
					if (mynumber == 2) {
						sprintf_s(query, "select * from test where playerName2 = ""'%s'", name);

						query_stat = mysql_query(connection, query);

						if (query_stat != 0)
						{
							fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
							return 1;
						}

						sql_result = mysql_store_result(connection);

						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							//printf("%s\n", sql_row[1]);
							if (strcmp(sql_row[5], "1") == 0) {
								goto end;
								break;
							}

							break;
						}
					}
					else {
						sprintf_s(query, "select * from test where playerName1 = ""'%s'", name);

						query_stat = mysql_query(connection, query);

						if (query_stat != 0)
						{
							fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
							return 1;
						}

						sql_result = mysql_store_result(connection);

						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							//printf("%s\n", sql_row[1]);
							if (strcmp(sql_row[5], "1") == 0) {
								goto end2;
								break;
							}

							break;
						}
					}


					period = 350;
					if (_kbhit()) {
						key = _getch();
						switch (key) {
						case DOWN:
							period = 10;
							break;

						case SPACEBAR: if (moveBlock(3, map, sq, multior) == 2) {
							break;
						}
									   printMap(map); break;

						case LEFT: moveBlock(2, map, sq, multior); printMap(map); break;
						case RIGHT: moveBlock(1, map, sq, multior); printMap(map); break;
						case ENTER: addLine(map); break;
						}
						if (key == UP) {
							turnBlock(map, sq);

						}

					}


					end = clock();
					if ((double)(end - start) >= period) {
						start = clock();
						if (moveBlock(3, map, sq, multior) == 2) {

							if (mynumber == 2) {
								sprintf_s(query, "UPDATE test SET endGame = 1 WHERE playerName2 = ""'%s'", name);

								query_stat = mysql_query(connection, query);

								if (query_stat != 0)
								{
									fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
									return 1;
								}

								sql_result = mysql_store_result(connection);

							}
							else {
								sprintf_s(query, "UPDATE test SET endGame = 1 WHERE playerName1 = ""'%s'", name);

								query_stat = mysql_query(connection, query);

								if (query_stat != 0)
								{
									fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
									return 1;
								}

								sql_result = mysql_store_result(connection);

							}
						end2:
							printf("return2");
							break;
						}

						if (mynumber == 2) {
							sprintf_s(query, "select * from test where playerName2 = ""'%s'", name);				//라인 추가 실험

							query_stat = mysql_query(connection, query);

							if (query_stat != 0)
							{
								fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
								return 1;
							}

							sql_result = mysql_store_result(connection);

							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s\n", sql_row[1]);
								if (strcmp(sql_row[1], "1") == 0) {
									addLine(map);
									break;
								}
								else if (strcmp(sql_row[1], "2") == 0) {
									addLine(map);
									addLine(map);
									break;
								}
								else if (strcmp(sql_row[1], "3") == 0) {
									addLine(map);
									addLine(map);
									addLine(map);
									break;
								}
								else if (strcmp(sql_row[1], "4") == 0) {
									addLine(map);
									addLine(map);
									addLine(map);
									addLine(map);
									break;
								}
								//printf("%s\n", sql_row[1]);
								//printf("%s\n", sql_row[2]);
								//printf("%s\n", sql_row[3]);
								//printf("%s\n", sql_row[4]);

								break;
							}
							sprintf_s(query, "UPDATE test SET line1 = 0 WHERE playerName2 = ""'%s'", name);
							//printf("업데이트 %s \n", query);
							query_stat = mysql_query(connection, query);
							if (query_stat != 0)
							{
								fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
								return 1;
							}

						}			//상대가 없엔 라인 추가하기
						else {
							sprintf_s(query, "select * from test where playerName1 = ""'%s'", name);				//라인 추가 실험

							query_stat = mysql_query(connection, query);

							if (query_stat != 0)
							{
								fprintf(stderr, "1Mysql query error : %s", mysql_error(&conn));
								return 1;
							}

							sql_result = mysql_store_result(connection);

							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s\n", sql_row[2]);
								if (strcmp(sql_row[2], "1") == 0) {
									addLine(map);
									break;
								}
								else if (strcmp(sql_row[2], "2") == 0) {
									addLine(map);
									addLine(map);
									break;
								}
								else if (strcmp(sql_row[2], "3") == 0) {
									addLine(map);
									addLine(map);
									addLine(map);
									break;
								}
								else if (strcmp(sql_row[2], "4") == 0) {
									addLine(map);
									addLine(map);
									addLine(map);
									addLine(map);
									break;
								}
								//printf("%s\n", sql_row[1]);
								//printf("%s\n", sql_row[2]);
								//printf("%s\n", sql_row[3]);
								//printf("%s\n", sql_row[4]);

								break;
							}
							sprintf_s(query, "UPDATE test SET line2 = 0 WHERE playerName1 = ""'%s'", name);
							//printf("업데이트 %s \n", query);
							query_stat = mysql_query(connection, query);
							if (query_stat != 0)
							{
								fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
								return 1;
							}

						}
					}
				}
				cleanMap(map);
				start = clock();
				system("cls");
				logo();
				gotoxy(15, 15);
				printf("Game Over!!!");
				while (1) {
					end = clock();
					if ((double)(end - start) >= 3000) {
						break;
					}
				}
				sprintf_s(query, "DELETE FROM test WHERE playerName2 = ""'%s'", name);							//지우기
				printf("딜리트 %s \n", query);
				query_stat = mysql_query(connection, query);
				if (query_stat != 0)
				{
					fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
					return 1;
				}

				goto menu;
				break;
		}
		
		break;

		case 0:
			difficulty = 1000 - selectD(10, 20) * 230;
			system("cls");								//여기서 부터 테트리스
			logo();
			start = clock();
			printMap(pmap);
			while (1) {
				period = difficulty;
				if (_kbhit()) {
					key = _getch();
					switch (key) {
					case DOWN:
						period = 40;
						break;

					case SPACEBAR: if (moveBlock(3, map, sq,multior) == 2) {
						break;
					}
								   printMap(map); break;

					case LEFT: moveBlock(2, map, sq,multior); printMap(map); break;
					case RIGHT: moveBlock(1, map, sq,multior); printMap(map); break;
					case ENTER: addLine(map); break;
					}
					if (key == UP) {
						turnBlock(map, sq);

					}

				}


				end = clock();
				if ((double)(end - start) >= period) {
					start = clock();
					if (moveBlock(3, map, sq,multior) == 2) {
						printf("return2");
						break;
					}
				}
			}
			goto menu;
			break;
	}

	
}

void gotoxy(int x, int y) {
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void printMap(int(*pmap)[12]) {

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 12; j++) {
			if (pmap[i][j] != 0) {
				gotoxy(j * 2 + 20, i + 2 + 6);
				color(i, j, pmap);
				printf(/*ANSI_COLOR_RED*/  "■");
			}
			else {
				gotoxy(j * 2 + 20, i + 2 + 6);
				color(i, j, pmap);
				printf("□");
			}
		}
	}
	//gotoxy(45, 14);
	//printf("Next Block");
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			if (next[i][j] != 0) {
				
				gotoxy(j * 2 + 44, i + 8);
				colornext(i, j, next);
				printf("■");
			}
			else {
				
				gotoxy(j * 2 + 44, i + 8);
				colornext(i, j, next);
				printf("□");
			}
		}
	}
	printf("\n");
	
}

void cleanMap(int(*pmap)[12]) {

	for (int i = 1; i < 20; i++) {
		for (int j = 1; j < 11; j++) {
			pmap[i][j] = 0;
		}
	}
}

int moveBlock(int dir, int(*pmap)[12], int(*psq)[4][4][4],int lineinsert) {

	int x = 0, y = 0;
	int fail = 1;
	switch (dir) {
	case 1: x = test.x + 1; y = test.y; break;
	case 2: x = test.x - 1; y = test.y; break;
	case 3: y = test.y + 1; x = test.x; break;
	}
	blockClean(pmap, psq, test.x, test.y);
	if (moveCheck(pmap, psq, x, y) == 1) {
		//printf("pre %d %d", test.x, test.y);

		test.prx = test.x; test.x = x;
		test.pry = test.y; test.y = y;

		blockClean(pmap, psq, test.prx, test.pry);
		block(pmap, psq, test.x, test.y);
		int fail = 0;
	}
	else if (moveCheck(pmap, psq, x, y) == 0) {
		block(pmap, psq, test.x, test.y);
		if (dir == 3) {
			if (test.y <= -2) {
				printf("goingth return2\n");
				return 2;
			}
			srand(time(NULL));
			int random = rand() % 7;
			test.prx = X;
			test.pry = Y;
			test.x = X;
			test.y = Y;
			test.shape = test.nextShape;
			test.nextShape = random;
			period -= 10;
			if (lineinsert == 1)
				lineMulti(pmap);
			else
				lineSingle(pmap);
			return 0;
		}
	}


	printMap(pmap);
	return 1;
}

void block(int(*pmap)[12], int(*psq)[4][4][4], int x, int y) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pmap[i + y][j + x] == 0 && psq[test.shape][test.turn][i][j] == 1) {
				pmap[i + y][j + x] = test.shape + 1;
				//colormap[i + y][j + x] = test.shape + 1;

			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			next[i + 1][j] = 0;
			//colormap[i + y][j + x] = test.shape + 1;


		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (next[i + 1][j] == 0 && psq[test.nextShape][0][i][j] == 1) {
				next[i + 1][j] = test.nextShape + 1;
				//colormap[i + y][j + x] = test.shape + 1;

			}
		}
	}
}

void deleteLine(int l, int(*pmap)[12]) {
	//Beep(1046.502, 30);
	//Beep(1174.659, 30);
	//Beep(1318.510, 40);
	for (int i = l; i >= 1; i--) {
		for (int j = 1; j < 11; j++) {
			pmap[i][j] = pmap[i - 1][j];
		}
	}
	for (int i = 1; i < 11; i++) {
		pmap[0][i] = 0;
	}
}
void addLine(int(*pmap)[12]) {
	int thereisone = 0;
	int start = 0;
	//Beep(1046.502, 30);
	//Beep(1174.659, 30);
	//Beep(1318.510, 40);
	for (int i = 1; i < 20; i++) {
		for (int j = 1; j < 11; j++) {
			if (pmap[i][j] != 0) {
				thereisone = 1;
			}
		}
		if (thereisone == 0) {
			start = i;
		}
		thereisone = 0;
	}
	for (int i = start; i < 20; i++) {
		for (int j = 1; j < 11; j++) {
			pmap[i][j] = pmap[i + 1][j];
		}
	}
	for (int i = 1; i < 11; i++) {
		pmap[19][i] = 1;
	}
	pmap[19][(rand() % 10) + 1] = 0;
}

void lineMulti(int(*pmap)[12]) {
	int check = 0;


	for (int i = 19; i >= 0; i--) {
		for (int j = 1; j < 11; j++) {
			if (pmap[i][j] != 0) {
				check += 1;
			}
		}
		if (check == 10) {
			deleteLine(i, pmap);
			lineInt++;
			if (mynumber == 1) {
				sprintf_s(query, "UPDATE test SET line1 = ""%d"" WHERE playerName1 = ""'%s'", lineInt, name);
				//printf("업데이트 %s \n", query);
				query_stat = mysql_query(connection, query);
				if (query_stat != 0)
				{
					fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
				}
			}
			else {
				sprintf_s(query, "UPDATE test SET line2 = ""%d"" WHERE playerName2 = ""'%s'", lineInt, name);
				//printf("업데이트 %s \n", query);
				query_stat = mysql_query(connection, query);
				if (query_stat != 0)
				{
					fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
				}
			}

			i += 1;
		}
		check = 0;
	}

}
void lineSingle(int(*pmap)[12]) {
	int check = 0;


	for (int i = 19; i >= 0; i--) {
		for (int j = 1; j < 11; j++) {
			if (pmap[i][j] != 0) {
				check += 1;
			}
		}
		if (check == 10) {
			deleteLine(i, pmap);
			lineInt++;

			i += 1;
		}
		check = 0;
	}
}

int moveCheck(int(*pmap)[12], int(*psq)[4][4][4], int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pmap[i + y][j + x] != 0 && psq[test.shape][test.turn][i][j] == 1) {
				return 0;
			}
		}
	}
	return 1;
}

void blockClean(int(*pmap)[12], int(*psq)[4][4][4], int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pmap[i + y][j + x] != 0 && psq[test.shape][test.turn][i][j] == 1) {
				pmap[i + y][j + x] = 0;
				//colormap[i + y][j + x] = 0;
			}
		}
	}
}

int turnBlock(int(*pmap)[12], int(*psq)[4][4][4]) {
	//int fail = 1;
	blockClean(pmap, psq, test.x, test.y);
	test.turn += 1;
	if (test.turn >= 4) {
		test.turn = 0;
	}
	if (moveCheck(pmap, psq, test.x, test.y) == 1) {
		//printf("pre %d %d", test.x, test.y);


		//Beep(1174.659, 100);
		block(pmap, psq, test.x, test.y);
		//fail = 0;
	}
	else if (moveCheck(pmap, psq, test.x, test.y) == 0) {
		test.turn -= 1;
		if (test.turn <= 0) {
			test.turn = 3;
		}

		block(pmap, psq, test.x, test.y);
	}


	printMap(pmap);
	return 1;
}



int choose() {
	int right = 0;
	int period;
	time_t start, end;
	char key;


	int arr[10][10] =
	{
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}
	};
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (arr[i][j] == 1) {
				gotoxy(i * 2 + 10, j + 7);
				printf("■");
				gotoxy(i * 2 + 50, j + 7);
				printf("▣");
			}
		}
	}
	

	start = clock();
	while (1) {
		period = 700;
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case LEFT:
				right = 0;
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						if (arr[i][j] == 1) {
							gotoxy(i * 2 + 10, j + 7);
							printf("■");
							gotoxy(i * 2 + 50, j + 7);
							printf("▣");
						}
					}
				}
				break;
			case RIGHT:
				right = 1;
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						if (arr[i][j] == 1) {
							gotoxy(i * 2 + 10, j + 7);
							printf("▣");
							gotoxy(i * 2 + 50, j + 7);
							printf("■");
						}
					}
				}
				break;
			case ENTER:
				return right;

			}


			end = clock();
			if ((double)(end - start) >= period) {
				start = clock();
			}

		}
	}
}

void logo() {
	int arr[6][6][4] = {
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,1,1,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,1,1,0}
		},
		{
			{0,0,0,0},
			{1,1,1,1},
			{1,0,0,0},
			{1,1,1,1},
			{1,0,0,0},
			{1,1,1,1}
		},
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,1,1,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,1,1,0}
		},
		{
			{0,0,0,0},
			{1,1,1,1},
			{1,0,0,1},
			{1,1,1,1},
			{1,0,1,0},
			{1,0,0,1}
		},
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,1,1,0},
			{0,1,1,0},
			{0,1,1,0},
			{1,1,1,1}
		},
		{
			{0,0,0,0},
			{0,1,1,1},
			{1,0,0,0},
			{1,1,1,1},
			{0,0,0,1},
			{1,1,1,0}


		}
	};
	for (int x = 0; x < 6; x++) {
		switch (x)
		{
		case 0:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
			break;
		case 1:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ORANGE);
			break;
		case 2:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
			break;
		case 3:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
			break;
		case 4:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
			break;
		case 5:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TURQUOISE);
			break;
		}
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {

				if (arr[x][i][j] == 1) {
					gotoxy(j * 2 + x * 12, i);
					printf("■");
				}
				else {
					gotoxy(j * 2 + x * 12, i);
					printf("  ");
				}
			}
			//printf("\n");
		}
	}
}
void color(int i, int j, int map[21][12]) {
	switch (map[i][j])
	{
	case 1:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PINK);
		break;
	case 2:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
		break;
	case 3:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ORANGE);
		break;
	case 4:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TURQUOISE);
		break;
	case 5:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		break;
	case 6:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
		break;
	case 7:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
		break;
	case 0:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	case 8:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	}

}
void colornext(int i, int j, int(*map)[5]) {
	switch (map[i][j])
	{
	case 1:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PINK);
		break;
	case 2:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
		break;
	case 3:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ORANGE);
		break;
	case 4:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TURQUOISE);
		break;
	case 5:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		break;
	case 6:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
		break;
	case 7:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
		break;
	case 0:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	case 8:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	}

}

int selectD(int x,int y) {

	int d = 2;
	char key;
	char difficulty[5][11] = { "매우쉬움", "쉬움", "보통", "어려움", "매우어려움" };


	int arr[5][6] =
	{
		{1,1,1,1,1,1},
		{1,0,0,0,0,1},
		{1,0,0,0,0,1},
		{1,0,0,0,0,1},
		{1,1,1,1,1,1}
	};
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			if (arr[i][j] == 1) {
				gotoxy(j * 2 + x, i + y);
				printf("▣");
				gotoxy(j * 2 + x + 13, i + y);
				printf("▣");
				gotoxy(j * 2 + x + 26, i + y);
				printf("■");
				gotoxy(j * 2 + x + 39, i + y);
				printf("▣");
				gotoxy(j * 2 + x + 52, i + y);
				printf("▣");
			}
		}
	}
	gotoxy(x + 2, y + 2);
	printf("%s", difficulty[0]);
	gotoxy(x + 15, y + 2);
	printf("%s", difficulty[1]);
	gotoxy(x + 28, y + 2);
	printf("%s", difficulty[2]);
	gotoxy(x + 41, y + 2);
	printf("%s", difficulty[3]);
	gotoxy(x + 54, y + 2);
	printf("%s", difficulty[4]);
	



	while (1) {
		period = 700;
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case LEFT:
				//system("cls");
				if (d != 0) {
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 6; j++) {
							if (arr[i][j] == 1) {
								gotoxy(j * 2 + x+ d * 13, i + y);
								printf("▣");
								
							}
						}
					}
					d -= 1;
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 6; j++) {
							if (arr[i][j] == 1) {
								gotoxy(j * 2 + x + d * 13, i + y);
								printf("■");

							}
						}
					}
				}
				break;
			case RIGHT:
				//system("cls");
				//printf("%d", d);
				if (d != 4) {
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 6; j++) {
							if (arr[i][j] == 1) {
								gotoxy(j * 2 + x + d * 13, i + y);
								printf("▣");

							}
						}
					}
					d += 1;
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 6; j++) {
							if (arr[i][j] == 1) {
								gotoxy(j * 2 + x + d * 13, i + y);
								printf("■");

							}
						}
					}
				}
				

				break;
			case ENTER:
				return d;

			}
		}
	}
}