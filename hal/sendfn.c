#include "sendfn.h"

#include <ctype.h>

static int started = 0;

void send_start()
{
	if (started) {
		send_stop();
	}
	hal_puts("{\n");
	started = 1;
}

void send_stop()
{
	if (started) {
		hal_puts("}\n");
		started = 0;
	}
}

/**
 * \brief Send a value to the host.
 *
 * \param name Name of the value.
 * \param value The value encoded as a nul terminated string.
 */
static void send_value(const char* name, const char* value)
{
	hal_puts("\"");
	hal_puts(name);
	hal_puts("\": ");
	hal_puts(value);
	hal_puts(",\n");
}

void send_unsigned(const char* name, unsigned int c, int base)
{
	char outs[sizeof(unsigned) * 8 + 3];
	int i = sizeof(outs) - 1;
  outs[i] = '\0';
  for (i -= 1; c != 0; c /= base, i -= 1) {
    unsigned d = c % base;
    outs[i] = (d < 10 ? '0' : 'A' - 10) + d;
  }
  /* Special case: c was 0 */
  if (i == sizeof(outs) - 2) {
    outs[i--] = '0';
  }
  int padend = 0;
  switch (base) {
  case 2:
    outs[i] = 'b';
    break;
  case 8:
    outs[i] = 'o';
    padend = (sizeof(outs) - 1) - ((sizeof(c) * 8) / 3 + 3);
    break;
  case 10:
    padend = i;
    break;
  case 16:
    outs[i] = 'x';
    padend = (sizeof(outs) - 1) - ((sizeof(c) * 8) / 4 + 2);
    break;
  }
  if (base != 10) {
    outs[--i] = '0';
    for (--i; i >= padend; --i) {
      outs[i] = ' ';
    }
  }
  i += 1;
	send_value(name, outs + i);
}

static char* basepow2_to_str(char* outs, unsigned long long c, int base)
{
  int i;
  unsigned mask = 0x1;
  unsigned shift = 1;
  char b = 0;
  switch (base) {
  case 2:
    b = 'b';
    break;
  case 8:
    mask = 0x7;
    shift = 3;
    b = 'o';
    break;
  case 16:
    mask = 0xF;
    shift = 4;
    b = 'x';
    break;
  }
  outs[66] = '\0';
  for (i = 65; c > 0; c >>= shift, i -= 1) {
    int h = c & mask;
    outs[i] = (h < 10 ? '0' : 'A' - 10) + h;
  }
  if (i == 65) {
    outs[i--] = '0';
  }
  outs[i--] = b;
  outs[i] = '0';
  return outs + i;
}

static char* base10_to_str(char outs[], unsigned long long c)
{
  int i = 0;
  if (c < 10) {
    outs[0] = '0' + c;
    outs[1] = '\0';
  } else {
    outs[20] = '\0';
    i = 19;
    while (c != 0) {
      /* Method adapted from ""hackers delight":
         Creates an approximation of q = (8/10) */
      unsigned long long q = (c >> 1) + (c >> 2);
      q = q + (q >> 4);
      q = q + (q >> 8);
      q = q + (q >> 16);
      q = q + (q >> 32);
      /* Now q = (1/10) */
      q = q >> 3;
      /* Since q contains an error due to the bits shifted out of the value, we
         only use it to determine the remainder.  */
      unsigned long long r = c - ((q << 3) + (q << 1));
      c = q;
      /* The remainder might be off by 10, so q may be off by 1 */
      if (r > 9) {
        c += 1;
        r -= 10;
      }
      outs[i] = '0' + (unsigned) r;
      i -= 1;
    }
    i += 1;
  }
  return outs + i;
}

void send_unsignedll(const char* name, unsigned long long c, int base)
{
  char outs[sizeof(c) * 8 + 3];
  char* val = "";
  switch (base) {
  case 2:
  case 8:
  case 16:
    val = basepow2_to_str(outs, c, base);
    break;
  case 10:
    val = base10_to_str(outs, c);
    break;
  }
  send_value(name, val);
}

void send_string(const char* name, const char* str)
{
	hal_putc('"');
	hal_puts(name);
	hal_puts("\": \"");
	while (*str != '\0') {
    int c = *str;
		switch (c) {
		case '\\':
			hal_puts("\\\\");
			break;
		case '\'':
			hal_puts("\\\'");
			break;
		case '"':
			hal_puts("\\\"");
			break;
		case '\a':
			hal_puts("\\a");
			break;
		case '\b':
			hal_puts("\\b");
			break;
		case '\f':
			hal_puts("\\f");
			break;
		case '\n':
			hal_puts("\\n");
			break;
		case '\r':
			hal_puts("\\r");
			break;
		case '\t':
			hal_puts("\\t");
			break;
		case '\v':
			hal_puts("\\v");
			break;
		default:
			if (isgraph(c) || c == ' ') {
				hal_putc(c);
			} else {
				hal_puts("\\x");
        int h = (c >> 4) & 0xFu;
				hal_putc((h < 10 ? '0' : 'a' - 10) + h);
        h = c & 0xFu;
				hal_putc((h < 10 ? '0' : 'a' - 10) + h);
			}
		}
		str++;
	}
	hal_puts("\",\n");
}

void send_bytes(const char *name, const unsigned char* buf, unsigned len)
{
	hal_putc('"');
	hal_puts(name);
	hal_puts("\": bytes.fromhex(\"");
  for (unsigned i = 0; i < len; ++i) {
    int c = buf[i];
    int h = (c >> 4) & 0xFu;
    hal_putc((h < 10 ? '0' : 'A' - 10) + h);
    h = c & 0xFu;
    hal_putc((h < 10 ? '0' : 'A' - 10) + h);
  }
	hal_puts("\"),\n");
}
