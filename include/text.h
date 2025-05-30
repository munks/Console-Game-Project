#pragma once
	
#define GAME_TITLE "Dungeon"

//void Game_Start (const char* cmd);
#define GAME_START_1				"You woke up in the strange kitchen.\n"
#define GAME_START_2				"Enter the \"s\" to search this room.\n"
#define GAME_START_3				"Searching"
#define GAME_START_4				"..."
#define GAME_START_5				"You found the map that placed on the wall.\n\n"

//void Game_Description ();
#define GAME_DESCRIPTION_1			"Your location is P.\n"
#define GAME_DESCRIPTION_2			"? is random encounter.\n"
#define GAME_DESCRIPTION_3			"@ is exit.\n\n"
#define GAME_DESCRIPTION_4			"To escape from dungeon, you need Key. Key can be obtained from K location in the map.\n\n"

//void Move_Description ();
#define MOVE_DESCRIPTION			"You can move in four direction. (W: up, A: left, S: down, D: right)\n"

//void Move_GetUserInput (char* output);
#define MOVE_INPUT					"Enter the direction.\n"

//void Move_CommandCheck (char* cmd, PLAYER* p);
#define MOVE_CHECK					"Invalid direction."

//bool Move_LocationValid (COORD check_loc, MAP* map);
#define MOVE_VALID_1				"You cannot go through the fourth wall.\n\n"
#define MOVE_VALID_2				"You tried to go to the direction, but wall blocked you.\n\n"

//void Game_Event_Random_Treasure_Ring (PLAYER* p);
#define RANDOM_EVENT_RING_1			"You found treasure box!\n"
#define RANDOM_EVENT_RING_2			"It contains ring of brave!\n"
#define RANDOM_EVENT_RING_3			"ATK +3!\n"
#define RANDOM_EVENT_RING_4			"Your ATK is now %d.\n\n" //%d(p->ATK)

//void Game_Event_Random_Treasure_Armor (PLAYER* p);
#define RANDOM_EVENT_ARMOR_1		"You found treasure box!\n"
#define RANDOM_EVENT_ARMOR_2		"It contains armor!\n"
#define RANDOM_EVENT_ARMOR_3		"DEF +3!\n"
#define RANDOM_EVENT_ARMOR_4		"Your DEF is now %d.\n\n" //%d(p->DEF)

//void Game_Event_Random_Enemy (PLAYER* p);
#define RANDOM_EVENT_ENEMY_1		"You encountered enemy! Enter the number of your action.\n"
#define RANDOM_EVENT_ENEMY_2		"Invalid action.\n"
#define RANDOM_EVENT_ENEMY_3		"YOU WIN!\n"
#define RANDOM_EVENT_ENEMY_4		"You escaped from that place.\n"
#define RANDOM_EVENT_ENEMY_5		"You left the place. And the place collapsed.\n\n"
#define RANDOM_EVENT_ENEMY_6		"YOU LOSE\n\n"

//void Game_Event_Random_Nothing (PLAYER* p);
#define RANDOM_EVENT_NOTHING_1		"There was nothing in the place.\n"
#define RANDOM_EVENT_NOTHING_2		"You left the place. And the place collapsed.\n\n"

//void Battle_GetUserInput (int* act);
#define BATTLE_INPUT_1				"1: Attack, 2: Heal[Cost MP: 3], 3: Run\n"

//int Battle_PlayerAction_Attack (PLAYER* p, ENEMY* e);
#define BATTLE_ATTACK_1				"You attack the goblin!\n"
#define BATTLE_ATTACK_2				"The mob's HP is now %d.\n" //%d(e->HP >= 0 ? e->HP : 0)
#define BATTLE_ATTACK_ENEMY_1		"The goblin attack back!\n"
#define BATTLE_ATTACK_ENEMY_2		"The player's HP is now %d.\n" //%d(p->HP >= 0 ? p->HP : 0)

//int Battle_PlayerAction_Heal (PLAYER* p, ENEMY* e);
#define BATTLE_HEAL_1				"You cast heal!\n"
#define BATTLE_HEAL_2				"Your HP is now %d.\n" //%d(p->HP)
#define BATTLE_HEAL_3				"Your MP is now %d.\n" //%d(p->MP)
#define BATTLE_HEAL_FAIL_1			"You don't have enoguh MP.\n"
#define BATTLE_HEAL_FAIL_2			"Your MP is %d.\n" //%d(p->MP)

//void Game_Event_OpenExit (PLAYER* p);
#define EVENT_EXIT_1				"The key just fit to the door keyhole!\n"
#define EVENT_EXIT_2				"You escape from that dungeon!\n\n"
#define EVENT_EXIT_FAIL				"You don't have all keys. so you cannot get out from here!\n\n"

//void Game_Event_FindKey (PLAYER* p);
#define EVENT_KEY					"You found the key!\n\n"

//void Game_Event_Default ();
#define EVENT_DEFAULT				"You walk to another place.\n\n"

//void Map_MoveToRoom (PLAYER* p, MAP* map, COORD loc);
#define MAP_DOOR_FAIL				"The door is locked.\n\n"
#define MAP_MOVE					"You moved to %s.\n\n"