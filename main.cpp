#include "main.h"

//Global Elements
int State_Flag = 0;
unsigned char Global_Input = 0;

void Print_Slow (const char* str, int delay) {
	State_Flag_Off(State_Print_Skip);
	for (int i = 0; i < strlen(str); i++) {
		printf("%c", str[i]);
		if (!State_Flag_Check(State_Print_Skip)) {
			Sleep(delay);
		}
	}
}

void Input_Process (unsigned char input, bool special) {
	switch (input) {
		//Print_Slow Skip
		case '\r': {
			State_Flag_On(State_Print_Skip);
			break;
		}
		//Battle Input Confirm
		case '1':
		case '2':
		case '3': {
			State_Flag_On(State_Input_Battle);
			break;
		}
		//Game Start Input Confirm
		case 's':
		case 'S': {
			State_Flag_On(State_Input_Start);
			break;
		}
		//move Input Confirm
		case 'l':
		case 'L':
		case 'r':
		case 'R':
		case 'u':
		case 'U':
		case 'd':
		case 'D': {
			State_Flag_On(State_Input_Move);
			break;
		}
	}
}

DWORD State_Input (PVOID param) {
	bool special;
	
	while (true) {
		special = false;
		Global_Input = _getch();
		if (Global_Input == 0x00 || Global_Input == 0xE0) {
			special = true;
			Global_Input = _getch();
		}
		Input_Process(Global_Input, special);
	}
	return 0;
}

//Battle Elements
int (*Battle_PlayerAction[3])(PLAYER*, ENEMY*);

int Battle_ResultCalculation (PLAYER* p, ENEMY* e) {
	if (e->HP <= 0) {
		return 1; //Win
	} else if (p->HP <= 0) {
		return 2; //Lose
	}
	
	return 0; //Nothing Happened
}

int Battle_PlayerAction_Attack (PLAYER* p, ENEMY* e) {
	//Player Attack
	Print_Slow(BATTLE_ATTACK_1, 100);
	e->HP -= p->ATK - e->DEF; //Enemy HP = Player ATK - Enemy DEF
	Printf_Slow(BATTLE_ATTACK_2, 100, e->HP >= 0 ? e->HP : 0);
	
	if (e->HP <= 0) {
		puts("");
		return Battle_ResultCalculation(p, e);
	}
	
	//Enemy Attack
	Print_Slow(BATTLE_ATTACK_ENEMY_1, 100);
	p->HP -= e->ATK - p->DEF; //Player HP = Enemy ATK - Player DEF
	Printf_Slow(BATTLE_ATTACK_ENEMY_2, 100, p->HP >= 0 ? p->HP : 0);
	puts("");
	return Battle_ResultCalculation(p, e);
}

int Battle_PlayerAction_Heal (PLAYER* p, ENEMY* e) {
	if (p->MP >= 3) {
		//Player Heal (Cost 3)
		Print_Slow(BATTLE_HEAL_1, 100);
		p->HP += 5;
		p->MP -= 3;
		
		Printf_Slow(BATTLE_HEAL_2, 100, p->HP);
		Printf_Slow(BATTLE_HEAL_3, 100, p->MP);
		
		puts("");
		//Enemy Attack
		Print_Slow(BATTLE_ATTACK_ENEMY_1, 100);
		
		p->HP -= e->ATK - p->DEF;
		Printf_Slow(BATTLE_ATTACK_ENEMY_2, 100, p->HP);
		return Battle_ResultCalculation(p, e);
	} else {
		//Not Enought Cost
		Print_Slow(BATTLE_HEAL_FAIL_1, 100);
		Printf_Slow(BATTLE_HEAL_FAIL_2, 100, p->MP);
	}
	return 0;
}

int Battle_PlayerAction_Run (PLAYER* p, ENEMY* e) {
	//Random Dice
	return 3;
}

void Battle_Action_Initialization () {
	Battle_PlayerAction[0] = Battle_PlayerAction_Attack;
	Battle_PlayerAction[1] = Battle_PlayerAction_Heal;
	Battle_PlayerAction[2] = Battle_PlayerAction_Run;
}

void Battle_GetUserInput (int* act) {
	Print_Slow(BATTLE_INPUT_1, 10); //Execution is 0, 1, 2
	
	Get_Global_Input(State_Input_Battle);
	Set_Global_Input(act, int);
	
	*act -= 0x31; //Command('1','2','3') To Act(0,1,2)
}

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

void Game_Event_Random (PLAYER* p, MAP* map) {
	srand(time(NULL));
	Game_Event_Random_Function[rand() % 4](p);
	
	map(map, p->LOCATION.X, p->LOCATION.Y) = 'x'; //Room Collapsing
}

void Game_Event_OpenDoor (PLAYER* p) {
	if (p->KEY > 0) { // If key > 0 (Door Open)
		Print_Slow(EVENT_DOOR_1, 100);
		Print_Slow(EVENT_DOOR_2, 100);
		p->GAME_OVER = true;
	} else { // key <= 0
		Print_Slow(EVENT_DOOR_FAIL, 100);
	}
}

void Game_Event_FindKey (PLAYER* p, MAP* map) {
	Print_Slow(EVENT_KEY, 100);
	p->KEY++;
	map(map, p->LOCATION.X, p->LOCATION.Y) = '0';
}

void Game_Event_Default () {
	Print_Slow(EVENT_DEFAULT, 100);
}

//Map Elements
void Map_RevealNearbyTile (COORD p_loc, MAP* map) {
	short x = p_loc.X;
	short y = p_loc.Y;
	
	reveal(map, x, y) = true;
	if (x - 1 >= 0) {
		reveal(map, x - 1, y) = true;
	}
	if (y - 1 >= 0) {
		reveal(map, x, y - 1) = true;
	}
	if (x + 1 < map->bound_x) {
		reveal(map, x + 1, y) = true;
	}
	if (y + 1 < map->bound_y) {
		reveal(map, x, y + 1) = true;
	}
}

void Map_PrintCharacter (MAP* map, char type, short x, short y) {
	WORD color = FOREGROUND_INTENSITY;
	
	switch (type) {
		case 'x': {
			color |= FOREGROUND_RED;
			break;
		}
		case '0': {
			color |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		}
		case '?': {
			color |= FOREGROUND_RED | FOREGROUND_GREEN;
			break;
		}
		case 'P': {
			color |= FOREGROUND_GREEN;
			break;
		}
		case '@':
		case 'k': {
			color |= FOREGROUND_BLUE;
			break;
		}
	}
	
	if (x == 0) {
		color |= COMMON_LVB_GRID_LVERTICAL;
	}
	if (y == 0) {
		color |= COMMON_LVB_UNDERSCORE;
	}
	if (y == map->bound_y - 1) {
		color |= COMMON_LVB_GRID_HORIZONTAL;
	}
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	printf("%c", type);
	
	color &= ~COMMON_LVB_GRID_LVERTICAL;
	if (x == map->bound_x - 1) {
		color |= COMMON_LVB_GRID_RVERTICAL;
	}
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	printf(" ");
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Map_Print (COORD p_loc, MAP* map) {
	char printChar = ' ';
	Map_RevealNearbyTile(p_loc, map);
	for (short y = (map->bound_y - 1); y >= 0; y--) {
		for (short x = 0; x <= (map->bound_x - 1); x++) {
			if (p_loc.X == x && p_loc.Y == y) {
				printChar = 'P';
			} else {
				printChar = reveal(map, x, y) ? map(map, x, y) : ' ';
			}
			Map_PrintCharacter(map, printChar, x, y);
		}
		puts("");
	}
	puts("");
}

void Map_EventExecute (PLAYER *p, MAP* map) {
	if (map(map, p->LOCATION.X, p->LOCATION.Y) == '?') {
		Game_Event_Random(p, map);
		return;
	}
	if (map(map, p->LOCATION.X, p->LOCATION.Y) == '@') {
		Game_Event_OpenDoor(p);
		return;
	}
	if (map(map, p->LOCATION.X, p->LOCATION.Y) == 'K') {
		Game_Event_FindKey(p, map);
		return;
	}
	
	Game_Event_Default();
}

MAP Map_Create (int params, ...) {
	MAP map_rtn;
	char** param_map;
	
	short x, y;
	char** map;
	bool** reveal;
	
	x = (short)params; //Bound X
	y = (short)*(&params + (sizeof(size_t) / sizeof(int))); //Bound Y
	map = (char**)malloc(sizeof(char*) * y); //Map Data (Y)
	reveal = (bool**)malloc(sizeof(bool*) * y); //Reveal Data (Y)
	
	param_map = (char**)*(((size_t*)&params) + 2);
	for (int i = 0; i < x; i++) { //Map Data (X)
		map[i] = (char*)malloc(sizeof(char) * x);
		reveal[i] = (bool*)calloc(x, sizeof(bool));
	}
	
	for (int i = 0; i <= (y - 1); i++) { //Map Data Copy
		memcpy(map[i], (void*)((char*)param_map+(i*x)), x);
	}
	
	map_rtn.bound_x = x;
	map_rtn.bound_y = y;
	map_rtn.map = map;
	map_rtn.reveal = reveal;
	
	return map_rtn;
}

void Map_Clear (MAP* map) {
	for (int y = 0; y <= (map->bound_y - 1); y++) {
		free(map->map[y]);
		free(map->reveal[y]);
	}
	free(map->map);
	free(map->reveal);
}
//Move Elements
void Move_Description () {
	Print_Slow(MOVE_DESCRIPTION, 100);
}

void Move_GetUserInput (char* output) {
	printf(MOVE_INPUT);
	Get_Global_Input(State_Input_Move);
	Set_Global_Input(output, char);
}

bool Move_LocationValid (COORD check_loc, MAP* map) {
	if (!Move_LocationInBoundary(check_loc.X, check_loc.Y, map->bound_x, map->bound_y)) {
		Print_Slow(MOVE_VALID_1, 100);
		return false;
	}
	
	if (map(map, check_loc.X, check_loc.Y) == 'x') {
		Print_Slow(MOVE_VALID_2, 100);
		return false;
	}

	return true;
}

void Move_CommandCheck (char* cmd, PLAYER* p, MAP* map) {
	COORD temp_loc = p->LOCATION;
	
	if (Move_Direction(cmd, 'u', 'U')) { //Up
		temp_loc.Y++;
	} else if (Move_Direction(cmd, 'd', 'D')) { //Down
		temp_loc.Y--;
	} else if (Move_Direction(cmd, 'l', 'L')) { //Left
		temp_loc.X--;
	} else if (Move_Direction(cmd, 'r', 'R')) { //Right
		temp_loc.X++;
	} else {
		puts(MOVE_CHECK);
		return;
	}
	
	if (!Move_LocationValid(temp_loc, map)) {
		return;
	}
	
	p->LOCATION = temp_loc; //Overwrite Player Location From Temp Location
	
	Map_EventExecute(p, map);
}

//Inits
void Initialization () {
	Game_Console_Setting();
	Game_Event_Random_Initialization();
	Battle_Action_Initialization();
	CreateThread(NULL, 0, State_Input, NULL, 0, 0);
}

//Main
int main() {
	MAP map;
	PLAYER player = {10, 10, 3, 0, 0, {0, 0}, false}; //HP, MP, ATK, DEF, KEY, LOCATION(X, Y), GAME_OVER
	char userInput;

	Initialization();
	Map_Create_Macro((&map), 5, 5, {{'x', '0', '0', '0', '0'},
															{'?', '0', 'x', 'x', '@'},
															{'x', '0', '0', 'K', 'x'},
															{'x', '0', 'x', 'x', '?'},
															{'0', '0', '0', '0', '0'}});
	Game_Start();
	Game_Description();
	
	Map_Print(player.LOCATION, &map);
	
	Move_Description();
	while (player.GAME_OVER == false) {
		Move_GetUserInput(&userInput);
		Game_Console_Scroll();
		Move_CommandCheck(&userInput, &player, &map);
		Map_Print(player.LOCATION, &map);
	}
	Map_Clear(&map);
	
	system("pause");
	return 0;
}
