#ifndef RW_H_INCLUDED
#define RW_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define MASK 0xff

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned short adr;

void b_write(adr a, byte val);
byte b_read(adr a);
void w_write(adr a, word val);
word w_read(adr a);

#define PC reg[7]
#define SP reg[6]
#define OSTAT 0177564
#define ODATA 0177566

#endif // RW_H_INCLUDED
