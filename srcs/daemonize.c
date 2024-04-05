/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemonize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:39:32 by jmaia             #+#    #+#             */
/*   Updated: 2024/03/06 20:03:48 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static bool lose_process_group_lead();

bool daemonize()
{
	bool success;
	pid_t sid;

	success = lose_process_group_lead();
	if (! success)
		return false;

	sid = setsid();
	if (sid == -1)
		return false;

	success = lose_process_group_lead();
	if (! success)
		return false;

	return true;
}

static bool lose_process_group_lead()
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
		return false;
	if (pid != 0)
		exit(0);
	return true;
}
