#include "commands.h"
#include "pdp.h"

extern word nn, r;
extern signed char xx;
extern Var ss, dd;
extern int b;
extern word reg[8];

char nzvc[4];

#define N nzvc[0]
#define Z nzvc[1]
#define V nzvc[2]
#define C nzvc[3]

void setNZ(word w) {
	Z = b ? ((w & 0xFF) == 0) : (w == 0);
	N = b ? (w >> 7) & 1 : (w >> 15) & 1;
}

void NZVC_print() {
	printf("%c", N ? 'n' : '-');
	printf("%c", Z ? 'z' : '-');
	printf("%c", V ? 'v' : '-');
	printf("%c", C ? 'c' : '-');
	printf(" ");
}

void push(short x) {
	SP -= 2;
	w_write(SP , x);
}

short pop() {
	short x;
	x = w_read(SP);
	SP += 2;
	return x;
}

void do_add() {
	dd.res = dd.val + ss.val;
	w_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_sub() {
	dd.res = dd.val - ss.val;
	w_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_mul() {
	dd.res = dd.val * ss.val;
	w_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_mov() {
	if (dd.a == ODATA)
		fprintf(stderr, "%c", ss.val);
	w_write(dd.a, ss.val);
	setNZ(ss.val);
}

void do_movb() {
	if (dd.a == ODATA)
		fprintf(stderr, "%c", ss.val);
	b_write(dd.a, ss.val);
	setNZ(ss.val);
}

void do_inc() {
	dd.res = dd.val + 1;
	w_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_incb() {
	dd.res = dd.val + 1;
	b_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_dec() {
	dd.res = dd.val - 1;
	w_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_decb() {
	dd.res = dd.val - 1;
	b_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_sob() {
	reg[r] --;
	if (reg[r] != 0)
		PC = PC - 2 * nn;
	printf("%06o ", PC);
}

void do_clr() {
	w_write(dd.a, 0);
}

void do_clrb() {
	b_write(dd.a, 0);
}

void do_tst() {
	unsigned int val = w_read(dd.a);
	setNZ(w_read(dd.a));
	printf("dd.a = %06o val = %06o b = %d ss.val = %06o", dd.a, val, b, ss.val);

}

void do_tstb() {
	unsigned int val = b_read(dd.a);
	setNZ(b_read(dd.a));
	printf("dd.a = %06o val = %06o b = %d ss.val = %06o", dd.a, val, b, ss.val);
}

void do_beq() {
	if (Z == 1)
		do_br();
}

void do_bne() {
	if (Z == 0)
		do_br();
}

void do_bpl() {
	if (N == 0)
		do_br();
}

void do_br() {
	PC = PC + 2 * xx;
	printf("%06o ", PC);
}

void do_jsr() {
	push(reg[r]);
	reg[r] = PC;
	PC = dd.a;
}

void do_rts() {
	PC = reg[r];
	reg[r] = pop();
}

void do_cmp() {
	dd.res = ss.val - dd.val;
	w_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_cmpb() {
	dd.res = ss.val - dd.val;
	b_write(dd.a, dd.res);
	setNZ(dd.res);
}

void do_jmp() {
	PC = dd.val;
}

void do_halt() {
	printf("\n");
	reg_print();
	exit(0);
}

void do_unknown() {
	exit(1);
}
