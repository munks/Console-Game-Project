#ifndef _global
	#define _global
	
	//Definition
	#define ID_ICON 1
	
	//Include Standard
	#include <stdio.h>
	#include <conio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <Windows.h>
	
	//Global Macro
	#define State_Flag_On(s)		(State_Flag |= s) //void
	#define State_Flag_Off(s)		(State_Flag &= ~s) //void
	#define State_Flag_Switch(s)	(State_Flag *= s) //void
	#define State_Flag_Check(s)		(State_Flag & s) //bool
	
	#define State_Print_Skip	(1 << 0)
	#define State_Input_Start	(1 << 1)
	#define State_Input_Move	(1 << 2)
	#define State_Input_Battle	(1 << 3)
	
	#define Get_Global_Input(s) State_Flag_Off(s); while (!State_Flag_Check(s)) { Sleep(1); }
	#define Set_Global_Input(v, t) (*v) = (t)Global_Input
	
	//Structure
	typedef struct PLAYER {	//Player Data
		int HP;
		int MP;
		int ATK;
		int DEF;
		int KEY;
		COORD LOCATION;
		bool GAME_OVER;
	} PLAYER;

	typedef struct ENEMY {	//Enemy Data
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
	
	//Include Custom
	#include "text.h"
	#include "main.h"
	#include "io.h"
	#include "battle.h"
	#include "game.h"
	#include "map.h"
	#include "move.h"
#endif