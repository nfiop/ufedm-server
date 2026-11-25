/*
 * SPDX-License-Identifier: GPL-2.0-only
 * Copyright (c) 2026 Liav A
 */

#ifndef SERVER__PID_LOCK_H
#define SERVER__PID_LOCK_H

#include <stddef.h>

#include "common/errno.h"

errno_t acquire_pid_file_lock(void);

#endif
