#include "global.h"

//Move Elements
#define Move_Direction(cmd, us, ls) ((*cmd == us) || (*cmd == ls)) //Check Direction (up, down, left, right)
#define Move_LocationInBoundary(loc_x, loc_y, bound_x, bound_y) (loc_x >= 0 && loc_x <= (bound_x - 1) && loc_y >= 0 && loc_y <= (bound_y - 1))	//Check Location Is In Boundary
																
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
		printf(MOVE_VALID_1);
		return false;
	}
	
	if (map(map, check_loc.X, check_loc.Y) == 'x') {
		printf(MOVE_VALID_2);
		return false;
	}

	return true;
}

void Move_CommandCheck (char* cmd, PLAYER* p) {
	COORD temp_loc = p->LOCATION;
	
	if (Move_Direction(cmd, 'w', 'W')) { //Up
		temp_loc.Y++;
	} else if (Move_Direction(cmd, 's', 'S')) { //Down
		temp_loc.Y--;
	} else if (Move_Direction(cmd, 'a', 'A')) { //Left
		temp_loc.X--;
	} else if (Move_Direction(cmd, 'd', 'D')) { //Right
		temp_loc.X++;
	} else {
		puts(MOVE_CHECK);
		return;
	}
	
	if (!Move_LocationValid(temp_loc, p->CURRENT_MAP)) {
		return;
	}
	
	p->LOCATION = temp_loc; //Overwrite Player Location From Temp Location
	
	Map_EventExecute(p);
}