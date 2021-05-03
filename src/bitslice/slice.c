#include "slice.h"
#include <stdio.h>

// Only fills first degree term and constant term
void fill_consistent(uint32_t *term[], uint32_t filler)
{
    for (int i = 0; i < 4; i++) {
        term[i] = filler;
    }
}

void binary_print(uint32_t number)
{
    uint32_t a[32], n = number, val = 0;
    int j, i = 0;
    while(i < 32)
    {
        a[i++] = number % 2;
        number = number / 2;
    }
    for(j = i - 1; j >= 0; j--)  
    {
        printf("%d", a[j]);
    }
}

void slice(uint8_t *coefficients, int c_amount, uint32_t *sliced) 
{
    int s;
    uint32_t expanded, bit;

    if (c_amount > 32) 
    {
        c_amount = 32; // Ensure that we only get as many bits as we can fit
    }

    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < c_amount; j++) 
        {
            expanded = (uint32_t) coefficients[j];
            bit = (((expanded >> i)) & ((uint32_t) 1)) << j;
            sliced[i] = sliced[i] | bit;
            /*printf("%d: %032u\n%d: %032u = %032u\n", j, binary_conversion((expanded >> i)), j, binary_conversion(1), binary_conversion(bit << j));*/
        }
        /*printf("R%d = %032u\n", i, binary_conversion(sliced[i]));*/
        /*printf("\n");*/
    }
}

void sliced_compute_publicmap() 
{
    int i;

    for (i = 0; i < 4; i++) {

    }
}

hl_poly expand_variable(uint8_t val) 
{
    uint32_t filler;
    uint32_t high_fst[4];
    uint32_t high_cnst[4];
    uint32_t low_fst[4];
    uint32_t low_cnst[4];

    uint32_t *all_vals[4] = {high_fst, high_cnst, low_fst, low_cnst};

    for (int i = 0; i < 4; i++) 
    {
        if ((val & (1 << i)) > 0) 
        {
            filler = ~0;
        } else {
            filler = 0;
        }
        for (int j = 0; j < 4; j++) 
        {
            (all_vals + i)[j] = ~1
        }
    }
 
    return res;
}

ll_poly construct_ll_poly(uint32_t snd[], uint32_t fst[], uint32_t cnst[]) 
{
    ll_poly res;
    int i;
    for (i = 0; i < 4; i++) 
    {
        res.snd[i] = snd[i];
        res.fst[i] = fst[i];
        res.cnst[i] = cnst[i];
    }
    return res;
}

hl_poly construct_hl_poly(ll_poly h, ll_poly l) 
{
    hl_poly res;
    res.high = h;
    res.low = l;
    return res;
}