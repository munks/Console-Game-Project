#include "global.h"

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