#include "global.h"

//Battle Elements

int (*Battle_PlayerAction[3])(PLAYER*, ENEMY*);

int Battle_ResultCalculation (PLAYER* p, ENEMY* e) {
	if (e->HP <= 0) {
		return 1; //Win
	} else if (p->HP <= 0) {
		return 2; //Lose
	}
	
	return 0; //Nothing Happened
}

int Battle_PlayerAction_Attack (PLAYER* p, ENEMY* e) {
	//Player Attack
	Print_Slow(BATTLE_ATTACK_1, 100);
	e->HP -= p->ATK - e->DEF; //Enemy HP = Player ATK - Enemy DEF
	Printf_Slow(BATTLE_ATTACK_2, 100, e->HP >= 0 ? e->HP : 0);
	
	if (e->HP <= 0) {
		puts("");
		return Battle_ResultCalculation(p, e);
	}
	
	//Enemy Attack
	Print_Slow(BATTLE_ATTACK_ENEMY_1, 100);
	p->HP -= e->ATK - p->DEF; //Player HP = Enemy ATK - Player DEF
	Printf_Slow(BATTLE_ATTACK_ENEMY_2, 100, p->HP >= 0 ? p->HP : 0);
	puts("");
	return Battle_ResultCalculation(p, e);
}

int Battle_PlayerAction_Heal (PLAYER* p, ENEMY* e) {
	if (p->MP >= 3) {
		//Player Heal (Cost 3)
		Print_Slow(BATTLE_HEAL_1, 100);
		p->HP += 5;
		p->MP -= 3;
		
		Printf_Slow(BATTLE_HEAL_2, 100, p->HP);
		Printf_Slow(BATTLE_HEAL_3, 100, p->MP);
		
		puts("");
		//Enemy Attack
		Print_Slow(BATTLE_ATTACK_ENEMY_1, 100);
		
		p->HP -= e->ATK - p->DEF;
		Printf_Slow(BATTLE_ATTACK_ENEMY_2, 100, p->HP);
		return Battle_ResultCalculation(p, e);
	} else {
		//Not Enought Cost
		Print_Slow(BATTLE_HEAL_FAIL_1, 100);
		Printf_Slow(BATTLE_HEAL_FAIL_2, 100, p->MP);
	}
	return 0;
}

int Battle_PlayerAction_Run (PLAYER* p, ENEMY* e) {
	//Random Dice
	return 3;
}

void Battle_Action_Initialization () {
	Battle_PlayerAction[0] = Battle_PlayerAction_Attack;
	Battle_PlayerAction[1] = Battle_PlayerAction_Heal;
	Battle_PlayerAction[2] = Battle_PlayerAction_Run;
}

void Battle_GetUserInput (int* act) {
	Print_Slow(BATTLE_INPUT_1, 10); //Execution is 0, 1, 2
	
	Wait_Global_Input(State_Input_Battle);
	Get_Global_Input(act, int);
	
	*act -= 0x31; //Command('1','2','3') To Act(0,1,2)
}