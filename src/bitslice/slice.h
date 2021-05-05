// The following functions all assume Rainbow level I (GF16, 100 polynomials)

#include <stdint.h>

// Represents a GF4 value as a polynomial. This should have snd = 0 to be valid.
typedef struct 
{
    uint32_t snd[2];
    uint32_t fst[2];
    uint32_t cnst[2];
} ll_poly;

// Represents a GF16 value as a polynomial with a first degree term of ll_poly high and constant term of ll_poly low. Hence, a polynomial with polynomial coefficients
typedef struct 
{
    ll_poly high;
    ll_poly low;
} hl_poly;

void binary_print(uint32_t num);

// Compute a system of multivariate polynomials over GF16 (bitsliced)
void sliced_compute();

// Slice GF16 coefficients into
void slice(uint8_t *coefficients, int c_amount, uint32_t *sliced);

// Reconstruct uint8_t values from sliced result
void deslice(hl_poly res);

// Product of two GF16 values
hl_poly gf16_prod(hl_poly f, hl_poly g);

// Product of two GF4 values
ll_poly gf4_prod(ll_poly f, ll_poly g);

// Adding two GF4 values
ll_poly gf4_nonpure_add(ll_poly f, ll_poly g);

// Expand a value to an hl_poly.
hl_poly expand_variable(uint8_t val);

// Reduce a "nonpure" GF4 value to be "pure"
void gf4_reduce(ll_poly f);

// Construct a low-level polynomial (coefficient of the coefficients)
ll_poly construct_ll_poly(uint32_t snd[], uint32_t fst[], uint32_t cnst[]);

// Construct a high-level polynomial (a coefficient of the multivariate polynomials)  
hl_poly construct_hl_poly(ll_poly h, ll_poly l);
