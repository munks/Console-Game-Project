#include "global.h"

MAP map_kitchen;
MAP map_mainhole;
MAP map_safe;
MAP map_bedroom;
MAP map_exit;
MOVEINFO* move_info;
int move_info_cnt;

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
	if (x + 1 < map->bound.X) {
		reveal(map, x + 1, y) = true;
	}
	if (y + 1 < map->bound.Y) {
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
	if (y == map->bound.Y - 1) {
		color |= COMMON_LVB_GRID_HORIZONTAL;
	}
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	printf("%c", type);
	
	color &= ~COMMON_LVB_GRID_LVERTICAL;
	if (x == map->bound.X - 1) {
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
	for (short y = (map->bound.Y - 1); y >= 0; y--) {
		for (short x = 0; x <= (map->bound.X - 1); x++) {
			if (p->LOCATION.X == x && p->LOCATION.Y == y) {
				printChar = 'P';
			} else {
				printChar = reveal(map, x, y) ? map(map, x, y) : ' ';
				if ((printChar >= 0) && (printChar <= 31)) {
					printChar = '@';
				}
			}
			Map_PrintCharacter(map, printChar, x, y);
		}
		if (y == map->bound.Y - 1) {
			printf("    Keys %d/%d\n", p->KEY, c_escapeKeyCount);
		} else if (y == map->bound.Y - 2) {
			printf("    HP: %d, MP: %d\n", p->HP, p->MP);
		} else if (y == map->bound.Y - 3) {
			printf("    ATK: %d, DEF: %d\n", p->ATK, p->DEF);
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
	if ((map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) >= 0) &&
		(map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) <= 31)) {
		Map_FindConnectedRoom(p, map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y));
	}
	if (map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) == '@') {
		Game_Event_OpenExit(p);
		return;
	}
	if (map(p->CURRENT_MAP, p->LOCATION.X, p->LOCATION.Y) == 'K') {
		Game_Event_FindKey(p);
		return;
	}
	
	Game_Event_Default();
}

void Map_FindConnectedRoom (PLAYER *p, char door) {
	int i;
	int idx;
	MAP* map = NULL;
	COORD dest;
	for (i = 0; i < move_info_cnt; i++) {
		if (move_info[i].door == door) {
			idx = p->CURRENT_MAP == move_info[i].connect[0] ? 1 : 0;
			map = move_info[i].connect[idx];
			dest = move_info[i].dest[idx];
			break;
		}
	}
	printf("Map: %s, Target: %s\n", p->CURRENT_MAP->map_name, map->map_name);
	Map_MoveToRoom(p, map, dest);
}

MAP Map_Create_Internal (int params, ...) {
	MAP map_rtn;
	char** param_map;
	
	short x, y;
	char** map;
	bool** reveal;
	char* name_t;
	
	int k;
	
	x = (short)params; //Bound X
	y = (short)*(&params + (sizeof(size_t) / sizeof(int))); //Bound Y
	map = (char**)malloc(sizeof(char*) * y); //Map Data (Y)
	reveal = (bool**)malloc(sizeof(bool*) * y); //Reveal Data (Y)
	
	param_map = (char**)*(((size_t*)&params) + 3);
	for (int i = 0; i < x; i++) { //Map Data (X)
		map[i] = (char*)malloc(sizeof(char) * x);
		reveal[i] = (bool*)calloc(x, sizeof(bool));
	}
	
	for (int i = 0; i < y; i++) { //Map Data Copy
		memcpy(map[i], (void*)((char*)param_map+(i*x)), x);
	}
	
	name_t = (char*)malloc(sizeof(char) * strlen((char*)*(((size_t*)&params) + 2)));
	strcpy(name_t, (char*)*(((size_t*)&params) + 2));
	
	map_rtn.bound.X = x;
	map_rtn.bound.Y = y;
	map_rtn.map = map;
	map_rtn.reveal = reveal;
	map_rtn.map_name = name_t;
	
	return map_rtn;
}

void Map_Clear_Internal (MAP* map) {
	for (int y = 0; y <= (map->bound.Y - 1); y++) {
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
	if (!map) {
		printf(MAP_DOOR_FAIL);
		return;
	}
	printf(MAP_MOVE, map->map_name);
	p->CURRENT_MAP = map;
	p->LOCATION = loc;
}

void Map_DoorConnectionCheck (MAP* map) {
	int idx;
	int k;
	
	for (int i = 0; i < map->bound.X; i++) { //Door Connection Check
		for (int j = 0; j < map->bound.Y; j++) {
			if ((map(map, i, j) >= 0) && (map(map, i, j) <= 31)) {
				for (k = 0; k < move_info_cnt; k++) {
					if (move_info[k].door == map(map, i, j)) {
						break;
					}
				}
				if (move_info_cnt <= k) {
					move_info = (MOVEINFO*)realloc(move_info, sizeof(MOVEINFO) * (k + 1));
					move_info_cnt++;
					
					memset(&(move_info[k]), 0, sizeof(MOVEINFO));
					move_info[k].door = map(map, i, j);
					idx = 0;
				} else {
					idx = 1;
				}
				move_info[k].connect[idx] = map;
				move_info[k].dest[idx] = (COORD){i, j};
			}
		}
	}
}

void Map_Create_Initialization () { //Map Must be over 3x3 size
	move_info_cnt = 0;
	move_info = (MOVEINFO*)malloc(sizeof(MOVEINFO) * 1);
	
	Map_Create((&map_kitchen), 7, 7, "Kitchen",
									 {{'x','x','x', 0 ,'x','x','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','0','0','K','0','0','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','0','0','0','0','0','x'},
									  {'x','x','x','x','x','x','x'}});
	Map_DoorConnectionCheck(&map_kitchen);
	Map_Create((&map_mainhole), 7, 7, "Main Hole",
									  {{'x','x','x', 3 ,'x','x','x'},
									   {'x','0','0','0','x','K','x'},
									   {'x','0','x','x','x','0','x'},
									   { 1 ,'0','0','?','0','0', 2 },
									   {'x','x','0','x','x','0','x'},
									   {'x','K','0','0','x','0','x'},
									   {'x','x','x', 0 ,'x','x','x'}});
	Map_DoorConnectionCheck(&map_mainhole);
	Map_Create((&map_safe), 7, 7, "Safe Room",
								  {{'x','x','x','x','x','x','x'},
								   {'x','K','0','0','0','0','x'},
								   {'x','K','0','0','0','0','x'},
								   {'x','K','0','0','0','0', 1 },
								   {'x','K','0','0','0','0','x'},
								   {'x','K','0','0','0','0','x'},
								   {'x','x','x','x','x','x','x'}});
	Map_DoorConnectionCheck(&map_safe);
	Map_Create((&map_bedroom), 7, 7, "Bedroom",
									 {{'x','x','x','x','x','x','x'},
									  {'x','K','x','K','x','K','x'},
									  {'x','0','x','0','x','0','x'},
									  { 2 ,'0','0','0','0','0','x'},
									  {'x','0','x','0','x','0','x'},
									  {'x','K','x','K','x','K','x'},
									  {'x','x','x','x','x','x','x'}});
	Map_DoorConnectionCheck(&map_bedroom);
	Map_Create((&map_exit), 7, 7, "Exit Room",
								  {{'x','x','x','x','x','x','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','0','0','@','0','0','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','0','0','0','0','0','x'},
								   {'x','x','x', 3 ,'x','x','x'}});
	Map_DoorConnectionCheck(&map_exit);
	for (int k = 0; k < move_info_cnt; k++) {
		printf("MOVEINFO %d: MAP - %s, %s/ DEST - (%d, %d), (%d, %d)\n", k, move_info[k].connect[0]->map_name, move_info[k].connect[1]->map_name, move_info[k].dest[0].X, move_info[k].dest[0].Y, move_info[k].dest[1].X, move_info[k].dest[1].Y);
	}
}