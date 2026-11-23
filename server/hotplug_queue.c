/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Liav A.
 *
 * Originally written for the sipbell project.
 * See https://github.com/supercomputer7/sipbell for more details.
 */

#include "server/hotplug_queue.h"
#include <string.h>

void hotplug_queue_init(hotplug_queue_t *q)
{
	q->head = q->tail = 0;
	pthread_mutex_init(&q->mutex, NULL);
	pthread_cond_init(&q->cond, NULL);
}

errno_t hotplug_queue_push(hotplug_queue_t *q, hotplug_event_t *event)
{
	pthread_mutex_lock(&q->mutex);

	int next = (q->tail + 1) % HOTPLUG_QUEUE_SIZE;
	if (next == q->head) {
		// queue full
		pthread_mutex_unlock(&q->mutex);
		return -ENOMEM;
	}

	memcpy(&q->items[q->tail], event, sizeof(hotplug_event_t));
	q->tail = next;

	pthread_cond_signal(&q->cond);
	pthread_mutex_unlock(&q->mutex);
	return 0;
}

void hotplug_queue_pop_blocking(hotplug_queue_t *q, hotplug_event_t *recv_event)
{
	pthread_mutex_lock(&q->mutex);

	while (q->head == q->tail) {
		pthread_cond_wait(&q->cond, &q->mutex);
	}

	memcpy(recv_event, &q->items[q->head], sizeof(hotplug_event_t));
	q->head = (q->head + 1) % HOTPLUG_QUEUE_SIZE;

	pthread_mutex_unlock(&q->mutex);
}
