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

//#define STATIC_SEED 0x31, 0x04, 0x57, 0x01, 0xac, 0x91, 0xc2, 0x20, 0xdc, 0x3e, 0x58, 0x52, 0xce, 0xd9, 0x1f, 0x8e, 0x9e, 0x13, 0xe9, 0xe5, 0x76, 0x2d, 0x17, 0x96, 0xf9, 0xdf, 0x95, 0x94, 0xd8, 0xbf, 0x33, 0x07, 0xc4, 0xfe, 0x12, 0x37, 0x1a, 0x6f, 0x22, 0x57, 0x37, 0x04, 0xe2, 0x91, 0x98, 0x0f, 0xf9, 0xdc

void get_rand_seed(unsigned char *rnd_seed);

int gen();

int main( void )
{
    while(1) {
        gen();
    }
}

int gen()
{
    uint8_t _sk[ CRYPTO_SECRETKEYBYTES ];
    uint8_t _pk[ CRYPTO_PUBLICKEYBYTES ];
    
    unsigned char rnd_seed[48] = {0};

    hal_setup(CLOCK_BENCHMARK);

    get_rand_seed(rnd_seed);

    randombytes_init(rnd_seed, NULL, 256);

    send_start();

	int r = crypto_sign_keypair( _pk, _sk );
	if( 0 != r ) {
        send_string("status", "genkey failed");
        return -1;
	}

    send_bytes("pk_value", _pk, CRYPTO_PUBLICKEYBYTES);
    send_bytes("sk_value", _sk, CRYPTO_SECRETKEYBYTES);
	send_string("status", "success" );
    send_stop();

    /*send_keys(_pk, _sk);*/

	return 0;
}

void get_rand_seed(unsigned char *rnd_seed) {
   
    int i;

    for (i = 0; i < 48; i++) {
        rnd_seed[i] = (unsigned char) hal_getc();
    }
}

