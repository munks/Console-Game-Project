#include "global.h"

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