///  @file rainbow-genkey.c
///  @brief A command-line tool for generating key pairs.
///
#include <hal.h>
#include <stdio.h>
#include <stdint.h>

#include "rainbow_config.h"

#include "utils.h"

#include "rng.h"

#include "api.h"

#include <sendfn.h>

int main( void )
{
    send_start();
	send_unsigned("name", CRYPTO_ALGNAME, 10);
    send_unsigned("sk_size", CRYPTO_SECRETKEYBYTES, 10);
    send_unsigned("pk_size",  CRYPTO_PUBLICKEYBYTES, 10);
    send_unsigned("hash_size", _HASH_LEN, 10);
    send_unsigned("signature_size", CRYPTO_BYTES, 10);

	// set random seed
	unsigned char rnd_seed[48] = {0};
    //int rr = byte_from_binfile( rnd_seed , 48 , (4==argc)? argv[3] : "/dev/random" );
	//if( 0 != rr ) send_string("status", "read seed file fail.");
	randombytes_init( rnd_seed , NULL , 256 );


	uint8_t *_sk = (uint8_t*)malloc( CRYPTO_SECRETKEYBYTES );
	uint8_t *_pk = (uint8_t*)malloc( CRYPTO_PUBLICKEYBYTES );

	int r = crypto_sign_keypair( _pk, _sk );
	/*if( 0 != r ) {*/
	    /*send_string("error", "genkey failed");*/
	    /*return -1;*/
	/*}*/
    
    if (0 != r) {
        send_string("status", "genkey failed");
        return -1;
    }

    // Currently the keys are printed via UART
    send_string("pk_value", (char *) _pk);
    send_string("sk_value", (char *) _sk);

	send_string("status", "success" );

	free( _sk );
	free( _pk );

    send_stop();

	return 0;
}

