#include "slice.h"

// Multiplies two GF16 values, represented as polynomials with polynomial coefficients
hl_poly gf16_prod(hl_poly f, hl_poly g) 
{ 

    ll_poly t2, t1, t1_0, t1_1, t0;

    // Compute the second degree term
    t2 = gf4_prod(f.high, g.high);

    // Compute the first degree term
    t1_0 = gf4_prod(f.high, g.low);
    t1_1 = gf4_prod(f.low, g.high);
    t1 = gf4_nonpure_add(t1_0, t1_1);
    
    // Compute the constant term
    t0 = gf4_prod(f.low, g.low);

    // Make sure all temporary values are "pure" GF4 values
    gf4_reduce(t2);
    gf4_reduce(t1);
    gf4_reduce(t0);

    // Reduce the "high-level" polynomial in case t2 is nonzero
    t1 = gf4_nonpure_add(t2, t1);
    t0 = gf4_nonpure_add(t2, t0);

    hl_poly res = {t1, t0};
    return res;

}

// Add two gf4 values, represented as polynomials. These may not be "pure" GF4 values.
ll_poly gf4_nonpure_add(ll_poly f, ll_poly g) 
{
    int i;
    ll_poly res = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (i = 0; i < 4; i++) {
        res.snd[i] = f.snd[i] ^ g.snd[i];
        res.fst[i] = f.fst[i] ^ g.fst[i];
        res.cnst[i] = f.cnst[i] ^ g.cnst[i];
    }
    return res;
}

void gf4_reduce(ll_poly f) 
{
    int i;
    for (i = 0; i < 4; i++) {
        f.fst[i] = f.snd[i] ^ f.fst[i];
        f.cnst[i] = f.snd[i] ^ f.cnst[i];
        f.snd[i] ^= f.snd[i];
    }
}

// Product of two gf4 values, represented as polynomials. Expects ll_poly.snd = {0,0,0,0} and returns a "nonpure" GF4 value
ll_poly gf4_prod(ll_poly f, ll_poly g) 
{
    int i;
    ll_poly res = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (i = 0; i < 4; i++) {
        res.snd[i] = f.fst[i] & g.fst[i];
        res.fst[i] = (f.fst[i] & g.cnst[i]) ^ (f.cnst[i] & g.fst[i]);
        res.cnst[i] = f.cnst[i] & g.cnst[i];
    }
    return res;
}