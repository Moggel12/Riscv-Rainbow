#ifndef SENDFN_H
#define SENDFN_H

#include "hal.h"

/**
 * \brief Send start signal to host.
 */
void send_start();

/**
 * \brief Send stop signal to host.
 */
void send_stop();

/**
 * \brief Send a named unsigned value.
 *
 * \param name Name of the integer.
 * \param c Value to send.
 */
void send_unsigned(const char *name, unsigned int c);

/**
 * \brief Send a named unsigned long long value.
 *
 * \param name Name of the integer.
 * \param c Value to send.
 */
void send_unsignedll(const char *name, unsigned long long c);

/**
 * \brief Send a named nul terminated string.
 *
 * \param name Name of the string.
 * \param str String to send.
 */
void send_string(const char *name, const char* str);

/**
 * \brief Send a named byte buffer.
 *
 * \param name Name of the buffer.
 * \param buf The buffer to send.
 * \param len Length of the buffer.
 */
void send_bytes(const char *name, const unsigned char* buf, unsigned len);

#endif /* SENDFN_H */

