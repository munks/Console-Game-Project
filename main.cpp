#include "global.h"

//Inits
void Initialization () {
	Game_Console_Setting();
	Game_Event_Random_Initialization();
	Battle_Action_Initialization();
	CreateThread(NULL, 0, State_Input, NULL, 0, 0);
}

//Main
int main() {
	MAP map;
	PLAYER player = {10, 10, 3, 0, 0, {0, 0}, false}; //HP, MP, ATK, DEF, KEY, LOCATION(X, Y), GAME_OVER
	char userInput;

	Initialization();
	Map_Create_Macro((&map), 5, 5, {{'x', '0', '0', '0', '0'},
									{'?', '0', 'x', 'x', '@'},
									{'x', '0', '0', 'K', 'x'},
									{'x', '0', 'x', 'x', '?'},
									{'0', '0', '0', '0', '0'}});
	Game_Start();
	Game_Description();
	
	Map_Print(player.LOCATION, &map);
	
	Move_Description();
	while (player.GAME_OVER == false) {
		Move_GetUserInput(&userInput);
		Game_Console_Scroll();
		Move_CommandCheck(&userInput, &player, &map);
		Map_Print(player.LOCATION, &map);
	}
	Map_Clear(&map);
	
	system("pause");
	return 0;
}
