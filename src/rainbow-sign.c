///  @file rainbow-sign.c
///  @brief A command-line tool for signing a file.
///

#include <stdio.h>
#include <stdint.h>

#include "rainbow_config.h"

#include "utils.h"

#include "api.h"



int main( int argc , char ** argv )
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
	uint8_t _sk[CRYPTO_SECRETKEYBYTES];

	FILE * fp;
	int r = 0;

	// fp = fopen( argv[1] , "r");
	// if( NULL == fp ) {
	// 	printf("fail to open secret key file.\n");
	// 	return -1;
	// }

	/*
	 * Put code to get the key here
	 */

	// r = byte_fget( fp ,  _sk , CRYPTO_SECRETKEYBYTES );
	// fclose( fp );
	// if( CRYPTO_SECRETKEYBYTES != r ) {
	// 	printf("fail to load key file.\n");
	// 	return -1;
	// }

	/*
	 * Put code to obtain message to be signed
	 */

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
		printf("sign() fail.\n");
		return -1;
	}

	// byte_fdump( stdout , CRYPTO_ALGNAME " signature"  , signature + mlen , CRYPTO_BYTES );
	/*
	 * Output signed message as above
	 */

	// printf("\n");

	// free( msg );
	// free( signature );
	// free( _sk );

	return 0;
}

