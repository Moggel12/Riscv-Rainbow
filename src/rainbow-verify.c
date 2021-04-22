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
        break;
    }
}

int verifier() 
{
    /*unsigned long long mlen;*/
    /*unsigned long long slen;*/
    /*int r;*/
	/*uint8_t pk[CRYPTO_PUBLICKEYBYTES];*/
    hal_setup(CLOCK_BENCHMARK);

    /*for (size_t i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {*/
        /*uint8_t c = (uint8_t) hal_getc();*/
        /*pk[i] = c;*/
    /*} */

    /*for (size_t i = 0; i < sizeof(pk); i++) {*/
        /*hal_putc(pk[i]);*/
    /*}*/

    /*mlen = get_text_len();*/
    unsigned char msg[10];

    for (unsigned long long i = 0; i < 10; i++) {
        msg[i] = (char) hal_getc();
        hal_putc(msg[i]);
    }

    // msg_end = (unsigned char *) msg + mlen;

    /*slen = get_text_len();*/
    /*unsigned char signature[slen];*/

    /*for (unsigned long long i = 0; i < slen; i++) {*/
        /*signature[i] = hal_getc();*/
    /*}*/
    /*for (size_t i = 0; i < sizeof(signature); i++) {*/
        /*hal_putc(signature[i]);*/
    /*}*/
    
    // signature_end = (unsigned char *) signature + slen;

    /*send_start();*/

	/*r = crypto_sign_open( (uint8_t *) msg , &mlen , (uint8_t *) signature , mlen + CRYPTO_BYTES , pk );*/


	/*if( 0 == r ) {*/
        /*send_string("Status", "Success" );*/
        /*send_stop();*/
		/*return 0;*/
	/*} */
    /*send_unsigned("r", r, 10);*/
    /*send_bytes("message", msg, sizeof(msg));*/
    /*send_bytes("pk", pk, sizeof(pk));*/
    /*send_bytes("signature", signature, sizeof(signature));*/
    /*send_string("Status", "Failed");*/
    /*send_stop();*/
    return -1;
}
