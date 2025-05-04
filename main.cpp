#include "global.h"

//Inits
void Initialization () {
	Game_Console_Setting();
	Game_Event_Random_Initialization();
	Battle_Action_Initialization();
	Map_Create_Initialization();
	CreateThread(NULL, 0, State_Input, NULL, 0, 0);
}

//Main
int main() {
	PLAYER player = {10, 10, 3, 0, 0, &map_kitchen, {1, 1}, false}; //HP, MP, ATK, DEF, KEY, MAP, LOCATION(X, Y), GAME_OVER
	char userInput;

	Initialization();
	Game_Start();
	Game_Description();
	
	Map_Print(&player);
	
	Move_Description();
	while (player.GAME_OVER == false) {
		Move_GetUserInput(&userInput);
		Game_Console_Scroll();
		Move_CommandCheck(&userInput, &player);
		Map_Print(&player);
	}
	Map_Clear();
	
	return 0;
}
