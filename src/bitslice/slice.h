// The following functions all assume Rainbow level I (GF16, 64 polynomials)

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

/////////////////////////TEMPORARY///////////////////////////////

#define CRYPTO_PUBLICKEYBYTES 161600

void binary_print(uint32_t num);

uint8_t gf16v_get_ele(const uint8_t *a, unsigned i);

void slice_32(uint8_t *coefficients, uint32_t sliced[]);

/////////////////////////////////////////////////////////////////

// Compute a system of multivariate polynomials over GF16 (bitsliced)
void sliced_compute();

// Slices a column of the coefficients in the Macaulay matrix
hl_poly slice_column(uint8_t coefficients[]);

// Reconstruct uint8_t values from sliced result
void deslice(hl_poly res, uint8_t coefficients[]);

// Product of two GF16 values
hl_poly gf16_prod(hl_poly f, hl_poly g);

// Sum of two GF16 values
hl_poly gf16_add(hl_poly f, hl_poly g);

// Product of two GF4 values
ll_poly gf4_prod(ll_poly f, ll_poly g);

// Sum two GF4 values
ll_poly gf4_nonpure_add(ll_poly f, ll_poly g);

// Expand a value to an hl_poly.
hl_poly expand_variable(uint8_t val);

// Reduce a "nonpure" GF4 value to be "pure"
void gf4_reduce(ll_poly f);

// Construct a low-level polynomial (coefficient of the coefficients) (GF4 value, potentially "nonpure" in the sense of Tower-field representation)
ll_poly construct_ll_poly(uint32_t snd[], uint32_t fst[], uint32_t cnst[]);

// Construct a high-level polynomial (a coefficient of the multivariate polynomials) (GF16 value)
hl_poly construct_hl_poly(ll_poly h, ll_poly l);
