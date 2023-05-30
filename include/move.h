#pragma once

//Move Elements
#define Move_Direction(cmd, us, ls) ((*cmd == us) || (*cmd == ls)) //Check Direction (up, down, left, right)
#define Move_LocationInBoundary(loc_x, loc_y, bound_x, bound_y) (loc_x >= 0 && loc_x <= (bound_x - 1) && loc_y >= 0 && loc_y <= (bound_y - 1))
																//Check Location Is In Boundary

void Move_Description ();									//Print Moving Tooltip
void Move_GetUserInput (char* output);						//Get User Input For Moving (up, down, left, right)
bool Move_LocationValid (COORD check_loc);					//Check Location Is Right Place
void Move_CommandCheck (char* cmd, PLAYER* p);				//Check User Input And Decide Moving