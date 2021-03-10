#include <hal.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) { 
    char str[19];
    int c_int;
    char *from_msg;
    int i;

    from_msg = " from device";
    i = 0;
    do {
        c_int = hal_getc();
        char c = (char) c_int;
        hal_putc(c_int);
        str[i] = c;
        i++;
    } while (c_int != 0);
    for (i = 7; i <= 18; i++) {
        str[i] = from_msg[i];
    }

    hal_puts(str);
}
