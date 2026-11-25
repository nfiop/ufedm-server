/*
 * SPDX-License-Identifier: GPL-2.0-only
 * Copyright (c) 2026 Liav A
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

#include "server/pid_lock.h"

/* Imported from libs/os-lock submodule */
#include "lock.h"

#ifdef _WIN32
#include <process.h>
#define PID_LOCK_FILE_PATH "C:\\Temp\\ufedm-server.pid"
#define GETPID() _getpid()
#else
#define PID_LOCK_FILE_PATH "/tmp/ufedm-server.pid"
#define GETPID() getpid()
#endif

errno_t acquire_pid_file_lock(void)
{
	int len;
	char pid_buf[32];
	int fd = open(PID_LOCK_FILE_PATH, O_CREAT | O_RDWR, 0644);

	if (fd < 0) {
		return -errno;
	}

	if (lock(fd, 0, 0, true, true) != 0) {
		close(fd);
		return -EBUSY;
	}

	len = snprintf(pid_buf, sizeof(pid_buf), "%ld\n", (long)GETPID());
	if (len < 0 || write(fd, pid_buf, (size_t)len) != len) {
		close(fd);
		return -EINVAL;
	}

	return 0;
}
