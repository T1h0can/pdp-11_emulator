#ifndef PDP_H_INCLUDED
#define PDP_H_INCLUDED

#include "rw.h"
#include "commands.h"

#define NO_PARAM 0
#define HAS_SS 1
#define HAS_DD 2
#define HAS_NN 4
#define HAS_XX 8
#define HAS_RL 16
#define HAS_RR 32

typedef struct _var {
	short val;
	short res;
	adr a;
	//int nn;
} Var;

typedef struct _commands {
	word mask;
	word opcode;
	char * name;
	void (* func) (void);
	word param;
} Commands;

void reg_print();

#endif // PDP_H_INCLUDED
