///  @file rainbow-verify.c
///  @brief A command-line tool for verifying a signature.
///

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "rainbow_config.h"

#include "utils.h"

#include "api.h"

#define MAX_MESSAGE_LENGTH 128*1024

int get_text(unsigned char *text, int *text_len);

int main(void)
{   
    unsigned char *msg_end;
    unsigned char *msg[MAX_MESSAGE_LENGTH];
    int mlen;
    unsigned char *signature_end;
    unsigned char *signature[MAX_MESSAGE_LENGTH + CRYPTO_BYTES];
    int slen;
    int r;
	uint8_t * pk[CRYPTO_PUBLICKEYBYTES];


    for (size_t i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
        unsigned char c = (unsigned char) hal_getc();
        pk[i] = c;
    } 

    mlen = get_text_len();
    
    for (int i = 0; i < mlen; i++) {
        msg[i] = (unsigned char) hal_getc();
    }

    msg_end = msg + mlen;

    slen = get_text();

    for (int i = 0; i < slen; i++) {
        signature[i] = (unsigned char) hal_getc();
    }

    signature_end = signature + slen;

	r = crypto_sign_open( msg , &mlen , signature , mlen + CRYPTO_BYTES , pk );

    send_start();

	if( 0 == r ) {
        send_string("Status", "Success" );
	    return 0;
	} 

    send_string("Status", "Failure");
    send_stop();
    return -1;
	
}
