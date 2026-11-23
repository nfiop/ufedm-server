/*
 * SPDX-License-Identifier: GPL-2.0-only
 * Copyright (c) 2026 Liav A
 */

#ifndef COMMON__ERRNO_H
#define COMMON__ERRNO_H

#include <stdint.h>

/* POSIX errno for errno_t type */
#include <errno.h>

/* Used for functions that return errno number -
 * either zero or a negative number (for POSIX error).
 * for example -
 * ```
 *	return -ENOMEM;
 * ```
 */
typedef int errno_t;

#endif
