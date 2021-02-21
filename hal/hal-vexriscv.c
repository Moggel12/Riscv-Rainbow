#include "hal.h"

void hal_setup(const enum clock_mode clock)
{
	/* Absolutely no setup required for Murax based platforms */
	(void) clock;
}

/* Murax UART */

typedef struct {
	volatile uint32_t DATA;
	volatile uint32_t STATUS;
	volatile uint32_t CLOCK_DIVIDER;
	volatile uint32_t FRAME_CONFIG;
} Uart_Reg;

enum UartParity { NONE = 0, EVEN = 1, ODD = 2 };
enum UartStop { ONE = 0, TWO = 1 };

typedef struct {
	uint32_t dataLength;
	enum UartParity parity;
	enum UartStop stop;
	uint32_t clockDivider;
} Uart_Config;

static uint32_t uart_readAvailability(Uart_Reg* reg)
{
	return (reg->STATUS >> 24) & 0xFF;
}

static int uart_read(Uart_Reg* reg)
{
	while (uart_readAvailability(reg) == 0)
		;
	return reg->DATA & (0xFFu);
}

static uint32_t uart_writeAvailability(Uart_Reg* reg)
{
	return (reg->STATUS >> 16) & 0xFF;
}

static void uart_write(Uart_Reg* reg, uint32_t data)
{
	while (uart_writeAvailability(reg) == 0)
		;
	reg->DATA = data;
}

#define UART ((Uart_Reg*) (0xF0010000))

int hal_getc(void)
{
  return uart_read(UART);
}

void hal_putc(int c)
{
	uart_write(UART, c & 0xFFu);
}

void hal_puts(const char* in)
{
	const char* cur = in;
	while (*cur) {
		uart_write(UART, *cur);
		cur += 1;
	}
}

__attribute__((naked)) uint64_t hal_get_time(void)
{
	// clang-format off
#define LE "\n\t"
  asm volatile (LE"csrr a1, mcycleh"
                LE"csrr a0, mcycle"
                LE"csrr a2, mcycleh"
                LE"bne a1, a2, hal_get_time"
                LE"ret");
#undef LE
	// clang-format on
}

__attribute__((naked)) uint64_t hal_get_num_instr(void)
{
	// clang-format off
#define LE "\n\t"
  asm volatile (LE"csrr a1, minstreth"
                LE"csrr a0, minstret"
                LE"csrr a2, minstreth"
                LE"bne a1, a2, hal_get_time"
                LE"ret");
#undef LE
	// clang-format on
}
