#include "rw.h"
#include "commands.h"
#include "pdp.h"

byte mem[65536];
word reg[8];
word nn, r;
signed char xx;
Var ss, dd;
int b;

void reg_print() {
	int h;
	for (h = 0; h < 8; h ++)
		printf("R%d=%06o ", h, reg[h]);
	printf("\n");
}

extern void (* func) (void);
extern void NZVC_print();

Var get_ssdd(int byte, word w) {
	int nn;
	int r = w & 7;
	int mode = (w >> 3) & 7;
	Var var;
	switch (mode) {
	case 0 :
		var.a = r;
		var.val = reg[r];
		printf("R%d ", r);
		break;
	case 1:
		var.a = reg[r];
		if (byte == 0)
			var.val = w_read(var.a);
		else
			var.val = b_read(var.a);
		printf("(R%d) ", r);
		break;
	case 2:
		var.a = reg[r];
		if (byte == 0)
			var.val = w_read(var.a);
		else
			var.val = b_read(var.a);
		if (r < 6)
			reg[r] ++;
		else
			reg[r] += 2;    // +2 word, r6, r7
		if (r == 7)
			printf("#%o ", var.val);
		else
			printf("(R%d)+ ", r);
		break;
	case 3:
		var.a = reg[r];
		reg[r] += 2;
		var.a = w_read(var.a);
		if (byte == 0)
			var.val = w_read(var.a);
		else
			var.val  = b_read(var.a);
		if (r == 7)
			printf("@#%o ", var.a );
		else
			printf("@(R%d) ", r);
		break;
	case 4:
		reg[r] -= 2;
		var.a = reg[r];
		if (byte == 0)
			var.val = w_read(var.a);
		else
			var.val = b_read(var.a);
		printf("-(R%d)", r);
		break;
	case 5:
		reg[r] -= 2;
		var.a = reg[r];
		var.a = w_read(var.a);
		if (byte == 0)
			var.val = w_read(var.a);
		else
			var.val = b_read(var.a);
		printf("@-(R%d) ", r);
		break;
	case 6:
		nn = w_read(PC);
		PC += 2;
		var.a = reg[r] + nn;
		// var.a = w_read(var.a);
		if (byte == 0)
			var.val =  w_read(var.a);
		else
			var.val = b_read(var.a);
		if (r == 7)
			printf("%o ", var.a);
		else
			printf("%o(R%d)", nn, r);
		break;
	case 7:
		nn = w_read(PC);
		PC += 2;
		var.a = reg[r] + nn;
		var.a = w_read(var.a);
		if (byte == 0) {
			var.a = w_read(var.a);
			var.val = w_read(var.a);
		} else {
			var.a = b_read(var.a);
			var.val = b_read(var.a);
		}
		if (r == 7)
			printf("@%o ", nn);
		else
			printf("@%o(R%d) ", nn, r);
		break;
	}
	return var;
}

char get_nn (word w) {
	return w & 077;
}

char get_xx (word w) {
	return (byte)(w & 0xFF);
}

int get_r (word w) {
	r = w & 07;
	printf("R%d ", r);
	return r;
}

Commands commands_list[] = {
	{0111111, 0000000, "halt", do_halt, NO_PARAM},
	{0170000, 0010000, "mov", do_mov, HAS_SS | HAS_DD},
	{0170000, 0110000, "movb", do_movb, HAS_SS | HAS_DD},
	{0170000, 0060000, "add", do_add, HAS_SS | HAS_DD},
	{0170000, 0160000, "sub", do_sub, HAS_SS | HAS_DD},
	{0170000, 0070000, "mul", do_mul, HAS_SS | HAS_DD},
	{0177700, 0005200, "inc", do_inc, HAS_DD},
	{0177700, 0105200, "incb", do_incb, HAS_DD},
	{0177700, 0005300, "dec", do_dec, HAS_DD},
	{0177700, 0105300, "decb", do_decb, HAS_DD},
	{0170000, 0020000, "cmp", do_cmp, HAS_SS | HAS_DD},
	{0170000, 0120000, "cmpb", do_cmpb, HAS_SS | HAS_DD},
	{0177000, 0077000, "sob", do_sob, HAS_NN | HAS_RL},
	{0177700, 0005000, "clr", do_clr, HAS_DD},
	{0177700, 0105000, "clrb", do_clrb, HAS_DD},
	{0177700, 0005700, "tst", do_tst, HAS_DD},
	{0177700, 0105700, "tstb", do_tstb, HAS_DD},
	{0XFF00, 0001400, "beq", do_beq, HAS_XX},
	{0XFF00, 0001000, "bne", do_bne, HAS_XX},
	{0XFF00, 0000400, "br", do_br, HAS_XX},
	{0177700, 0000100, "jmp", do_jmp, HAS_DD},
	{0XFF00, 0100000, "bpl", do_bpl, HAS_XX},
	{0177000, 0004000, "jsr", do_jsr, HAS_DD | HAS_RL},
	{0177770, 0000200, "rts", do_rts, HAS_RR},
	{0, 0, "unknown", do_unknown, 0}
};

void load_files(FILE * f) {
	unsigned int start, n, byte;
	int i;
	while (fscanf(f, "%x%x", &start, &n) == 2) {
		for (i = 0; i < n; i ++) {
			fscanf(f, "%x", &byte);
			b_write(start + i, byte);
		}
	}
}

void run_programm(adr start) {
	int i;
	word w;
	PC = start;
	w_write(OSTAT, 0x0080);
	while (1) {
		w = w_read(PC);
		printf("%06o : %06o ", PC, w);
		PC += 2;
		for (i = 0; i < sizeof(commands_list) / sizeof(commands_list[0]); i ++) {
			Commands x;
			x = commands_list[i];
			if ((w & x.mask) == x.opcode) {
				printf("%s ", x.name);
				b = (w >> 15) & 1;
				if (x.param & HAS_NN)
					nn = get_nn(w);
				if (x.param & HAS_RL)
					r = get_r (w >> 6);
				if (x.param & HAS_RR)
					r = get_r(w);
				if (x.param  & HAS_SS)
					ss = get_ssdd(b, w >> 6);
				if (x.param  & HAS_DD)
					dd = get_ssdd(b, w);
				if (x.param & HAS_XX) {
					xx = get_xx(w);
					printf("x = %d ", xx);
				}
				x.func();
				printf("\n");
				NZVC_print();
				reg_print();
				//printf("\n");
				break;
			}
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[])
{
	if (argc == 1)
		exit(1);
	FILE * file_input = NULL;
	file_input = fopen(argv[1], "r");
	if (file_input == NULL) {
		perror(argv[1]);
		exit(1);
	}
	load_files(file_input);
	fclose(file_input);
	run_programm(0x200);
	return 0;
}