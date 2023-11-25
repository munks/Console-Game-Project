#pragma once

//Move Elements
void Move_Description ();									//Print Moving Tooltip
void Move_GetUserInput (char* output);						//Get User Input For Moving (up, down, left, right)
bool Move_LocationValid (COORD check_loc);					//Check Location Is Right Place
void Move_CommandCheck (char* cmd, PLAYER* p);				//Check User Input And Decide Moving