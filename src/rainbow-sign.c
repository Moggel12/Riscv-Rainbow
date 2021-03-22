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

int main(void)
{	
	// printf( "%s\n", CRYPTO_ALGNAME );

	// printf("sk size: %lu\n", CRYPTO_SECRETKEYBYTES );
	// printf("pk size: %lu\n",  CRYPTO_PUBLICKEYBYTES );
	// printf("hash size: %d\n", _HASH_LEN );
	// printf("signature size: %d\n\n", CRYPTO_BYTES );

	send_start();
	send_unsigned("sk size: %lu\n", CRYPTO_SECRETKEYBYTES, 10);
	send_unsigned("pk size: %lu\n",  CRYPTO_PUBLICKEYBYTES, 10);
	send_unsigned("hash size: %d\n", _HASH_LEN, 10);
	send_unsigned("signature size: %d\n\n", CRYPTO_BYTES, 10);

	// if( !(3 == argc) ) {
	// 	printf("Usage:\n\n\trainbow-sign sk_file_name file_to_be_signed\n\n");
	// 	return -1;
	// }

	//uint8_t *_sk = (uint8_t*)malloc( CRYPTO_SECRETKEYBYTES );
	uint8_t _sk[CRYPTO_SECRETKEYBYTES] = {0};

	int r = 0;

	// fp = fopen( argv[1] , "r");
	// if( NULL == fp ) {
	// 	printf("fail to open secret key file.\n");
	// 	return -1;
	// }

	/*
	 * This iteration of the RAINBOW-RISCV implementation reads one byte of the key at a time
     * The key is provided by the host machine
	 */
    send_string("Status", "Starting retrival of secret key");
    for (size_t i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
        unsigned char c = (unsigned char) hal_getc();
        _sk[i] = c;
        send_bytes("Status", &c, 1);
        /*hal_putc("1");*/
    } 
    send_string("Status", "Secret key retrived");

	// r = byte_fget( fp ,  _sk , CRYPTO_SECRETKEYBYTES );
	// fclose( fp );
	// if( CRYPTO_SECRETKEYBYTES != r ) {
	// 	printf("fail to load key file.\n");
	// 	return -1;
	// }

	/*
	 * The protocol used to communicate the message requires a null-byte before and after the size of the message, or (if the length of the message is <= 256) a single character
	 */
    send_string("Status", "Starting retrieval of message");
    int code = hal_getc();
    
    if (code == 0) {
        
        int old_code = 0;
        
        do {
            old_code = code;
            code += hal_getc();
            
            if ((old_code == 0) && (code == 0)) {
                send_string("Status", "Fail due to message being of length 0");
                return -1;
            }
            
        } while (old_code > 0);
    }

    int mlen = code;
    unsigned char *msg = (unsigned char *) malloc(mlen);

    for (int i = 0; i < mlen; i++) {
        msg[i] = (unsigned char) hal_getc();
    }

	// unsigned char * msg = NULL;
	// unsigned long long mlen = 0;
	// r = byte_read_file( &msg , &mlen , argv[2] );
	// if( 0 != r ) {
	// 	printf("fail to read message file.\n");
	// 	return -1;
	// }

	// unsigned char * signature = malloc( mlen + CRYPTO_BYTES );
	unsigned char signature[mlen + CRYPTO_BYTES];

	unsigned long long smlen = 0;
	r = crypto_sign( signature, &smlen, msg , mlen , _sk );
	if( 0 != r ) {
        send_string("Status", "Failed to sign message");
		return -1;
	}
    
	// byte_fdump( stdout , CRYPTO_ALGNAME " signature"  , signature + mlen , CRYPTO_BYTES );
	/*
	 * Output signed message as above
	 */
    send_bytes("Signature", signature, mlen + CRYPTO_BYTES);
    
    send_string("Status", "Success");

    send_stop();

	return 0;
}

