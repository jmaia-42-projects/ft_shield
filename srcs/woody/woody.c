/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/14 20:13:48 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

static bool move(char const *src_filename, char const *dest_filename);

bool woody(char *filename)
{
	t_file file;
	t_injection injection;

	if (!get_file_type(filename, &file))
		return false;
	if (!get_specific_file(&file))
		return false;
	if (!prepare_injection(file, &injection))
		return false;
	close_file(file);

	bool already_signed = check_signature_present(injection);
	if (!already_signed) {
		inject(injection);
	} else {
		printf("File already signed\n");
	}

	end_injection(injection);

	if (!move(WOODY_TMP_FILE, filename))
		return false;
	return true;
}

int remove_injection(char *filename)
{
	t_file file;
	t_injection injection;

	if (!get_file_type(filename, &file))
		return -1;
	if (!get_specific_file(&file))
		return -1;
	if (!prepare_uninjection_elf64(file, &injection))
		return false;
	close_file(file);

	bool already_signed = check_signature_present(injection);
	if (!already_signed) {
		end_injection(injection);
		return 1;
	}

	uninject(injection);

	end_injection(injection);
	if (!move(WOODY_TMP_FILE, filename))
		return -1;
	return 0;
}

static bool move(char const *src_filename, char const *dest_filename)
{
	int success;
	size_t cp_cmd_len;
	char *cp_cmd = NULL;
	bool func_result = false;

	cp_cmd_len = snprintf(NULL, 0, "mv %s %s", src_filename, dest_filename);
	cp_cmd = malloc(sizeof(char) * cp_cmd_len + 1);
	if (cp_cmd == NULL)
		goto cleanup;
	success = sprintf(cp_cmd, "mv %s %s", src_filename, dest_filename) >= 0;
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
