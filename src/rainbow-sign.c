///  @file rainbow-sign.c
///  @brief A command-line tool for signing a file.
///

#include <stdio.h>
#include <stdint.h>
#include <hal.h>
#include <sendfn.h>

#include "rainbow_config.h"

#include "utils.h"

#include "api.h"

int sign();

int main(void)
{	
    while (1) 
    {
        sign();
    }
}

int sign() 
{
    unsigned long long mlen;
    unsigned long long smlen;
	// unsigned char *msg_end; // Future proofing
	// unsigned char *signature_end; // Also future proofing
	// unsigned char msg[MAX_MESSAGE_LENGTH];
	// unsigned char signature[MAX_MESSAGE_LENGTH + CRYPTO_BYTES];

	//uint8_t *_sk = (uint8_t*)malloc( CRYPTO_SECRETKEYBYTES );
	uint8_t _sk[CRYPTO_SECRETKEYBYTES] = {0};

	int r = 0;

    hal_setup(CLOCK_BENCHMARK);

	/*
	 * This iteration of the RAINBOW-RISCV implementation reads one byte of the key at a time
     * The key is provided by the host machine
	 */
    for (size_t i = 0; i < CRYPTO_SECRETKEYBYTES; i++) { 
        unsigned char c = (unsigned char) hal_getc(); 
        _sk[i] = c;
        /*hal_putc("1");*/
    } 

	// /*
	//  * The protocol used to communicate the message requires a null-byte before and after the size of the message, or (if the length of the message is <= 256) a single character
	//  */
    // int code = hal_getc();
    
    // if (code == 0) {
        
    //     int old_code = 0;
        
    //     do {
    //         old_code = code;
    //         code += hal_getc();
            
    //         if ((old_code == 0) && (code == 0)) {
    //             return -1;
    //         }
            
    //     } while (old_code > 0);
    // }

    mlen = get_text_len();
    // msg_end = (unsigned char *) msg + mlen;
    unsigned char msg[mlen];
    unsigned char signature[mlen + CRYPTO_BYTES];
	send_start();

    for (unsigned long long i = 0; i < mlen; i++) {
        msg[i] = hal_getc();
    }

	// unsigned char * signature = malloc( mlen + CRYPTO_BYTES );
	// signature_end = (unsigned char *) signature + mlen + CRYPTO_BYTES;

	smlen = 0;
	r = crypto_sign( signature, &smlen, msg , mlen , _sk );
	if( 0 != r ) {
        send_string("Status", "Failure");
		return -1;
	}
    
	/*
	 * Output signed message as above
	 */
    send_string("Message", (char *) msg);
    // send_bytes("sk_value", _sk, CRYPTO_SECRETKEYBYTES);
    send_bytes("Signature", signature, mlen + CRYPTO_BYTES);
    
    send_string("Status", "Success");

    send_stop();

	return 0;
}

