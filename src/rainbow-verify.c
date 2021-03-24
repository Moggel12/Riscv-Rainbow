///  @file rainbow-verify.c
///  @brief A command-line tool for verifying a signature.
///

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "rainbow_config.h"

#include "utils.h"

#include "api.h"

int get_text(unsigned char *text, int *text_len);

int main(void)
{
    unsigned char *msg;
    int mlen;
    unsigned char *signature;
    int slen;
    int r;
	uint8_t * pk[CRYPTO_PUBLICKEYBYTES];

    send_start();
    send_string("name", CRYPTO_ALGNAME);
    send_unsigned("sk size", CRYPTO_SECRETKEYBYTES, 10);
    send_unsigned("sk size", CRYPTO_PUBLICKEYBYTES, 10);
    send_unsigned("hash size", _HASH_LEN, 10);
    send_unsigned("signature size", CRYPTO_BYTES, 10);

    send_string("Status", "Starting retrival of public key");

    for (size_t i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
        unsigned char c = (unsigned char) hal_getc();
        pk[i] = c;
        send_bytes("Status", &c, 1);
        /*hal_putc("1");*/
    } 
    send_string("Status", "Secret key retrived");

    r = get_text(msg, &mlen);
    
    if (r != 0) {
        return -1;
    }

    r = get_text(signature, &slen);

    if (r != 0) {
        return -1;
    }

	r = crypto_sign_open( msg , &mlen , signature , mlen + CRYPTO_BYTES , pk );

	free( msg );
	free( signature );

	if( 0 == r ) {
        send_string("Status", "Correctly verified." );
	    return 0;
	} 

    send_string("Status", "Verification failed.");
    return -1;
	
}

// Blocks until the host provides some text
int get_text(unsigned char *text, int *text_len) {
	/*
	 * The protocol used to communicate the message requires a null-byte before and after the size of the message, or (if the length of the message is <= 255) a single character
	 */
    send_string("Status", "Starting retrieval of text");
    int code = hal_getc();
    
    if (code == 0) {
        
        int old_code = 0;
        
        do {
            old_code = code;
            code += hal_getc();
            
            if ((old_code == 0) && (code == 0)) {
                send_string("Status", "Failed to obtain text size: Size is 0");
                return -1;
            }
            
        } while (old_code > 0);
    }

    text_len = &code;
    text = (unsigned char *) malloc(*text_len + CRYPTO_BYTES);
	if( NULL == text ) {
        send_string("Status", "Failed to allocate text buffer. Aborting");
	    return -1;
	}

    for (int i = 0; i < *text_len; i++) {
        text[i] = (unsigned char) hal_getc();
    }

    return 0;
}
