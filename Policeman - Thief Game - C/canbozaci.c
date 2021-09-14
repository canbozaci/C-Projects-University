/***** INCLUDES *****/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
/***** DEFINES *****/
#define MANHATTAN_DISTANCE_LIMIT 16 

#define GAME_MODE_HARD 1
#define GAME_MODE_EASY 0

#define GAME_STATE_WIN 1
#define GAME_STATE_LOST 0
#define GAME_STATE_CONTINUE -1

#define PLAYER_TYPE_THIEF 'T'
#define PLAYER_TYPE_POLICE 'P'

#define UP_DIRECTION_LOWER 'w'
#define UP_DIRECTION_UPPER 'W'

#define DOWN_DIRECTION_LOWER 's'
#define DOWN_DIRECTION_UPPER 'S'

#define LEFT_DIRECTION_LOWER 'a'
#define LEFT_DIRECTION_UPPER 'A'

#define RIGHT_DIRECTION_LOWER 'd'
#define RIGHT_DIRECTION_UPPER 'D'

#define INVALID_MOVE 0
#define VALID_MOVE 1

#define WALL 'w'
#define ESCAPE_POINT 'E'

/***** GLOBAL VARIABLES *****/
char *Description = "Press 'W' to move up\nPress 'S' to move down\nPress 'A' to move left\nPress 'D' to move right\nGame Logic : Reach Escape Point which is marked as E before Police Officer which is marked as P catchs you!\nPress any key to when ever you are ready to play\n\0"; // \0 kullaným amacý : printf fonksiyonu null terminator bir fonksiyonu olduðu için stringin null ile termine edilmesini saðladýk.
char mazeGraph[16][16];

int escape_point_x = 15; // çýkýþ noktasýnýn x koordinatý 
int escape_point_y = 7; // çýkýþ noktasýnýn y koordinatý

int thief_position_x; // hýrsýzýn labirent üzerindeki x koordinatý
int thief_position_y; // hýrsýzýn labirent üzerindeki y koordinatý

int police_position_x; // polisin labirent üzerindeki x koordinatý
int police_position_y; // polisin labirent üzerindeki y koordinatý

int game_mode; // oyunun kolay mý zor mu olduðunu belirler.

int game_state; // oyunun durumu hakkýnda bilgi verir.

int  mazeWalls[22][2] = {{2,2},{2,3},{8,0},{8,1}
						,{13,2},{13,3},{6,5},{7,5}
						,{8,5},{6,6},{3,9},{4,9}
						,{5,9},{5,10},{10,8},{10,9}
						,{10,10},{10,11},{11,11},{2,14}
						,{3,14},{4,14}}; // duvarlarýn labirent içerisindeki koordinatýný tutar.
						

/***** FUNCTIONS DECLARATIONS *****/
void fillMazeGraph();
int initialPosition(); // polis ve hýrsýz için
int manhattanDistance(int x1, int y1,int x2,int y2); // baþlangýç pozisyonlarýnýn seçimi için verilmiþ kural
void modeSelection(); // oyunun zor mu yoksa kolay mý olacaðýnýn seçimi.
int collisionDetection(int *xPosition, int *yPosition, char direction,char playerType); // Eðer ilerlenecek yerde duvar var ise uyarý mekanizmasý
void movePlayer(char playerType); // polis ve hýrsýz için tahta üzerinde ilerleme
void Render(); //  labirentin dýþ hatlarýný çizdirip olumlu ya da olumsuz mesaj dönen fonksiyon

/***** MAIN FUNCTION *****/
int main(void)
{
	StartLabel:
		game_state = GAME_STATE_CONTINUE;
		
		int roundCount = 0;
		
		modeSelection();
		
		printf("%s",Description);
		getch();
		system("cls");
		
		initialPosition();
		
		fillMazeGraph();
		
		while(game_state == GAME_STATE_CONTINUE)
		{
			roundCount++;
			printf("\t\t ***** ROUND : %d *****\n\t\t Game Mode : %s\n",roundCount,game_mode ? "Hard Mode" : "Easy Mode");
			
			Render();
			
			if((roundCount - 1) % 2 == 0)
			{
				movePlayer(PLAYER_TYPE_THIEF);
				system("cls");
				sleep(1);
			}
			else
			{
				movePlayer(PLAYER_TYPE_POLICE);
				sleep(1);
				system("cls");
			}
			
			if (police_position_x == thief_position_x && police_position_y == thief_position_y)
			{
				game_state = GAME_STATE_LOST;
				system("cls");
				printf("\t\tYOU LOST AT ROUND %d\n",roundCount);
			}
			
			if (escape_point_x == thief_position_x && escape_point_y == thief_position_y)
			{
				game_state = GAME_STATE_WIN;
				system("cls");
				printf("\t\tYOU WON AT ROUND %d\n",roundCount);
			}
			
			if(game_state == GAME_STATE_WIN || game_state ==  GAME_STATE_LOST)
			{
				int answer;
				printf("Do you wanna try again to play this game ? Y/N\n");
				while(1)
				{
					answer = getch();
					
					if(answer != 'y' && answer != 'Y' && answer != 'N' && answer != 'n')
					{
						printf("Answer it as Y for Yes or N for no\n");
					}
					else if(answer == 'y' || answer == 'Y')
					{
						mazeGraph[thief_position_x][thief_position_y] = ' ';
						mazeGraph[police_position_x][police_position_y] = ' ';
						mazeGraph[escape_point_x][escape_point_y] = ESCAPE_POINT;
						game_state = GAME_STATE_CONTINUE;
						goto StartLabel;
						break;
						
					}
					else if(answer == 'n' || answer == 'N')
					{
						break;
					}
				}
			}
		}
	getch();
	return 0;
}

/***** FUNCTION DEFINITIONS *****/

void fillMazeGraph()
{
	int x, y,wall;
	
	for(x = 0; x < 16; x++)
	{
		for(y = 0; y < 16; y++)
		{
			if(x == 0 || x == 15 || y == 0 || y== 15 )
			{
				mazeGraph[x][y] = '*';
			}
		}
	}
	
	for(wall = 0; wall < 22; wall++)
	{
			mazeGraph[mazeWalls[wall][0]][mazeWalls[wall][1]] = WALL;
			
	}
	
	mazeGraph[escape_point_x][escape_point_y] = ESCAPE_POINT;
}

int manhattanDistance(int x1, int y1,int x2,int y2)
{
	int x = abs(x2 - x1);
	int y = abs(y2 - y1);
	return x+y;
}

int initialPosition()
{
	int state = 1;
	int distanceBetweenEscapePoint;
	srand(time(0));
	while(state == 1)
	{
		/* Hýrsýzýn pozisyonu ve kurala uygunluðu*/
		
		thief_position_x = rand() % 16;
		thief_position_y = rand() % 16;
		
		distanceBetweenEscapePoint = manhattanDistance(thief_position_x,thief_position_y,escape_point_x,escape_point_y);
		
		if(distanceBetweenEscapePoint < MANHATTAN_DISTANCE_LIMIT)
		{
			continue;
		}
		
		/* Polisin pozisyonu ve kurala uygunluðu*/
		
		police_position_x = rand() % 16;
		police_position_y = rand() % 16;
		
		distanceBetweenEscapePoint = manhattanDistance(police_position_x,police_position_y,escape_point_x,escape_point_y);
		
		if(distanceBetweenEscapePoint < MANHATTAN_DISTANCE_LIMIT)
		{
			continue;
		}

		/* Hýrsýz ve polisin uzaklýk kuralýna uygunluðu*/
		
		distanceBetweenEscapePoint = manhattanDistance(police_position_x,police_position_y,thief_position_x,thief_position_y);
		
		if(distanceBetweenEscapePoint < MANHATTAN_DISTANCE_LIMIT)
		{
			continue;
		}
		else
		{
			state = 0;  // Eðer herþey kurallara uygunsa baþlangýç pozisyonlarýmýz belirlenmiþ oldu ve döngüden çýkabiliriz.
		}
 }
}
void modeSelection()
{
	char selection;
	
	do
	{
		printf("Please select game mode you want to play\n\t1.Easy Mode\n\t2.Hard Mode\n");
		scanf("%c",&selection);
		// oyuncunun hatalý seçim yapmasý durumunu engellemek için
		if(selection != '1' && selection != '2')
		{
			system("cls");
			printf("You made a bad selection\nPlease try again as descripted.\n");
		}
		
	}while(selection != '1' && selection != '2');
	
	if(selection == '1')
	{
		game_mode = GAME_MODE_EASY;
	} 
	else
	{
		game_mode = GAME_MODE_HARD;
	}
	
	system("cls");
}
int collisionDetection(int *xPosition, int *yPosition, char direction,char playerType)
{
	int next_x = *xPosition;
	int next_y = *yPosition;
	int returnValue = VALID_MOVE;
	
	switch(direction)
	{
		case UP_DIRECTION_LOWER:	next_y--; 
									break;
		case UP_DIRECTION_UPPER:	next_y--; 
									break;
		case DOWN_DIRECTION_LOWER:	next_y++;
									break;
		case DOWN_DIRECTION_UPPER:	next_y++;
									break;
		case LEFT_DIRECTION_LOWER:	next_x--;
									break;
		case LEFT_DIRECTION_UPPER:	next_x--;
									break;
		case RIGHT_DIRECTION_LOWER:	next_x++;
									break;
		case RIGHT_DIRECTION_UPPER:	next_x++;
									break;
		default:					returnValue = INVALID_MOVE;
									break;
	}
	
	if(returnValue == VALID_MOVE){
		if ((next_x < 0 || next_x > 15) || (next_y < 0 || next_y > 15)) // labirent sýnýrlarýndan taþma kontrolü
		{
			returnValue = INVALID_MOVE;
		}
		else 
		{
			int wallSquare, axis;
			returnValue = VALID_MOVE;
			for(wallSquare = 0; wallSquare < 22; wallSquare++) // duvarlarý ezip geçme kontrolü
			{
				axis = 0;
				if(next_x == mazeWalls[wallSquare][axis] && next_y == mazeWalls[wallSquare][axis+1])
				{
					if(playerType == PLAYER_TYPE_THIEF)
						printf("Oops! It seems you did not see the wall!\n you : %d %d wall : %d %d\n ",next_x,next_y,mazeWalls[wallSquare][0],mazeWalls[wallSquare][1]);
					returnValue = INVALID_MOVE;
					break;
				}	
			}	
		}
		
		if(returnValue == VALID_MOVE)
		{
				mazeGraph[*xPosition][*yPosition] = ' ';
				*xPosition = next_x;
				*yPosition = next_y;
		}
	}
	return returnValue;
}

void movePlayer(char playerType)
{
	char direction;
	int isCollideWith = INVALID_MOVE;
	int trials[4]= {0,0,0,0};
	/** Easy Mode **/
	int easyDirection;
	/** Hard Mode **/
	char directions[4] = {UP_DIRECTION_UPPER,RIGHT_DIRECTION_UPPER,DOWN_DIRECTION_UPPER,LEFT_DIRECTION_LOWER};
	int closest = 255;
	int trial;
	int indexOfBestChoice;
	
	if(playerType == PLAYER_TYPE_THIEF)
	{
		printf("Make your move\n");
		
		while(isCollideWith == INVALID_MOVE)
		{
			printf("x : %d y: %d\n",thief_position_x,thief_position_y);
			scanf(" %c",&direction);
			isCollideWith = collisionDetection(&thief_position_x,&thief_position_y,direction,playerType);
			if(isCollideWith == INVALID_MOVE) printf("Please don't violette rules!\n%s",Description);
		}		
	}
	else if(game_mode == GAME_MODE_EASY)
	{
		srand(time(0));
		
		while(!isCollideWith)
		{
			easyDirection = rand() % 4;
			
			switch(easyDirection)
			{
				case 0:		direction = UP_DIRECTION_UPPER;
							break;
				case 1:		direction = RIGHT_DIRECTION_UPPER;
							break;
				case 2:		direction = DOWN_DIRECTION_UPPER;
							break;
				case 3:		direction = LEFT_DIRECTION_LOWER;
							break;
				default:	break;
			}
			
			if(trials[easyDirection] == 0) // ayný sayý gelmesi durumunda denemeyi engellemek için yapýlan mekanizma
			{
				trials[easyDirection] = 1;
			}
			else
			{
				continue;
			}
		
			isCollideWith = collisionDetection(&police_position_x,&police_position_y,direction,playerType);
		}
	}
	else
	{
		int i;
		
		for(i = 0; i < 4; i++)
		{
			int tempXPolice = police_position_x, tempYPolice = police_position_y;
			trials[i] = collisionDetection(&tempXPolice,&tempYPolice,directions[i],playerType);
		}
		
		for(i = 0; i < 4; i++)
		{
			if(trials[i] == 1)
			{
				int tempXPolice = police_position_x, tempYPolice = police_position_y;
				switch(directions[i])
				{
					case UP_DIRECTION_UPPER: 	tempYPolice--;
												break;
					case RIGHT_DIRECTION_UPPER: tempXPolice++;
												break;
					case DOWN_DIRECTION_UPPER: 	tempYPolice++;
							break;
					case LEFT_DIRECTION_LOWER: 	tempXPolice--;
												break;
					default:					break;
				
				}
				
				trial = manhattanDistance(tempXPolice,tempYPolice,thief_position_x,thief_position_y);
				
				if(trial < closest)
				{
					closest = trial;
					indexOfBestChoice = i;
				}
			}
		}
		
		isCollideWith = collisionDetection(&police_position_x,&police_position_y,directions[indexOfBestChoice],playerType);
	}
}

void Render()
{
	int xAxis, yAxis;
	mazeGraph[thief_position_x][thief_position_y] = PLAYER_TYPE_THIEF;
	mazeGraph[police_position_x][police_position_y] = PLAYER_TYPE_POLICE;
	for(yAxis = 0; yAxis < 48; yAxis++)
	{
		for(xAxis = 0; xAxis < 48; xAxis++)
		{
			if(yAxis == 0 || xAxis == 0 || yAxis == 47 || xAxis == 47)
			{
				printf("* ");
			}
			else if((xAxis % 3 == 1) && (yAxis % 3 == 1))
			{
					switch(mazeGraph[(int)(xAxis/3)][(int)(yAxis/3)])
					{
						case PLAYER_TYPE_THIEF:		printf("%c ",PLAYER_TYPE_THIEF);
													break;
						case PLAYER_TYPE_POLICE:	printf("%c ",PLAYER_TYPE_POLICE);
													break;
						case ESCAPE_POINT:			printf("%c ",ESCAPE_POINT);
													break;
						case WALL:					printf("%c ",WALL);
													break;
						default:					printf("  ");
													break;
					}
			}
			else if((xAxis % 3 == 2) && (yAxis % 3 == 2))
			{
					printf("* ");
			}	
			else
			{
				printf("  ");
			}
		}
		printf("\n");	
	}
		
}
