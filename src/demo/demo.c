#include <hal.h>
#include <sendfn.h>
#include "test.h"
#include <aes/api_aes.h>
#include <aes/ttable/common.h>
#include <sha256/api_sha256.h>
int main(void)
{
    sha256_hash(NULL, NULL, 0);
    hal_setup(CLOCK_FAST);
    unsigned int j = 25*4;
    unsigned int i = 1;
    send_start();
    send_unsigned("number", i+j, 10);
    send_string("strings", "You can send \"escaped\"\n strings!");
    send_stop();
    return 0;
}
