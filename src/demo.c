#include "hal.h"

#include <fips202.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void bytes_to_hashed(char outs[33], const unsigned char* x, unsigned long long xlen)
{
	unsigned char buf[32];
	sha3_256(buf, x, xlen);
	unsigned long long i;
	for (i = 0; i < 32; i++)
		sprintf(outs + 2 * i, "%02x", buf[i]);
	outs[2 * 32] = 0;
}

unsigned long long hash_cycles;

#define EVAL(X) STR(X)
#define STR(X) #X

static void hal_printf(const char* format, ...)
{
	char outs[80];
	va_list args;
	va_start(args, format);
	vsnprintf(outs, sizeof(outs), format, args);
	va_end(args);
	hal_send_str(outs);
}

__attribute__((naked)) uint64_t hal_get_num_instr(void);
__attribute__((naked)) uint64_t hal_get_num_instr(void)
{
#define LE "\n\t"
	asm volatile(LE "csrr a1, minstreth" LE "csrr a0, minstret" LE "csrr a2, minstreth" LE
			"bne a1, a2, hal_get_num_instr" LE "ret");
}

int main(void)
{
  hal_printf("Hello world!");
  while(1);
  return 0;
}
