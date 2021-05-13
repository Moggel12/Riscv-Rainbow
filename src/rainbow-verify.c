///  @file rainbow-verify.c
///  @brief A command-line tool for verifying a signature.
///
#include <stdio.h>
#include <string.h>
#include <hal.h>
#include <stdint.h>
#include <sendfn.h>

#include "rainbow_config.h"

#include "utils.h"

#include "api.h"

// #define MAX_MESSAGE_LENGTH 128*1024

int verifier();

int main(void)
{   
    while(1) 
    {
        verifier();
    }
}

int verifier() 
{
    unsigned long long mlen;
    unsigned long long slen;
    int r;
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    hal_setup(CLOCK_BENCHMARK);

    for (size_t i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
        uint8_t c = (uint8_t) hal_getc();
        pk[i] = c;
    } 

    mlen = get_text_len();
    unsigned char msg[mlen];

    for (unsigned long long i = 0; i < mlen; i++) {
        msg[i] = (unsigned char) hal_getc();
        /*hal_putc(msg[i]);*/
    }

    slen = get_text_len();
    unsigned char signature[mlen + CRYPTO_BYTES];
    for (unsigned long long i = 0; i < mlen; i++) {
        signature[i] = msg[i];
    }
    

    for (unsigned long long i = mlen; i < mlen + slen; i++) {
        signature[i] = hal_getc();
    }
    
    send_start();

    r = crypto_sign_open( (uint8_t *) msg , &mlen , (uint8_t *) signature , mlen + CRYPTO_BYTES , pk );

    if( 0 == r ) {
        send_string("Status", "Success" );
        send_stop();
        return 0;
    } 
    send_string("Status", "Failed");
    send_stop();
    return -1;
}
