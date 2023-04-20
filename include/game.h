#ifndef _game
	#define _game
	
	//Game Elements
	extern void (*Game_Event_Random_Function[4])(PLAYER*);
	
	void Game_Console_Setting ();		//Set Console Property
	void Game_Console_Scroll ();		//Console Scrolling
	void Game_Start ();					//Game Start with 's' Command
	void Game_Description ();			//Print Game's Description
	
	void Game_Event_Random (PLAYER* p, MAP* map);		//Decide Game Random Event - map '?'
	void Game_Event_Random_Treasure_Ring (PLAYER* p);	//Game Random Event (Tresure Ring: ATK+3)
	void Game_Event_Random_Treasure_Armor (PLAYER* p);	//Game Random Event (Tresre Armor: DEF+3)
	void Game_Event_Random_Enemy (PLAYER* p);			//Game Random Event (Encount Emeny - Battle)
	void Game_Event_Random_Nothing (PLAYER* p);			//Game Random Event (Nothing)
	
	void Game_Event_Random_Initialization ();			//Inits 'Game_RandomEvent_Function' Variable
	
	void Game_Event_OpenDoor (PLAYER* p);				//Open Door Event - map '@'
	void Game_Event_FindKey (PLAYER* p, MAP* map);		//Find Key Event - map 'k'
	void Game_Event_Default ();							//Default Event - map -'0'
#endif