#include "slice.h"
#include <stdio.h>

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

uint8_t gf16v_get_ele(const uint8_t *a, unsigned i) {
    uint8_t r = a[i >> 1];
    uint8_t r0 = r&0xf;
    uint8_t r1 = r>>4;
    uint8_t m = (uint8_t)(-(i&1));
    return (r1&m)|((~m)&r0);
}

void slice_32(const uint8_t *coefficients, uint32_t sliced[]) 
{
    uint32_t expanded, bit;

    for (int i = 0; i < 4; i++) 
    {
        for (int j = 31; j >= 0; j--) 
        {
            expanded = (uint32_t) gf16v_get_ele(coefficients, 31-j);
            bit = (((expanded >> i)) & ((uint32_t) 1)) << j;
            sliced[i] = sliced[i] | bit;
        }
    }
}

hl_poly slice_column(const uint8_t coefficients[]) 
{
    uint32_t sliced_upper32[4] = {0, 0, 0, 0};
    uint32_t sliced_lower32[4] = {0, 0, 0, 0};

    slice_32(coefficients, sliced_upper32); 

    slice_32(coefficients + 16, sliced_lower32);

    uint32_t zeros[4] = {0, 0, 0, 0};
    uint32_t high_fst[2] = {sliced_upper32[3], sliced_lower32[3]};
    uint32_t high_cnst[2] = {sliced_upper32[2], sliced_lower32[2]};
    uint32_t low_fst[2] = {sliced_upper32[1], sliced_lower32[1]};
    uint32_t low_cnst[2] = {sliced_upper32[0], sliced_lower32[0]};

    return construct_hl_poly(construct_ll_poly(zeros, high_fst, high_cnst), construct_ll_poly(zeros, low_fst, low_cnst));
}

void deslice(hl_poly res, uint8_t coefficients[]) 
{
    uint8_t idx = 0;
    for (int i = 63; i >= 0; i--)
    {
        uint32_t mask = (1 << (i%32));
        if (i < 32) 
            idx = 1;
        coefficients[63 - i] = 0;
        uint8_t b3 = 0;
        if ((res.high.fst[idx] & mask) > 0)
            b3 = 8;
        uint8_t b2 = 0;
        if ((res.high.cnst[idx] & mask) > 0)
            b2 = 4;
        uint8_t b1 = 0;
        if ((res.low.fst[idx] & mask) > 0)
            b1 = 2;
        uint8_t b0 = 0;
        if ((res.low.cnst[idx] & mask) > 0)
            b0 = 1;
        coefficients[63 - i] = coefficients[63 - i] ^ (b3 ^ (b2 ^ (b1 ^ b0)));
    }
}

void print_poly(hl_poly f) {
    printf("High-level:\n");
    binary_print(f.high.snd[0]); 
    printf(" ");
    binary_print(f.high.fst[0]); 
    printf(" "); 
    binary_print(f.high.cnst[0]);
    printf("\n"); 
    binary_print(f.high.snd[1]);
    printf(" ");
    binary_print(f.high.fst[1]); 
    printf(" ");
    binary_print(f.high.cnst[1]);
    printf("\nLow-level:\n");
    binary_print(f.low.snd[0]); 
    printf(" ");
    binary_print(f.low.fst[0]); 
    printf(" ");
    binary_print(f.low.cnst[0]); 
    printf("\n");
    binary_print(f.low.snd[1]);
    printf(" ");
    binary_print(f.low.fst[1]); 
    printf(" ");
    binary_print(f.low.cnst[1]);
}

void despand(hl_poly poly, uint8_t *var)
{
    *var = 0;
    *var = 0 ^ ((poly.high.fst[0] >> 28) ^ ((poly.high.cnst[0] >> 29) ^ (poly.low.fst[0] >> 30) ^ (poly.low.cnst[0] >> 31)));
}

void sliced_compute_publicmap(uint8_t *digest, const uint8_t *signature, const uint8_t *pk)
{
    /*
     * PSEUDO-CODE FOR PROCEDURE:
     * Initialize hl_poly *total* with zero values
     * Initialize a coefficient array *coeff* of size 32
     * For i = 1 to pk_size/64
     *     Compute the hl_poly *current* for column i
     *     Expand z[i] to a hl_poly
     *     gf16_prod(current, z[i])
     *     total = gf16_add(current, total)
     * deslice(total, coeff)
     */
    // hl_poly total = expand_variable(0);
    uint32_t idx = 0, i, j;
    for (j = 0; j < 100; j++)
    {
        for (i = j; i < 100; i++)
        {
            // hl_poly current = slice_column(&(pk[(idx++)*32]));
            uint8_t xi = gf16v_get_ele(signature, i);
            uint8_t xj = gf16v_get_ele(signature, j);
            // printf("x_%ux_%u: %u, %u\n", j+1, i+1, xj, xi);
            printf("x_%ux_%u: %u, %u\n", i + 1, j + 1, xi, xj);
            hl_poly x_ix_j = gf16_prod(expand_variable(xi), expand_variable(xj));
            uint8_t var;
            despand(x_ix_j, &var);
            printf("= %u\n", var);
            // hl_poly prod = gf16_prod(current, x_ix_j);
            // total = gf16_add(current, total);
            printf("===================\n");
        }
    }
    // deslice(total, digest);
}

hl_poly expand_variable(uint8_t val) 
{
    uint32_t filler;
    uint32_t static_arr[2] = {0,0};
    uint32_t high_fst[2];
    uint32_t high_cnst[2];
    uint32_t low_fst[2];
    uint32_t low_cnst[2];

    uint32_t *all_vals[4] = {low_cnst, low_fst, high_cnst, high_fst};

    for (uint32_t i = 0; i < 4; i++) 
    {
        if ((val & (1 << i)) > 0) 
        {
            filler = ~0;
        } else {
            filler = 0;
        }
        for (int j = 0; j < 2; j++) 
        {
            *(all_vals[i] + j) = filler;
        }
    }
    ll_poly high = construct_ll_poly(static_arr, high_fst, high_cnst);
    ll_poly low = construct_ll_poly(static_arr, low_fst, low_cnst);
    hl_poly res = construct_hl_poly(high, low);
    return res;
}

ll_poly construct_ll_poly(uint32_t snd[], uint32_t fst[], uint32_t cnst[]) 
{
    ll_poly res;
    int i;
    for (i = 0; i < 2; i++) 
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
