#include "rw.h"

extern byte mem[65536];
extern word reg[8];

byte b_read (adr a) {
	if (a < 8) {
		return reg[a];
	}
	else {
		return mem[a];
	}
}

void b_write (adr a, byte val) {
	if (a < 8) {
		reg[a] = val;
	}
	else {
		mem[a] = val;
	}
}

word w_read  (adr a) {
	//assert(a%2==0);
	word x;
	if (a < 8) {
		x = (reg[a + 1] << 8) | reg[a];
	}
	else {
		x = (mem[a + 1] << 8) | mem[a];
	}
	return x;
}

void w_write (adr a, word val) {
	//assert(a%2==0);
	if (a < 8) {
		reg[a] = val;
	}
	else {
		mem[a] = val & MASK;
		mem[a + 1] = val >> 8 & MASK;
	}
}
