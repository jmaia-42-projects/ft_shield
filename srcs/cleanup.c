/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 17:29:16 by jmaia             #+#    #+#             */
/*   Updated: 2024/05/09 17:52:36 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "daemon.h"

#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>

#define COUNTOF(x) (sizeof(x) / sizeof(*x))

static void cleanup_from_signal(int signo);
static void cleanup();

void setup_cleanup()
{
	int const CATCHED_SIGNALS[] = {SIGINT, SIGQUIT, SIGTERM};
	struct sigaction sa;

	// Cleanup from signals
	bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &cleanup_from_signal;
	sigemptyset(&sa.sa_mask);

	sa.sa_flags = 0;

	for (uint32_t i = 0; i < COUNTOF(CATCHED_SIGNALS); i++)
	{
		sigaddset(&sa.sa_mask, CATCHED_SIGNALS[i]);
		sigaction(CATCHED_SIGNALS[i], &sa, 0);
	}

	// Cleanup from exit
	atexit(cleanup);
}

static void cleanup_from_signal(int signo)
{
	(void) signo;

	cleanup();
}

static void cleanup()
{
	release_lock();
}
