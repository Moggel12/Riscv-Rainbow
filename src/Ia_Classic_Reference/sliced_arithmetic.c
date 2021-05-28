#include "slice.h"

// Multiplies two GF16 values, represented as polynomials with polynomial coefficients
void gf16_prod(hl_poly *res, hl_poly *f, hl_poly *g) 
{ 

    ll_poly t2, t1, t1_0, t1_1, t0;

    // Compute the second degree term
    gf4_prod(&t2, &f->high, &g->high);

    // Compute the first degree term
    gf4_prod(&t1_0, &f->high, &g->low);
    gf4_prod(&t1_1, &f->low, &g->high);
    gf4_nonpure_add(&t1, &t1_0, &t1_1);

    // Compute the constant term
    gf4_prod(&t0, &f->low, &g->low);

    ll_poly temp = {{0,0}, {~0, ~0}, {0,0}};
    
    gf4_reduce(&t2);

    // Obtain t2 * (1a + 0)
    gf4_prod(&temp, &temp, &t2);

    // Reduce t2 * (1a + 0)
    gf4_reduce(&temp);

    // Make sure all temporary values are "pure" GF4 values
    gf4_reduce(&t1);
    gf4_reduce(&t0);

    // Reduce the "high-level" polynomial in case t2 is nonzero
    gf4_nonpure_add(&res->high, &t2, &t1);
    gf4_nonpure_add(&res->low, &temp, &t0);
}

void gf16_add(hl_poly *res, hl_poly *f, hl_poly *g) 
{
    gf4_nonpure_add(&res->high, &f->high, &g->high);
    gf4_nonpure_add(&res->low, &f->low, &g->low);
}

// Add two gf4 values, represented as polynomials. These may not be "pure" GF4 values.
void gf4_nonpure_add(ll_poly *res, ll_poly *f, ll_poly *g) 
{
    //*res = {{0,0}, {0,0}, {0,0}};
    for (int i = 0; i < 2; i++) 
    {
        res->snd[i] = f->snd[i] ^ g->snd[i];
        res->fst[i] = f->fst[i] ^ g->fst[i];
        res->cnst[i] = f->cnst[i] ^ g->cnst[i];
    }
}

void gf4_reduce(ll_poly *f) 
{
    for (int i = 0; i < 2; i++) 
    {
        f->fst[i] = f->snd[i] ^ f->fst[i];
        f->cnst[i] = f->snd[i] ^ f->cnst[i];
        f->snd[i] = 0; //f->snd[i] ^ f->snd[i];
    }
}

// Product of two gf4 values, represented as polynomials. Expects ll_poly.snd = {0,0,0,0} and returns a "nonpure" GF4 value
void gf4_prod(ll_poly *res, ll_poly *f, ll_poly *g) 
{
    for (int i = 0; i < 2; i++) 
    {
        res->snd[i] = f->fst[i] & g->fst[i];
        res->fst[i] = (f->fst[i] & g->cnst[i]) ^ (f->cnst[i] & g->fst[i]);
        res->cnst[i] = f->cnst[i] & g->cnst[i];
    }
}

