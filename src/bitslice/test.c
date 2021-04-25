#include <stdio.h>
#include "slice.h"
#include <stdint.h>

int main(void)
{
    uint32_t sliced[4] = {0, 0, 0, 0};
    uint8_t coefficients[3] = {7, 3, 0};
    printf("Test\n");


    slice(coefficients, 3, sliced); 
    return 0;
}
