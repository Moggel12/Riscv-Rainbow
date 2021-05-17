///  @file  sign.c
///  @brief the implementations for functions in api.h
///
///
#include <stdlib.h>
#include <string.h>

#include "rainbow_config.h"
#include "rainbow_keypair.h"
#include "rainbow.h"

#include "api.h"

#include "utils_hash.h"

#include <sendfn.h>

#include <rng.h>

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{
    unsigned char sk_seed[LEN_SKSEED] = {0};
    randombytes( sk_seed , LEN_SKSEED );

    int r = generate_keypair( (pk_t*) pk , (sk_t*) sk , sk_seed );

    for(int i=0;i<LEN_SKSEED;i++) sk_seed[i]=0;

    return r;
}

int
crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk)
{
	unsigned char digest[_HASH_LEN];

	hash_msg( digest , _HASH_LEN , m , mlen );

	send_bytes("Message digest", digest, _HASH_LEN);

	int r = -1;

	r = rainbow_sign( sm + mlen , (const sk_t*)sk , digest );

	memcpy( sm , m , mlen );
	smlen[0] = mlen + _SIGNATURE_BYTE;

	return r;
}

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,const unsigned char *sm, unsigned long long smlen,const unsigned char *pk)
{
	if( _SIGNATURE_BYTE > smlen ) return -1;

	unsigned char digest[_HASH_LEN];
	hash_msg( digest , _HASH_LEN , sm , smlen-_SIGNATURE_BYTE );

	int r = -1;

	r = rainbow_verify( digest , sm + smlen-_SIGNATURE_BYTE , (const pk_t *)pk );

	memcpy( m , sm , smlen-_SIGNATURE_BYTE );
	mlen[0] = smlen-_SIGNATURE_BYTE;

	return r;
}

