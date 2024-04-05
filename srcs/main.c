/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:13:32 by jmaia             #+#    #+#             */
/*   Updated: 2024/04/05 20:10:07 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include "polling.h"

bool daemonize();

int main()
{
	//THE ONLY AUTHORISED PRINT
	printf("jmaia dhubleur\n");
	
	// check process not already running
	// daemonize();
	int sockfd = prepare_socket();
	if (sockfd == -1)
		return (1);
	poll_routine(sockfd);
}
