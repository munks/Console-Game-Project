#pragma once

//Map Elements
#define map(m, x, y)					m->map[(y - (m->bound_y - 1)) * -1][x]		//Represent 'map' data as a coordinate plane
#define reveal(m, x, y)					m->reveal[(y - (m->bound_y - 1)) * -1][x]	//Represent 'reveal' data as a coordinate plane
#define Map_Create(m, x, y, n, ...) {char tempMap[y][x] = __VA_ARGS__; \
										*m = Map_Create_Internal(x, y, n, tempMap); }	//'Map_Create' Function Simplification
																					//(MAP*, bound_x, bound_y, map data)

void Map_Print (PLAYER* p);											//Print Map (COORD{X, Y})

void Map_EventExecute (PLAYER *p);									//Read Map And Execute Event
void Map_MoveToRoom (PLAYER* p, MAP* map, COORD loc);				//Set Player Map Data

void Map_Clear ();													//Clean Map Memory Allocation
void Map_Create_Initialization ();									//Map Initialization

//MAP Variables
extern MAP map_kitchen;
extern MAP map_mainhole;
extern MAP map_safe;
extern MAP map_bedroom;
extern MAP map_exit;
