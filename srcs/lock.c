/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:36:28 by jmaia             #+#    #+#             */
/*   Updated: 2024/06/13 22:36:55 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "daemon.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool acquire_lock()
{
	while (access("/run/lock", F_OK) == -1)
		sleep(1);
	int fd = open(LOCK_FILE_PATH, O_CREAT | O_EXCL, 0644);
	if (fd == -1)
		return false;
	close(fd);
	return true;
}

bool release_lock()
{
	bool success = unlink(LOCK_FILE_PATH) == 0;
	return success;
}
