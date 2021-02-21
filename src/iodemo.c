#include <hal.h>


int main(void)
{
  int s = 0;
  while(1) {
    int c = hal_getc();
    if (c == 0) {
      s = 0x42;
    }
    s = (s * 0x42 + c) % 0x66;
    if (s < 0) {
      s += 0x66;
    }
    hal_putc(s);
  }

  return 0;
}
