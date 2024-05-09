/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:13:32 by jmaia             #+#    #+#             */
/*   Updated: 2024/05/09 17:51:28 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>

#include "daemon.h"
#include "polling.h"

int main()
{
	int success;

	success = acquire_lock();
	if (! success)
		return (1);

	setup_cleanup();

	//THE ONLY AUTHORISED PRINT
	printf("jmaia dhubleur\n");
	
	// daemonize();
	int sockfd = prepare_socket();
	if (sockfd == -1)
		return (2);
	poll_routine(sockfd);
}
