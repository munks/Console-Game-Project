#include "global.h"

int State_Flag = 0;
unsigned char Global_Input = 0;

void Print_Slow (const char* str, int delay) {
	State_Flag_Off(State_Print_Skip);
	for (int i = 0; i < strlen(str); i++) {
		printf("%c", str[i]);
		if (!State_Flag_Check(State_Print_Skip)) {
			Sleep(delay);
		}
	}
}

void Input_Process (unsigned char input, bool special) {
	switch (input) {
		//Print_Slow Skip
		case '\r': {
			State_Flag_On(State_Print_Skip);
			break;
		}
		//Battle Input Confirm
		case '1':
		case '2':
		case '3': {
			State_Flag_On(State_Input_Battle);
			break;
		}
		//Game Start Input Confirm
		case 's':
		case 'S': {
			if (!State_Flag_Check(State_Input_Start)) {
				State_Flag_On(State_Input_Start);
				break;
			}
		}
		//move Input Confirm
		case 'w':
		case 'W':
		case 'a':
		case 'A':
		case 'd':
		case 'D': {
			State_Flag_On(State_Input_Move);
			break;
		}
	}
}

DWORD State_Input (PVOID param) {
	bool special;
	
	while (true) {
		special = false;
		Global_Input = _getch();
		if (Global_Input == 0x00 || Global_Input == 0xE0) {
			special = true;
			Global_Input = _getch();
		}
		Input_Process(Global_Input, special);
	}
	return 0;
}