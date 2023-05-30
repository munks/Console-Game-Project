#include "global.h"

//Game Elements

void (*Game_Event_Random_Function[4])(PLAYER*);

void Game_Console_Setting () {
	LONG style = GetWindowLong(GetConsoleWindow(), GWL_STYLE);
	
	//Disable Maximize, Sizing
	style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, style);
	
	//Set Game Title
	SetConsoleTitle(GAME_TITLE);
	
	//Set Game Icon
	HMODULE hmodule = LoadLibrary("Kernel32.dll");
	HICON icon = (HICON)MAKEINTRESOURCE(ID_ICON);
	
	typedef DWORD (*SCI)(HICON);
	SCI SetConsoleIcon = (SCI)GetProcAddress(hmodule, "SetConsoleIcon");
	SetConsoleIcon(icon); 
 
	FreeLibrary(hmodule);
}

void Game_Console_Scroll () {
	CONSOLE_SCREEN_BUFFER_INFO scbi;
	SMALL_RECT sr = {0, 0, 0, 0};
	
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &scbi);
	
	sr.Top = scbi.dwCursorPosition.Y - scbi.srWindow.Top;
	sr.Bottom = scbi.dwCursorPosition.Y - scbi.srWindow.Top;
	
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), false, &sr);
}
	
void Game_Start () {
	Print_Slow(GAME_START_1, 100);
	Print_Slow(GAME_START_2, 100);
	
	Get_Global_Input(State_Input_Start); //Get 's' Input
	
	printf(GAME_START_3);
	Sleep(1000);
	Print_Slow(GAME_START_4, 1000);
	puts("\n");
	Print_Slow(GAME_START_5, 100);
}

void Game_Description () {
	Print_Slow(GAME_DESCRIPTION_1, 100);
	Print_Slow(GAME_DESCRIPTION_2, 100);
	Print_Slow(GAME_DESCRIPTION_3, 100);
	Print_Slow(GAME_DESCRIPTION_4, 100);
}

void Game_Event_Random_Treasure_Ring (PLAYER* p) {
	Print_Slow(RANDOM_EVENT_RING_1, 100);
	Print_Slow(RANDOM_EVENT_RING_2, 100);
	Print_Slow(RANDOM_EVENT_RING_3, 10);
	p->ATK += 3;
	
	Printf_Slow(RANDOM_EVENT_RING_4, 100, p->ATK);
}

void Game_Event_Random_Treasure_Armor (PLAYER* p) {
	Print_Slow(RANDOM_EVENT_ARMOR_1, 100);
	Print_Slow(RANDOM_EVENT_ARMOR_2, 100);
	Print_Slow(RANDOM_EVENT_ARMOR_3, 10);
	p->DEF += 3;
	
	Printf_Slow(RANDOM_EVENT_ARMOR_4, 100, p->DEF);
}

void Game_Event_Random_Enemy (PLAYER* p) {
	ENEMY mob = {5, 0, 2, 0}; //HP, MP, ATK, DEF
	int act;
	int result;
	
	Print_Slow(RANDOM_EVENT_ENEMY_1, 100);
	while (true) {
		Battle_GetUserInput(&act);
		if (!Battle_ValidAction(act)) {
			puts(RANDOM_EVENT_ENEMY_2);
			continue;
		}
		result = Battle_PlayerAction[act](p, &mob);
		
		switch (result) {
			case 0: continue; //Nothing Happened
			case 1: 	//Win
			case 3: {	//Run
				if (result == 1) {
					Print_Slow(RANDOM_EVENT_ENEMY_3, 100);
				} else if (result == 3) {
					Print_Slow(RANDOM_EVENT_ENEMY_4, 100);
				}
				Print_Slow(RANDOM_EVENT_ENEMY_5, 100);
				return;
			}
			case 2: { //Lose
				Print_Slow(RANDOM_EVENT_ENEMY_6, 100);
				p->GAME_OVER = true;
				return;
			}
		}
	}
}

void Game_Event_Random_Nothing (PLAYER* p) {
	Print_Slow(RANDOM_EVENT_NOTHING_1, 100);
	Print_Slow(RANDOM_EVENT_NOTHING_2, 100);
}

void Game_Event_Random_Initialization () {
	Game_Event_Random_Function[0] = Game_Event_Random_Treasure_Ring;
	Game_Event_Random_Function[1] = Game_Event_Random_Treasure_Armor;
	Game_Event_Random_Function[2] = Game_Event_Random_Enemy;
	Game_Event_Random_Function[3] = Game_Event_Random_Nothing;
}

void Game_Event_Random (PLAYER* p) {
	srand(time(NULL));
	Game_Event_Random_Function[rand() % 4](p);
	
	map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) = 'x'; //Room Collapsing
}

bool Location_Compare(COORD loc, short x, short y) {
	COORD temp = {x, y};
	
	return memcmp(&loc, &temp, sizeof(COORD)) == 0;
}

void Game_Event_OpenDoor (PLAYER* p) {
	if (p->CURRENT_MAP == &map_kitchen) { //Kitchen to Mainhole
		Map_MoveToRoom(p, &map_mainhole, (COORD){3,0});
	} else if (p->CURRENT_MAP == &map_safe) { //Safe to Mainhole
		Map_MoveToRoom(p, &map_mainhole, (COORD){0,3});
	} else if (p->CURRENT_MAP == &map_bedroom) { //Bedroom to Mainhole
		Map_MoveToRoom(p, &map_mainhole, (COORD){6,3});
	} else if (p->CURRENT_MAP == &map_mainhole) { //Main hole to other rooms
		if (Location_Compare(p->LOCATION, 0, 3)) {
			Map_MoveToRoom(p, &map_safe, (COORD){6,3});
		} else if (Location_Compare(p->LOCATION, 6, 3)) {
			Map_MoveToRoom(p, &map_bedroom, (COORD){0,3});
		} else if (Location_Compare(p->LOCATION, 3, 0)) {
			Map_MoveToRoom(p, &map_kitchen, (COORD){3,6});
		} else if (Location_Compare(p->LOCATION, 3, 6)) {
			Map_MoveToRoom(p, &map_exit, (COORD){3,0});
		}
	} else if (p->CURRENT_MAP == &map_exit) { //Exit to Mainhole/Escape
		if (Location_Compare(p->LOCATION, 3, 0)) {
			Map_MoveToRoom(p, &map_mainhole, (COORD){3,6});
		} else {
			if (p->KEY == c_escapeKeyCount) { // If key == 14 (Door Open)
				Print_Slow(EVENT_DOOR_1, 100);
				Print_Slow(EVENT_DOOR_2, 100);
				p->GAME_OVER = true;
			} else { // key <= 0
				Print_Slow(EVENT_DOOR_FAIL, 100);
			}
		}
	}
}

void Game_Event_FindKey (PLAYER* p) {
	printf(EVENT_KEY);
	p->KEY++;
	map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) = '0';
}

void Game_Event_Default () {
	printf(EVENT_DEFAULT);
}