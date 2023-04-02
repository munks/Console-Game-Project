#ifndef _text
	#define _text
	
	#define GAME_TITLE "Dungeon"
	
	//void Game_Start (const char* cmd);
	#define GAME_START_1				"you wake up in the strange kitchen\n"
	#define GAME_START_2				"enter the \"%s\" to search this room\n" //%s(cmd)
	#define GAME_START_3				"searching...\n"
	#define GAME_START_4				"You found the map that placed on the wall\n\n"
	
	//void Game_Description ();
	#define GAME_DESCRIPTION_1			"your location is P\n"
	#define GAME_DESCRIPTION_2			"? is random encounter\n"
	#define GAME_DESCRIPTION_3			"@ is exit\n\n"
	#define GAME_DESCRIPTION_4			"to escape from dungeon you need key and the key is came from k location in the map\n\n"
	
	//void Move_Description ();
	#define MOVE_DESCRIPTION			"you can move in four direction.\n"
	
	//void Move_GetUserInput (char* output);
	#define MOVE_INPUT					"Enter the direction.(up, down, left, right): "
	
	//void Move_CommandCheck (char* cmd, PLAYER* p, MAP* map);
	#define MOVE_CHECK					"Invalid direction."
	
	//bool Move_LocationValid (COORD check_loc, MAP* map);
	#define MOVE_VALID_1				"you tried to go to the direction but wall blocked you\n"
	#define MOVE_VALID_2				"you cannot go through the fourth wall\n"
	
	//void Game_Event_Random_Treasure_Ring (PLAYER* p);
	#define RANDOM_EVENT_RING_1			"You found treasure box!\n"
	#define RANDOM_EVENT_RING_2			"ATK +3!\n"
	#define RANDOM_EVENT_RING_3			"You found treasure box!\n"
	#define RANDOM_EVENT_RING_4			"Your ATK is now %d\n" //%d(p->ATK)
	
	//void Game_Event_Random_Treasure_Armor (PLAYER* p);
	#define RANDOM_EVENT_ARMOR_1		"You found treasure box!\n"
	#define RANDOM_EVENT_ARMOR_2		"It contains armor!\n"
	#define RANDOM_EVENT_ARMOR_3		"DEF +3!\n"
	#define RANDOM_EVENT_ARMOR_4		"Your DEF is now %d\n" //%d(p->DEF)
	
	//void Game_Event_Random_Enemy (PLAYER* p);
	#define RANDOM_EVENT_ENEMY_1		"you encountered enemy! enter the number of your action\n"
	#define RANDOM_EVENT_ENEMY_2		"Invalid action.\n"
	#define RANDOM_EVENT_ENEMY_3		"YOU WIN!\n"
	#define RANDOM_EVENT_ENEMY_4		"you escaped from that room\n"
	#define RANDOM_EVENT_ENEMY_5		"you exit from that room and the room collapse\n"
	#define RANDOM_EVENT_ENEMY_6		"YOU LOSE\n"
	
	//void Game_Event_Random_Nothing (PLAYER* p);
	#define RANDOM_EVENT_NOTHING_1		"there was nothing in the room\n"
	#define RANDOM_EVENT_NOTHING_2		"you exit from that room and the room collapse\n"
	
	//void Battle_GetUserInput (int* act);
	#define BATTLE_INPUT_1				"1: Attack, 2: Heal[Cost MP: 3], 3: Run"
	#define BATTLE_INPUT_2				"Command: "
	
	//int Battle_PlayerAction_Attack (PLAYER* p, ENEMY* e);
	#define BATTLE_ATTACK_1				"You attack the goblin!\n"
	#define BATTLE_ATTACK_2				"The mob's HP is now %d\n" //%d(e->HP >= 0 ? e->HP : 0)
	#define BATTLE_ATTACK_ENEMY_1		"The goblin attack back!\n"
	#define BATTLE_ATTACK_ENEMY_2		"The player's HP is now %d\n" //%d(p->HP >= 0 ? p->HP : 0)
	
	//int Battle_PlayerAction_Heal (PLAYER* p, ENEMY* e);
	#define BATTLE_HEAL_1				"You cast heal!\n"
	#define BATTLE_HEAL_2				"Your HP is now %d\n" //%d(p->HP)
	#define BATTLE_HEAL_3				"Your MP is now %d\n" //%d(p->MP)
	#define BATTLE_HEAL_FAIL_1			"You don't have enoguh MP\n"
	#define BATTLE_HEAL_FAIL_2			"Your MP is %d\n" //%d(p->MP)
	
	//void Game_Event_OpenDoor (PLAYER* p);
	#define EVENT_DOOR_1				"The key just fit to the door keyhole!\n"
	#define EVENT_DOOR_2				"You escape from that dungeon!\n"
	#define EVENT_DOOR_FAIL				"you don't have any key so you cannot get out from here!\n"
	
	//void Game_Event_FindKey (PLAYER* p, MAP* map);
	#define EVENT_KEY					"You found the key!"
	
	//void Game_Event_Default ();
	#define EVENT_DEFAULT				"You walk to another room.\n"
#endif