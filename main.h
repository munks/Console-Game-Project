#ifndef _main
	#define _main
	
	//Definition
	#define GAME_TITLE "Dungeon"
	#define ID_ICON 1
	
	//Include Standard
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <Windows.h>
	
	//Global Elements
	#define map(m, x, y) m->map[(y - (m->bound_y - 1)) * -1][x] //Represent 'map' data as a coordinate plane
	#define reveal(m, x, y) m->reveal[(y - (m->bound_y - 1)) * -1][x] //Represent 'reveal' data as a coordinate plane
	
	//Structure
	typedef struct PLAYER { //Player Data
		int HP;
		int MP;
		int ATK;
		int DEF;
		int KEY;
		COORD LOCATION;
		bool GAME_OVER;
	} PLAYER;

	typedef struct ENEMY { //Enemy Data
		int ATK;
		int MP;
		int HP;
		int DEF;
	} ENEMY;
	
	typedef struct MAP { //Map Data
		char** map;
		bool** reveal;
		short bound_x;
		short bound_y;
	} MAP;
	
	void Print_Slow (const char* str, int delay); //Print 'str' one character at each 'delay' millisecond
	
	//Battle Elements
	#define Battle_ValidAction(act) (act >= 0 && act <= 2) //Check if 'act' is within the correct bounds (0~2)
	
	int Battle_ResultCalculation (PLAYER* p, ENEMY* e); //Returns Battles result (0: Nothing Happened, 1: Win, 2: Lose)
	int Battle_PlayerAction_Attack (PLAYER* p, ENEMY* e); //Battle Action (1: Attack)
	int Battle_PlayerAction_Heal (PLAYER* p, ENEMY* e); //Battle Action (2: Heal)
	int Battle_PlayerAction_Run (PLAYER* p, ENEMY* e); //Battle Action (3: Run)
	
	void Battle_Action_Initialization (); //Inits 'Battle_PlayerAction' Variable
	
	void Battle_GetUserInput (int* act); //Get User Input For Battle (1~3)
	
	//Game Elements
	void Game_Console_Setting (); //Set Console Property
	void Game_Start (const char* cmd); //Game Start with 'cmd' Command
	void Game_Description (); //Print Game's Description
	
	void Game_Event_Random (PLAYER* p, MAP* map); //Decide Game Random Event - map '?'
	void Game_Event_Random_Treasure_Ring (PLAYER* p); //Game Random Event (Tresure Ring: ATK+3)
	void Game_Event_Random_Treasure_Armor (PLAYER* p); //Game Random Event (Tresre Armor: DEF+3)
	void Game_Event_Random_Enemy (PLAYER* p); //Game Random Event (Encount Emeny - Battle)
	void Game_Event_Random_Nothing (PLAYER* p); //Game Random Event (Nothing)
	
	void Game_RandomEvent_Initialization (); //Inits 'Game_RandomEvent_Function' Variable
	
	void Game_Event_OpenDoor (PLAYER* p); //Open Door Event - map '@'
	void Game_Event_FindKey (PLAYER* p, MAP* map); //Find Key Event - map 'k'
	void Game_Event_Default (); //Default Event - map -'0'
	
	//Map Elements
	#define Map_Create_Macro(m, x, y, ...) {char tempMap[y][x] = __VA_ARGS__; \
																	*m = Map_Create(x, y, tempMap); } //'Map_Create' Function Simplification
																																//(MAP*, bound_x, bound_y, map data)
	
	void Map_Print (COORD p_loc, MAP* map); //Print Map (COORD{X, Y})
	void Map_PrintCharacter (MAP* map, char type, short x, short y); //Print Map (Internal)
	void Map_RevealNearbyTile (COORD p_loc, MAP* map); //Reveal Nearby Tiles
	
	void Map_EventExecute (PLAYER *p, MAP* map); //Read Map And Execute Event
	
	MAP Map_Create (int params, ...); //Create Map (bound x, bound y, map tiles, map reveals,...)
	void Map_Clear (MAP* map); //Clean Map Memory Allocation
	
	//Move Elements
	#define Move_Direction(cmd, dir) (strcmp(cmd, dir) == 0) //Check Direction (up, down, left, right)
	#define Move_LocationInBoundary(loc_x, loc_y, bound_x, bound_y) (loc_x >= 0 && loc_x <= (bound_x - 1) && loc_y >= 0 && loc_y <= (bound_y - 1)) //Check Location Is In Boundary
	
	void Move_Description (); //Print Moving Tooltip
	void Move_GetUserInput (char* output); //Get User Input For Moving (up, down, left, right)
	bool Move_LocationValid (COORD check_loc); //Check Location Is Right Place
	void Move_CommandCheck (char* cmd, PLAYER* p, MAP* map); //Check User Input And Decide Moving
	
	//Inits
	void Initialization (); //Run 'Game_RandomEvent_Initialization' And 'Battle_Action_Initialization' Function
	
	//Main
	int main (); //Game Starting Point
#endif