#include "main.h"

//Global Elements
void Print_Slow (const char* str, int delay) {
	for (int i = 0; i < strlen(str); i++) {
		printf("%c", str[i]);
		Sleep(delay);
	}
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
	Print_Slow("You attack the goblin!\n", 100);
	e->HP -= p->ATK - e->DEF; //Enemy HP = Player ATK - Enemy DEF
	printf("The mob's HP is now %d\n", e->HP);
	
	//Enemy Attack
	Print_Slow("The goblin attack back!\n", 100);
	p->HP -= e->ATK - p->DEF; //Player HP = Enemy ATK - Player DEF
	printf("The player's HP is now %d\n", p->HP);
	
	return Battle_ResultCalculation(p, e);
}

int Battle_PlayerAction_Heal (PLAYER* p, ENEMY* e) {
	if (p->MP >= 3) {
		//Player Heal (Cost 3)
		Print_Slow("You cast heal!\n", 100);
		p->HP += 5;
		p->MP -= 3;
		
		Print_Slow("Your HP is now ", 100); printf("%d\n", p->HP);
		Print_Slow("Your MP is now ", 100); printf("%d\n", p->MP);
		
		puts("");
		//Enemy Attack
		Print_Slow("The goblin attack back!\n", 100);
		
		p->HP -= e->ATK - p->DEF;
		printf("The player's HP is now %d\n", p->HP);
		return Battle_ResultCalculation(p, e);
	} else {
		//Not Enought Cost
		Print_Slow("You don't have enoguh MP\n", 100);
		Print_Slow("Your MP is ", 100); printf("%d\n", p->MP);
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
	char cmd[100];
	
	puts("1: Attack, 2: Heal[Cost MP: 3], 3: Run"); //Execution is 0, 1, 2
	printf("Command: ");
	gets(cmd);
	
	*act = atoi(cmd) - 1; //String Command To Integer
}

//Game Elements
void (*Game_Event_Random_Function[4])(PLAYER*);

void Game_Start (const char* cmd) {
	char answer[100];
	
	Print_Slow("you wake up in the strange kitchen\n", 100);
	Print_Slow("enter the \"", 100); Print_Slow(cmd, 100); Print_Slow("\" to search this room\n", 100);
	
	while (true) {
		gets(answer);
		strlwr(answer);
		if (strcmp(answer, cmd) == 0) {
			puts("searching...\n");
			Sleep(3000);
			Print_Slow("You found the map that placed on the wall\n\n", 100);
			return;
		}
	}
}

void Game_Description () {
	Print_Slow("your location is P\n", 100);
	Print_Slow("? is random encounter\n", 100);
	Print_Slow("@ is exit\n\n", 100);
	Print_Slow("to escape from dungeon you need key and the key is came from k location in the map\n\n", 100);
}

void Game_Event_Random_Treasure_Ring (PLAYER* p) {
	Print_Slow("You found treasure box!\n", 100);
	Print_Slow("It contains ring of brave!\n", 100);
	Print_Slow("ATK +3!\n", 10);
	p->ATK += 3;
	
	printf("Your ATK is now %d\n", p->ATK);
}

void Game_Event_Random_Treasure_Armor (PLAYER* p) {
	Print_Slow("You found treasure box!\n", 100);
	Print_Slow("It contains armor!\n", 100);
	Print_Slow("DEF +3!\n", 10);
	p->DEF += 3;
	
	printf("Your DEF is now %d\n", p->DEF);
}

void Game_Event_Random_Enemy (PLAYER* p) {
	ENEMY mob = {5, 0, 2, 0}; //HP, MP, ATK, DEF
	int act;
	int result;
	
	Print_Slow("you encountered enemy! enter the number of your action\n", 100);
	while (true) {
		Battle_GetUserInput(&act);
		if (!Battle_ValidAction(act)) {
			puts("Invalid action.\n");
			continue;
		}
		result = Battle_PlayerAction[act](p, &mob);
		
		switch (result) {
			case 0: continue; //Nothing Happened
			case 1: 	//Win
			case 3: {	//Run
				if (result == 1) {
					Print_Slow("YOU WIN!\n", 100);
					p->GAME_OVER = true;
				} else if (result == 3) {
					Print_Slow("you escaped from that room\n", 100);
				}
				Print_Slow("you exit from that room and the room collapse\n", 100);
				return;
			}
			case 2: { //Lose
				Print_Slow("YOU LOSE\n", 100);
				p->GAME_OVER = true;
				return;
			}
		}
	}
}

void Game_Event_Random_Nothing (PLAYER* p) {
	Print_Slow("there was nothing in the room\n", 100);
	Print_Slow("you exit from that room and the room collapse\n", 100);
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
		Print_Slow("The key just fit to the door keyhole!\n", 100);
		Print_Slow("You escape from that dungeon!\n", 100);
		p->GAME_OVER = true;
	} else { // key <= 0
		Print_Slow("you don't have any key so you cannot get out from here!\n", 100);
	}
}

void Game_Event_FindKey (PLAYER* p, MAP* map) {
	puts("You found the key!");
	p->KEY++;
	map(map, p->LOCATION.X, p->LOCATION.Y) = '0';
}

void Game_Event_Default () {
	Print_Slow("You walk to another room.\n", 100);
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
	if (map(map, p->LOCATION.X, p->LOCATION.Y) == 'k') {
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
	Print_Slow("you can move in four direction.\n", 100);
	Sleep(1000);
}

void Move_GetUserInput (char* output) {
	printf("Enter the direction.(up, down, left, right): ");
	gets(output);
}

bool Move_LocationValid (COORD check_loc, MAP* map) {
	if (map(map, check_loc.X, check_loc.Y) == 'x') {
		Print_Slow("you tried to go to the direction but wall blocked you\n", 100);
		return false;
	}
	if (!Move_LocationInBoundary(check_loc.X, check_loc.Y, map->bound_x, map->bound_y)) {
		Print_Slow("you cannot go through the fourth wall\n", 100);
		return false;
	}
	return true;
}

void Move_CommandCheck (char* cmd, PLAYER* p, MAP* map) {
	COORD temp_loc = p->LOCATION;
	
	strlwr(cmd);
	
	if (Move_Direction(cmd, "up")) {
		temp_loc.Y++;
	} else if (Move_Direction(cmd, "down")) {
		temp_loc.Y--;
	} else if (Move_Direction(cmd, "left")) {
		temp_loc.X--;
	} else if (Move_Direction(cmd, "right")) {
		temp_loc.X++;
	} else {
		puts("Invalid direction.");
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
	Game_Event_Random_Initialization();
	Battle_Action_Initialization();
}

//Main
int main() {
	MAP map;
	PLAYER player = {10, 10, 3, 0, 0, {0, 0}, false}; //HP, MP, ATK, DEF, KEY, LOCATION(X, Y), GAME_OVER
	char userInput[100];

	Initialization();
	Map_Create_Macro((&map), 5, 5, {{'x', '0', '0', '0', '0'},
															{'?', '0', 'x', 'x', '@'},
															{'x', '0', '0', 'k', 'x'},
															{'x', '0', 'x', 'x', '?'},
															{'0', '0', '0', '0', '0'}});
	Game_Start("Search");
	Game_Description();
	
	Map_Print(player.LOCATION, &map);
	
	Move_Description();
	while (player.GAME_OVER == false) {
		Move_GetUserInput(userInput);
		Move_CommandCheck(userInput, &player, &map);
		Map_Print(player.LOCATION, &map);
	}
	Map_Clear(&map);
	
	system("pause");
	return 0;
}
