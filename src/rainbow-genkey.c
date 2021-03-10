///  @file rainbow-genkey.c
///  @brief A command-line tool for generating key pairs.
///

#include <stdio.h>
#include <stdint.h>

#include "rainbow_config.h"

#include "utils.h"

#include "rng.h"

#include "api.h"

#include <sendfn.h>

int main( int argc , char ** argv )
{
    send_start();
	send_unsigned("name", CRYPTO_ALGNAME );
    send_unsigned("sk_size", CRYPTO_SECRETKEYBYTES );
    send_unsigned("pk_size",  CRYPTO_PUBLICKEYBYTES );
    send_unsigned("hash_size", _HASH_LEN );
    send_unsigned("signature_size", CRYPTO_BYTES );

	/*if( !((3 == argc) || (4 == argc)) ) {*/
		/*printf("Usage:\n\n\trainbow-genkey pk_file_name sk_file_name [random_seed_file]\n\n");*/
		/*return -1;*/
	/*}*/

	// set random seed
	unsigned char rnd_seed[48] = {0};
	int rr = byte_from_binfile( rnd_seed , 48 , (4==argc)? argv[3] : "/dev/random" );
	if( 0 != rr ) send_string("status", "read seed file fail.");
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

	/*fp = fopen( argv[1] , "w+");*/
	/*if( NULL == fp ) {*/
	    /*printf("fail to open public key file.\n");*/
	    /*return -1;*/
	/*}*/
	/*byte_fdump( fp , CRYPTO_ALGNAME " public key" , _pk , CRYPTO_PUBLICKEYBYTES );*/
	/*fclose( fp );*/

    send_string("pk_value", _pk);

	/*fp = fopen( argv[2] , "w+");*/
	/*if( NULL == fp ) {*/
        /*printf("fail to open secret key file.\n");*/
	    /*return -1;*/
	/*}*/
	/*//ptr = (unsigned char *)&sk;*/
	/*//sprintf(msg,"%s secret key", name);*/
	/*byte_fdump( fp ,  CRYPTO_ALGNAME " secret key" , _sk , CRYPTO_SECRETKEYBYTES );*/
	/*fclose( fp );*/

    send_string("sk_value", _sk);

	send_string("status", "success" );

	free( _sk );
	free( _pk );

    send_stop();

	return 0;
}

