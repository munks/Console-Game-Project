#include "global.h"

MAP map_kitchen;
MAP map_mainhole;
MAP map_safe;
MAP map_bedroom;
MAP map_exit;

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

void Map_Print (PLAYER* p) {
	char printChar = ' ';
	MAP* map = p->CURRENT_MAP;
	
	Map_RevealNearbyTile(p->LOCATION, p->CURRENT_MAP);
	printf("%s\n", map->map_name);
	for (short y = (map->bound_y - 1); y >= 0; y--) {
		for (short x = 0; x <= (map->bound_x - 1); x++) {
			if (p->LOCATION.X == x && p->LOCATION.Y == y) {
				printChar = 'P';
			} else {
				printChar = reveal(map, x, y) ? map(map, x, y) : ' ';
			}
			Map_PrintCharacter(map, printChar, x, y);
		}
		if (y == map->bound_y - 1) {
			printf("    Keys %d/%d\n", p->KEY, c_escapeKeyCount);
		} else {
			puts("");
		}
	}
	puts("");
}

void Map_EventExecute (PLAYER *p) {
	if (map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) == '?') {
		Game_Event_Random(p);
		return;
	}
	if (map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) == '@') {
		Game_Event_OpenDoor(p);
		return;
	}
	if (map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) == 'K') {
		Game_Event_FindKey(p);
		return;
	}
	
	Game_Event_Default();
}

MAP Map_Create_Internal (int params, ...) {
	MAP map_rtn;
	char** param_map;
	
	short x, y;
	char** map;
	bool** reveal;
	char* name_t;
	
	x = (short)params; //Bound X
	y = (short)*(&params + (sizeof(size_t) / sizeof(int))); //Bound Y
	map = (char**)malloc(sizeof(char*) * y); //Map Data (Y)
	reveal = (bool**)malloc(sizeof(bool*) * y); //Reveal Data (Y)
	
	param_map = (char**)*(((size_t*)&params) + 3);
	for (int i = 0; i < x; i++) { //Map Data (X)
		map[i] = (char*)malloc(sizeof(char) * x);
		reveal[i] = (bool*)calloc(x, sizeof(bool));
	}
	
	for (int i = 0; i <= (y - 1); i++) { //Map Data Copy
		memcpy(map[i], (void*)((char*)param_map+(i*x)), x);
	}
	
	name_t = (char*)malloc(sizeof(char) * strlen((char*)*(((size_t*)&params) + 2)));
	strcpy(name_t, (char*)*(((size_t*)&params) + 2));
	
	map_rtn.bound_x = x;
	map_rtn.bound_y = y;
	map_rtn.map = map;
	map_rtn.reveal = reveal;
	map_rtn.map_name = name_t;
	
	return map_rtn;
}

void Map_Clear_Internal (MAP* map) {
	for (int y = 0; y <= (map->bound_y - 1); y++) {
		free(map->map[y]);
		free(map->reveal[y]);
	}
	free(map->map);
	free(map->reveal);
	free(map->map_name);
}

void Map_Clear () {
	Map_Clear_Internal(&map_kitchen);
	Map_Clear_Internal(&map_mainhole);
	Map_Clear_Internal(&map_safe);
	Map_Clear_Internal(&map_bedroom);
	Map_Clear_Internal(&map_exit);
}

void Map_MoveToRoom (PLAYER* p, MAP* map, COORD loc) {
	printf(MAP_MOVE, map->map_name);
	p->CURRENT_MAP = map;
	p->LOCATION = loc;
}

void Map_Create_Initialization () {
	Map_Create((&map_kitchen), 7, 7, "Kitchen",
									 {{'x','x','x','@','x','x','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','0','0','K','0','0','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','x','x','x','x','x','x'}});
	Map_Create((&map_mainhole), 7, 7, "Main Hole",
									  {{'x','x','x','@','x','x','x'},
									   {'x','0','0','0','x','K','x'},
									   {'x','0','x','x','x','0','x'},
									   {'@','0','0','0','0','0','@'},
									   {'x','x','0','x','x','0','x'},
									   {'x','K','0','0','x','0','x'},
									   {'x','x','x','@','x','x','x'}});
	Map_Create((&map_safe), 7, 7, "Safe Room",
								  {{'x','x','x','x','x','x','x'},
								   {'x','K','0','0','0','0','x'},
								   {'x','K','0','0','0','0','x'},
								   {'x','K','0','0','0','0','@'},
								   {'x','K','0','0','0','0','x'},
								   {'x','K','0','0','0','0','x'},
								   {'x','x','x','x','x','x','x'}});
	Map_Create((&map_bedroom), 7, 7, "Bedroom",
									 {{'x','x','x','x','x','x','x'},
									  {'x','K','x','K','x','K','x'},
									  {'x','0','x','0','x','0','x'},
									  {'@','0','0','0','0','0','x'},
									  {'x','0','x','0','x','0','x'},
									  {'x','K','x','K','x','K','x'},
									  {'x','x','x','x','x','x','x'}});
	Map_Create((&map_exit), 7, 7, "Exit Room",
								  {{'x','x','x','x','x','x','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','0','0','@','0','0','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','x','x','@','x','x','x'}});
}