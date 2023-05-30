#pragma once

//Battle Elements
extern int (*Battle_PlayerAction[3])(PLAYER*, ENEMY*);
#define Battle_ValidAction(act) (act >= 0 && act <= 2)	//Check if 'act' is within the correct bounds (0~2)

int Battle_ResultCalculation (PLAYER* p, ENEMY* e);		//Returns Battles result (0: Nothing Happened, 1: Win, 2: Lose)
int Battle_PlayerAction_Attack (PLAYER* p, ENEMY* e);	//Battle Action (1: Attack)
int Battle_PlayerAction_Heal (PLAYER* p, ENEMY* e);		//Battle Action (2: Heal)
int Battle_PlayerAction_Run (PLAYER* p, ENEMY* e);		//Battle Action (3: Run)

void Battle_Action_Initialization ();		//Inits 'Battle_PlayerAction' Variable

void Battle_GetUserInput (int* act);		//Get User Input For Battle (1~3)