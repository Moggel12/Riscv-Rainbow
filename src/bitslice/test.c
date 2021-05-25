#include <stdio.h>
#include "slice.h"
#include <stdlib.h>
#include <stdbool.h> 
#include <time.h>
#include <string.h>


void random_polysystem(hl_poly *polys) {
    int i;
 
    srand(1);
    for (i = 0; i < 64; i++) {
        uint32_t snd_1[2] = {0,0};
        uint32_t fst_1[2] = {rand(),rand()};
        uint32_t cnst_1[2] = {rand(),rand()};
        uint32_t snd_2[2] = {0,0};
        uint32_t fst_2[2] = {rand(),rand()};
        uint32_t cnst_2[2] = {rand(),rand()};
        ll_poly temp_h = construct_ll_poly(snd_1, fst_1, cnst_1);
        ll_poly temp_l = construct_ll_poly(snd_2, fst_2, cnst_2);
        polys[i] = construct_hl_poly(temp_h, temp_l);
        // print_poly(polys[i]);
    }
}

void random_arr(uint8_t arr[]) 
{
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < 32; i++) 
    {
        arr[i] = (uint8_t) rand();
    }
}

static inline
int is_empty( char c )
{
	if( ' ' == c ) return 1;
	if( '\t' == c ) return 1;
	if( '\n' == c ) return 1;
	return 0;
}

unsigned byte_fget( FILE * fp, unsigned char *v , unsigned n_byte )
{
	char c0 = 0, c1 = 0;
	char vv[3]; vv[2] = '\0';

	while( EOF != c0 ) {
		c0 = fgetc( fp );
		if( ('=' == c0) ) break;
	}

	int r = 0;
	while( 1 ) {
		while( is_empty( c0 = fgetc(fp) ) ) ;
		c1 = fgetc( fp );
		if( EOF == c0 ) break;
		if( EOF == c1 ) break;

		vv[0] = c0;
		vv[1] = c1;

		int value = 0;
		int t = sscanf( vv , "%2x" , &value );
		if( 1 == t ) {
			v[r] = value;
			r++;
		}
		else break;
		if( n_byte == (unsigned)r ) return r;
	}
	if( 0 < r ) return r;
	return -1;
}

int main(void)
{
//     hl_poly polys[64];
//     random_polysystem(polys);
    
//     uint8_t pk[CRYPTO_PUBLICKEYBYTES];

//     FILE * fp;
// 	int r;

// 	fp = fopen( "../../test/KAT_1619004850/KATpk" , "r");
// 	if( NULL == fp ) {
// 		printf("fail to open public key file.\n");
// 		return -1;
// 	}
// 	r = byte_fget( fp ,  pk , CRYPTO_PUBLICKEYBYTES );
// 	fclose( fp );
// 	if( CRYPTO_PUBLICKEYBYTES != r ) {
// 		printf("fail to load key file.\n");
// 		return -1;
// 	}

//     uint8_t signature[66] = {245, 87, 101, 125, 82, 182, 244, 60, 84, 74, 15, 8, 217, 239, 202, 190, 62, 191, 123, 57, 158, 222, 57, 217, 114, 191, 75, 88, 201, 39, 115, 92, 111, 224, 31, 242, 202, 160, 254, 157, 30, 42, 92, 198, 77, 210, 137, 0, 31, 157, 24, 194, 32, 23, 199, 19, 206, 161, 87, 233, 219, 60, 86, 31, 61, 85};

//     uint8_t digest[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

//     uint8_t digest2[32] = {175, 47, 228, 91, 3, 91, 101, 255, 96, 74, 100, 69, 231, 142, 164, 23, 46, 230, 249, 182, 46, 32, 191, 165, 65, 14, 166, 132, 43, 129, 161, 211};

//     int c = 0;

    // sliced_compute_publicmap(digest, signature, pk);

    // for (int i = 0; i < 64; i++)
    // {
    //     printf("%u ", digest[i]);
    // }


    // for (int j = 0; j < 100; j++)
    // {
    //     for (int i = j; i < 100; i++) {
    //         c++;
    //         printf("x_%ux_%u\n", i+1, j+1);
    //     }
    //     printf("===========\n");
    // }

    // printf("%u\n", c);

    // printf("%u\n", c);


    hl_poly t1 = expand_variable(5);
    hl_poly t2 = expand_variable(15);

    print_poly(t1);
    printf("\n");
    print_poly(t2);
    printf("\n");
    hl_poly res = gf16_prod(t1, t2);
    print_poly(res);
    printf("\n");
    uint8_t var;
    despand(res, &var);
    printf("%u\n", var);
    

    return 0;
}