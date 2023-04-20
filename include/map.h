#ifndef _map
	#define _map
	
	//Map Elements
	#define map(m, x, y)					m->map[(y - (m->bound_y - 1)) * -1][x]		//Represent 'map' data as a coordinate plane
	#define reveal(m, x, y)					m->reveal[(y - (m->bound_y - 1)) * -1][x]	//Represent 'reveal' data as a coordinate plane
	#define Map_Create_Macro(m, x, y, ...) {char tempMap[y][x] = __VA_ARGS__; \
											*m = Map_Create(x, y, tempMap); }	//'Map_Create' Function Simplification
																				//(MAP*, bound_x, bound_y, map data)
	
	void Map_Print (COORD p_loc, MAP* map);								//Print Map (COORD{X, Y})
	void Map_PrintCharacter (MAP* map, char type, short x, short y);	//Print Map (Internal)
	void Map_RevealNearbyTile (COORD p_loc, MAP* map);					//Reveal Nearby Tiles
	
	void Map_EventExecute (PLAYER *p, MAP* map);						//Read Map And Execute Event
	
	MAP Map_Create (int params, ...);									//Create Map (bound x, bound y, map tiles, map reveals,...)
	void Map_Clear (MAP* map);											//Clean Map Memory Allocation
#endif