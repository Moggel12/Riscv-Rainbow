#include <hal.h>
#include <sendfn.h>
#include "test.h"
int main(void)
{
    hal_setup(CLOCK_FAST);
    int i = add(1, 2);
    send_start();
    send_unsigned("result", i, 10);
    send_stop();
    return 0;
}
