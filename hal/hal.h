#ifndef HAL_H
#define HAL_H

#include <stdint.h>

/** HAL clocking mode */
enum clock_mode {
  CLOCK_FAST, /** Run the clock as fast as possible */
  CLOCK_BENCHMARK /** Clock mode for reliable cycle counts */
};

/**
 * \brief Setup the HAL.
 *
 * \param clock  Mode for the clock.
 */
void hal_setup(const enum clock_mode clock);

/**
 * \brief Get character from the host.
 *
 * Blocks until a character is available.
 *
 * \return The character (8-bit).
 */
int hal_getc();

/**
 * \brief Send a character to the host.
 *
 * \param str String to send.
 */
void hal_putc(int c);

/**
 * \brief Send string to host.
 *
 * \param str String to send.
 */
void hal_puts(const char* str);

/**
 * \brief Get the current time.
 *
 * The definition of time may vary between HAL implementations, but
 * usually defined as number of cycles since boot.
 *
 * \return Time relative to boot.
 */
uint64_t hal_get_time(void);

/**
 * \brief Get the number of retired instructions.
 *
 * \return The number of retired instructions since boot.
 */
uint64_t hal_get_num_instr(void);

/**
 * \brief Target name generated by the build system.
 */
extern const char hal_name[];

#endif
