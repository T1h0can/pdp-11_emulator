#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include "rw.h"

void do_add();
void do_sub();
void do_mul();
void do_mov();
void do_movb();
void do_inc();	//d++
void do_incb();
void do_dec();	//d--
void do_decb();
void do_sob();
void do_clr();
void do_clrb();
void do_tst();
void do_tstb();
void do_beq();	//Z
void do_bne();	//!Z
void do_bpl();
void do_br();
void do_jsr();
void do_rts();
void do_cmp();
void do_cmpb();
void do_jmp();
void do_halt();
void do_unknown();

#endif // COMMANDS_H_INCLUDED
