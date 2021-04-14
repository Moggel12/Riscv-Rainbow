/// @file utils.c
/// @brief Implementations for utils.h
///

#include "utils.h"
#include <hal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Blocks until the host provides some text
int get_text_len() {
	/*
	 * The protocol used to communicate the message requires a null-byte before and after the size of the message, or (if the length of the message is <= 255) a single character
	 */
    int code = hal_getc();
    
    if (code == 0) {
        
        int old_code = 0;
        
        do {
            old_code = code;
            code += hal_getc();
            
            if ((old_code == 0) && (code == 0)) {
                return -1;
            }
            
        } while (old_code > 0);
    }

    return code;
}