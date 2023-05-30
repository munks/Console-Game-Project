#pragma once

//IO Elements

extern int State_Flag;
extern unsigned char Global_Input;

void Print_Slow (const char* str, int delay); //Print 'str' one character at each 'delay' millisecond
#define Printf_Slow(s, d, ...) {char *tempStr = (char*)calloc(strlen(s) + 20, sizeof(char)); \
								sprintf(tempStr, s, __VA_ARGS__); \
								Print_Slow(tempStr, d); \
								free(tempStr); }
void Input_Process (unsigned char input, bool special); //Check input and Process condition
DWORD State_Input (PVOID param);						//Get Input Thread