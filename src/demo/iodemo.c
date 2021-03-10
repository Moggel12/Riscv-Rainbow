#include <hal.h>


int main(void)
{
  while(1) {
    int c = hal_getc();
    hal_putc(c);
  }

  return 0;
}
