#include "slice.h"
#include <stdio.h>

uint32_t binary_conversion(uint32_t num)
{
    if (num == 0)
    {
        return 0;
    }
    else
    {
        return (num % 2) + 10 * binary_conversion(num / 2);
    }
}

void slice(uint8_t *coefficients, int c_amount, uint32_t *sliced) 
{
    int s;
    uint32_t expanded, bit;

    if (c_amount > 32) {
        c_amount = 32; // Ensure that we only get as many bits as we can fit
    }
    /*printf("%d\n", c_amount);*/

    /*printf("Coefficients:\n");*/
    /*for (int i = 0; i < c_amount; i++) {*/
        /*printf("%08u\n", binary_conversion(coefficients[i]));*/
    /*}*/

    for (int i = 0; i < 4; i++) {
        /*printf("Round %d\n", i);*/
        for (int j = 0; j < c_amount; j++) {
            expanded = (uint32_t) coefficients[j];
            bit = (((expanded >> i)) & ((uint32_t) 1)) << j;
            sliced[i] = sliced[i] | bit;
            /*printf("%d: %032u\n%d: %032u = %032u\n", j, binary_conversion((expanded >> i)), j, binary_conversion(1), binary_conversion(bit << j));*/
        }
        /*printf("R%d = %032u\n", i, binary_conversion(sliced[i]));*/
        /*printf("\n");*/
    }

    /*for (int i = 0; i < 4; i++) {*/
        /*printf("sliced[%d] = %032u\n", i, binary_conversion(sliced[i]));*/
    /*}*/
}
