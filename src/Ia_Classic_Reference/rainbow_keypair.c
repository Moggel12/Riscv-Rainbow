/// @file rainbow_keypair.c
/// @brief implementations of functions in rainbow_keypair.h
///
#include "rainbow_keypair.h"
#include "rainbow_keypair_computation.h"

#include "blas_comm.h"
#include "blas.h"
#include "rainbow_blas.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sendfn.h>
/////////////////////////////////////////////////////////////////


#include "utils_prng.h"

#include "utils_malloc.h"

/*#if 64 < _V1*/
/*#define _MALLOC_*/
/*#endif*/


static
void generate_S_T( unsigned char * s_and_t , prng_t * prng0 )
{
    sk_t * _sk;
    unsigned size;

    size = sizeof(_sk->s1);
    prng_gen( prng0 , s_and_t , size );
    s_and_t += size;

    size = sizeof(_sk->t1);
    prng_gen( prng0 , s_and_t , size );
    s_and_t += size;

    size = sizeof(_sk->t4);
    prng_gen( prng0 , s_and_t , size );
    s_and_t += size;

    size = sizeof(_sk->t3);
    prng_gen( prng0 , s_and_t , size );
    s_and_t += size;
}


static
unsigned generate_l1_F12( unsigned char * sk, prng_t * prng0 )
{
    unsigned n_byte_generated = 0;
    sk_t * _sk;
    unsigned size;

    size = sizeof(_sk->l1_F1);
    prng_gen( prng0 , sk , size );
    sk += size;
    n_byte_generated += size;

    size = sizeof(_sk->l1_F2);
    prng_gen( prng0 , sk , size );
    sk += size;
    n_byte_generated += size;

    return n_byte_generated;
}


static
unsigned generate_l2_F12356( unsigned char * sk, prng_t * prng0 )
{
    unsigned n_byte_generated = 0;
    sk_t * _sk;
    unsigned size;

    size = sizeof(_sk->l2_F1);
    prng_gen( prng0 , sk , size );
    sk += size;
    n_byte_generated += size;

    size = sizeof(_sk->l2_F2);
    prng_gen( prng0 , sk , size );
    sk += size;
    n_byte_generated += size;

    size = sizeof(_sk->l2_F3);
    prng_gen( prng0 , sk , size );
    sk += size;
    n_byte_generated += size;

    size = sizeof(_sk->l2_F5);
    prng_gen( prng0 , sk , size );
    sk += size;
    n_byte_generated += size;

    size = sizeof(_sk->l2_F6);
    prng_gen( prng0 , sk , size );
    sk += size;
    n_byte_generated += size;

    return n_byte_generated;
}


static
void generate_B1_B2( unsigned char * sk , prng_t * prng0 )
{
    sk += generate_l1_F12( sk , prng0 );
    generate_l2_F12356( sk , prng0 );
}


//////////////////////////////////////////////////////////



int cpk_to_pk( pk_t * rpk, const cpk_t * cpk )
{
    // procedure:  cpk_t --> extcpk_t  --> pk_t

    // convert from cpk_t to extcpk_t
    ext_cpk_t _pk;
    ext_cpk_t * pk = &_pk;

    // setup prng
    prng_t prng0;
    prng_set( &prng0 , cpk->pk_seed , LEN_PKSEED );

    // generating parts of key with prng
    generate_l1_F12( pk->l1_Q1 , &prng0 );
    // copying parts of key from input. l1_Q3, l1_Q5, l1_Q6, l1_Q9
    memcpy( pk->l1_Q3 , cpk->l1_Q3 , sizeof(cpk->l1_Q3) );
    memcpy( pk->l1_Q5 , cpk->l1_Q5 , sizeof(cpk->l1_Q5) );
    memcpy( pk->l1_Q6 , cpk->l1_Q6 , sizeof(cpk->l1_Q6) );
    memcpy( pk->l1_Q9 , cpk->l1_Q9 , sizeof(cpk->l1_Q9) );

    // generating parts of key with prng
    generate_l2_F12356( pk->l2_Q1 , &prng0 );
    // copying parts of key from input: l2_Q9
    memcpy( pk->l2_Q9 , cpk->l2_Q9 , sizeof(cpk->l2_Q9) );

    // convert from extcpk_t to pk_t
    extcpk_to_pk( rpk , pk );

    return 0;
}



/////////////////////////////////////////////////////////



static
void calculate_t4( unsigned char * t2_to_t4 , const unsigned char *t1 , const unsigned char *t3 )
{
    //  t4 = T_sk.t1 * T_sk.t3 - T_sk.t2
    unsigned char temp[_V1_BYTE+32];
    unsigned char * t4 = t2_to_t4;
    for(unsigned i=0;i<_O2;i++) {  /// t3 width
        gfmat_prod( temp , t1 , _V1_BYTE , _O1 , t3 );
        gf256v_add( t4 , temp , _V1_BYTE );
        t4 += _V1_BYTE;
        t3 += _O1_BYTE;
    }
}



static
void obfuscate_l1_polys( unsigned char * l1_polys , const unsigned char * l2_polys , unsigned n_terms , const unsigned char * s1 )
{
    unsigned char temp[_O1_BYTE + 32];
    while( n_terms-- ) {
        gfmat_prod( temp , s1 , _O1_BYTE , _O2 , l2_polys );
        gf256v_add( l1_polys , temp , _O1_BYTE );
        l1_polys += _O1_BYTE;
        l2_polys += _O2_BYTE;
    }
}



///////////////////  Classic //////////////////////////////////

#if 0
// For debug
static void dump_sk( const sk_t* sk )
{
  byte_fdump(stdout, "sk->sk_seed:" , sk->sk_seed , LEN_SKSEED ); printf("\n");
  byte_fdump(stdout, "sk->s1 head:" , sk->s1 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->t1 head:" , sk->t1 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->t4 head:" , sk->t4 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->t3 head:" , sk->t3 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->l1_F1 head:" , sk->l1_F1 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->l1_F2 head:" , sk->l1_F2 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->l2_F1 head:" , sk->l2_F1 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->l2_F2 head:" , sk->l2_F2 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->l2_F3 head:" , sk->l2_F3 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->l2_F5 head:" , sk->l2_F5 , _O2_BYTE ); printf("\n");
  byte_fdump(stdout, "sk->l2_F6 head:" , sk->l2_F6 , _O2_BYTE ); printf("\n");
}
#endif

static
void _generate_secretkey( sk_t* sk, const unsigned char *sk_seed )
{
    memcpy( sk->sk_seed , sk_seed , LEN_SKSEED );

    // set up prng
    prng_t prng0;
    prng_set( &prng0 , sk_seed , LEN_SKSEED );

    // generating secret key with prng.
    generate_S_T( sk->s1 , &prng0 );
    generate_B1_B2( sk->l1_F1 , &prng0 );

    // clean prng
    memset( &prng0 , 0 , sizeof(prng_t) );
}


void generate_secretkey( sk_t* sk, const unsigned char *sk_seed )
{
    _generate_secretkey( sk , sk_seed );
    calculate_t4( sk->t4 , sk->t1 , sk->t3 );
}


int sk_to_pk( pk_t * rpk , const sk_t* isk )
{
    ext_cpk_t * pk = (ext_cpk_t*)malloc(sizeof(ext_cpk_t));
    sk_t * sk = (sk_t*)malloc(sizeof(sk_t));

    memcpy( sk , isk , sizeof(sk_t) );
    calculate_t4( sk->t4 , sk->t1 , sk->t3 );  // convert to t2

    calculate_Q_from_F( pk, sk , sk );   // compute the public key in ext_cpk_t format.
    calculate_t4( sk->t4 , sk->t1 , sk->t3 );  // recover t4

    obfuscate_l1_polys( pk->l1_Q1 , pk->l2_Q1 , N_TRIANGLE_TERMS(_V1) , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q2 , pk->l2_Q2 , _V1*_O1 , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q3 , pk->l2_Q3 , _V1*_O2 , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q5 , pk->l2_Q5 , N_TRIANGLE_TERMS(_O1) , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q6 , pk->l2_Q6 , _O1*_O2 , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q9 , pk->l2_Q9 , N_TRIANGLE_TERMS(_O2) , sk->s1 );
    // so far, the pk contains the full pk but in ext_cpk_t format.

    extcpk_to_pk( rpk , pk );     // convert the public key from ext_cpk_t to pk_t.

    memset( sk , 0 , sizeof(sk_t) );
    return 0;
}


int generate_keypair( pk_t * rpk, sk_t* sk, const unsigned char *sk_seed )
{
    _generate_secretkey( sk , sk_seed );
    
    // set up a temporary structure ext_cpk_t for calculating public key.
    ext_cpk_t _pk;
    ext_cpk_t * pk = &_pk;
    
    calculate_Q_from_F( pk, sk , sk );   // compute the public key in ext_cpk_t format.
    calculate_t4( sk->t4 , sk->t1 , sk->t3 );

    obfuscate_l1_polys( pk->l1_Q1 , pk->l2_Q1 , N_TRIANGLE_TERMS(_V1) , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q2 , pk->l2_Q2 , _V1*_O1 , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q3 , pk->l2_Q3 , _V1*_O2 , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q5 , pk->l2_Q5 , N_TRIANGLE_TERMS(_O1) , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q6 , pk->l2_Q6 , _O1*_O2 , sk->s1 );
    obfuscate_l1_polys( pk->l1_Q9 , pk->l2_Q9 , N_TRIANGLE_TERMS(_O2) , sk->s1 );
    // so far, the pk contains the full pk but in ext_cpk_t format.

    extcpk_to_pk( rpk , pk );     // convert the public key from ext_cpk_t to pk_t.

    return 0;
}