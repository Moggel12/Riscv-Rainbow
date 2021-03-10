#include <hal.h>
#include <sendfn.h>

int main(void)
{
    hal_setup(CLOCK_FAST);
    unsigned int j = 25*4;
    unsigned int i = 1;
    send_start();
    send_unsigned("number", i+j, 10);
    send_string("strings", "You can send \"escaped\"\n strings!");
    send_stop();
    return 0;
}
