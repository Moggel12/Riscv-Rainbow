
#include "rainbow_keypair.h"
#include "rainbow_blas.h"


//
//  The file implements the idea of (publicly) evaluating MQ
//  come from Tung Chou(https://www.win.tue.nl/~tchou/).
//


#define _MAX_N 256

#if _PUB_N > _MAX_N
error. _PUB_N > _MAX_N
#endif



#if 16 == _GFSIZE
#if _PUB_M_BYTE > 32
error. _PUB_M_BYTE > 32
#endif
#define TMPVEC_LEN 32
#else
#if _PUB_M_BYTE > 128
error. _PUB_M_BYTE > 128
#endif
#define TMPVEC_LEN 128
#endif



static
void accu_eval_quad( unsigned char * accu_res , const unsigned char * trimat , const unsigned char * x_in_byte , unsigned num_gfele_x , unsigned vec_len )
{
   const unsigned char *_x = x_in_byte;
   unsigned char _xixj[_MAX_N];
   unsigned n = num_gfele_x;

   for(unsigned i=0;i<n;i++) {
      unsigned i_start = i-(i&3);
      for(unsigned j=i;j<n;j++) _xixj[j]=_x[j];
      gfv_mul_scalar( _xixj+i_start , _x[i] , n-i_start );
      for(unsigned j=i;j<n;j++) {
         unsigned idx = _xixj[j];
         if(idx) gf256v_add( accu_res + TMPVEC_LEN*idx , trimat , vec_len );
         trimat += vec_len;
      }
   }
}


static
void madd_reduce( unsigned char * y , const unsigned char * tmp_res , unsigned vec_len )
{
    unsigned char tmp[TMPVEC_LEN];
    int accu_bit = 1;

    gf256v_set_zero( y , vec_len );
    // x1
    for(int i=1;i<_GFSIZE;i+=2) gf256v_add( y , tmp_res+TMPVEC_LEN*i , vec_len );
    // x2
    accu_bit = 1<<1; // 2
    gf256v_set_zero( tmp , vec_len );
    for(int i=accu_bit;i<_GFSIZE;i+=accu_bit*2) {
      for(int j=0;j<accu_bit;j++) gf256v_add( tmp , tmp_res+TMPVEC_LEN*(i+j) , vec_len );
    }
    gf16v_madd( y , tmp , accu_bit ,  vec_len );

    // x4
    accu_bit = 1<<2; // 4
    gf256v_set_zero( tmp , vec_len );
    for(int i=accu_bit;i<_GFSIZE;i+=accu_bit*2) {
      for(int j=0;j<accu_bit;j++) gf256v_add( tmp , tmp_res+TMPVEC_LEN*(i+j) , vec_len );
    }
    gf16v_madd( y , tmp , accu_bit ,  vec_len );

    // x8
    accu_bit = 1<<3; // 8
    gf256v_set_zero( tmp , vec_len );
    for(int i=accu_bit;i<_GFSIZE;i+=accu_bit*2) {
      for(int j=0;j<accu_bit;j++) gf256v_add( tmp , tmp_res+TMPVEC_LEN*(i+j) , vec_len );
    }
    gf16v_madd( y , tmp , accu_bit ,  vec_len );
}



void rainbow_publicmap( unsigned char * y, const unsigned char * trimat, const unsigned char * x )
{
    unsigned char tmp[TMPVEC_LEN*_GFSIZE] = {0};
    unsigned char _x[_MAX_N];
    for(unsigned i=0;i<_PUB_N;i++) _x[i] = gfv_get_ele( x , i );

    accu_eval_quad( tmp , trimat , _x , _PUB_N , _PUB_M_BYTE );
    madd_reduce( y , tmp , _PUB_M_BYTE );
}

