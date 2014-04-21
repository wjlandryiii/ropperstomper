/*
 *	Copyright 2014 Joseph Landry
 *
 *
 */



#ifndef INSTRUCTIONS_X86
#define INSTRUCTIONS_X86

typedef enum instruction_type {
	INS_RET,
	INS_POP,
	INS_MOV,
	INS_ALTER,
	INS_LOAD,
	INS_STORE,
	INS_FIX_ESP,
	INS_CALL,
	INS_DEBUG
} instruction_type_t;

typedef struct instruction {
	int type;
	int length;
	char *code;
	char *str;
} instruction_t;

extern instruction_t instruction_list_x86_ret[];
extern int ret_count;
extern instruction_t instruction_list_x86[];
extern int inst_count;
#endif
