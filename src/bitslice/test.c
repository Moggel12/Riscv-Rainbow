#include <stdio.h>
#include "slice.h"
#include <stdlib.h>

void print_poly(hl_poly f) {
    printf("High-level:\n");
    binary_print(f.high.snd[0]); 
    binary_print(f.high.snd[1]); 
    binary_print(f.high.snd[2]);
    binary_print(f.high.snd[3]);
    binary_print(f.high.fst[0]); 
    binary_print(f.high.fst[1]); 
    binary_print(f.high.fst[2]); 
    binary_print(f.high.fst[3]);
    binary_print(f.high.cnst[0]); 
    binary_print(f.high.cnst[1]);
    binary_print(f.high.cnst[2]); 
    binary_print(f.high.cnst[3]);
    printf("Low-level:\n");
    binary_print(f.low.snd[0]);
    binary_print(f.low.snd[1]);
    binary_print(f.low.snd[2]);
    binary_print(f.low.snd[3]);
    binary_print(f.low.fst[0]);
    binary_print(f.low.fst[1]);
    binary_print(f.low.fst[2]);
    binary_print(f.low.fst[3]);
    binary_print(f.low.cnst[0]);
    binary_print(f.low.cnst[1]);
    binary_print(f.low.cnst[2]);
    binary_print(f.low.cnst[3]);
}

void random_polysystem(hl_poly *polys) {
    int i;
 
    srand(1);
    for (i = 0; i < 100; i++) {
        uint32_t snd_1[4] = {0,0,0,0};
        uint32_t fst_1[4] = {rand(),rand(),rand(),rand()};
        uint32_t cnst_1[4] = {rand(),rand(),rand(),rand()};
        uint32_t snd_2[4] = {0,0,0,0};
        uint32_t fst_2[4] = {rand(),rand(),rand(),rand()};
        uint32_t cnst_2[4] = {rand(),rand(),rand(),rand()};
        ll_poly temp_h = construct_ll_poly(snd_1, fst_1, cnst_1);
        ll_poly temp_l = construct_ll_poly(snd_2, fst_2, cnst_2);
        polys[i] = construct_hl_poly(temp_h, temp_l);
        // print_poly(polys[i]);
    }
}

int main(void)
{
    hl_poly polys[100];
    random_polysystem(polys);
    
    // ll_poly res = gf4_prod(polys[0].high, polys[1].high);
    
    // printf("First degree terms:\n");
    // printf("Slice1:");
    // binary_print(polys[0].high.fst[0]);
    // printf(" ");
    // binary_print(polys[0].high.fst[1]);
    // printf(" ");
    // binary_print(polys[0].high.fst[2]);
    // printf(" ");
    // binary_print(polys[0].high.fst[3]);
    // printf("\n");
    // printf("Slice2:");
    // binary_print(polys[1].high.fst[0]);
    // printf(" ");
    // binary_print(polys[1].high.fst[1]);
    // printf(" ");
    // binary_print(polys[1].high.fst[2]);
    // printf(" ");
    // binary_print(polys[1].high.fst[3]);
    // printf("\n");
    // printf("--------------------------------------------\n");
    // printf("       ");
    // binary_print(res.snd[0]);
    // printf(" ");
    // binary_print(res.snd[1]);
    // printf(" ");
    // binary_print(res.snd[2]);
    // printf(" ");
    // binary_print(res.snd[3]);

    // printf("\n\n");

    // printf("Second degree terms:\n");
    // printf("Slice1:");
    // binary_print(polys[0].high.cnst[0]);
    // printf(" ");
    // binary_print(polys[0].high.cnst[1]);
    // printf(" ");
    // binary_print(polys[0].high.cnst[2]);
    // printf(" ");
    // binary_print(polys[0].high.cnst[3]);
    // printf("\n");
    // printf("Slice2:");
    // binary_print(polys[1].high.cnst[0]);
    // printf(" ");
    // binary_print(polys[1].high.cnst[1]);
    // printf(" ");
    // binary_print(polys[1].high.cnst[2]);
    // printf(" ");
    // binary_print(polys[1].high.cnst[3]);
    // printf("\n");
    // printf("--------------------------------------------\n");
    // printf("       ");
    // binary_print(res.cnst[0]);
    // printf(" ");
    // binary_print(res.cnst[1]);
    // printf(" ");
    // binary_print(res.cnst[2]);
    // printf(" ");
    // binary_print(res.cnst[3]);

    // printf("\n\n");

    // binary_print(polys[0].high.snd[0]);
    // printf(" ");
    // binary_print(polys[0].high.snd[1]);
    // printf(" ");
    // binary_print(polys[0].high.snd[2]);
    // printf(" ");
    // binary_print(polys[0].high.snd[3]);
    // printf("\n");
    // binary_print(polys[1].high.snd[0]);
    // printf(" ");
    // binary_print(polys[1].high.snd[1]);
    // printf(" ");
    // binary_print(polys[1].high.snd[2]);
    // printf(" ");
    // binary_print(polys[1].high.snd[3]);
    // printf("\n--------------------------------------------\n");
    // binary_print(res.snd[0]);
    // printf(" ");
    // binary_print(res.snd[1]);
    // printf(" ");
    // binary_print(res.snd[2]);
    // printf(" ");
    // binary_print(res.snd[3]);
    binary_print((uint32_t) ~0);
    printf("\n");
    // uint32_t sliced[4] = {0, 0, 0, 0};
    // uint8_t coefficients[3] = {7, 3, 0};

    // slice(coefficients, 3, sliced); 
    return 0;
}