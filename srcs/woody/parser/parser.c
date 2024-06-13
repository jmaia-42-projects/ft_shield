/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:37:30 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/13 23:03:55 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

void close_file(t_file file)
{
	if (file.specific_file != NULL)
	{
		if (file.type == ELF64)
		{
			t_file_elf64 *elf64 = (t_file_elf64 *)file.specific_file;
			free(elf64->header);
			free(elf64->sections);
			free(elf64->programs);
		}
		free(file.specific_file);
	}
	if (file.map != NULL)
		munmap(file.map, file.size);
	if (file.fd != -1)
		close(file.fd);
}

bool	get_file_type(char *name, t_file *file)
{
	*file = (t_file){NONE, -1, NULL, 0, NULL};

	file->fd = open(name, O_RDONLY);
	if (file->fd == -1)
	{
		close_file(*file);
		return false;
	}
	char buffer[5];
	int ret = read(file->fd, buffer, 5);
	if (ret == -1)
	{
		close_file(*file);
		return false;
	}
	if (ret != 5)
	{
		close_file(*file);
		return false;
	}
	if (buffer[0] == 0x7f && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F')
	{
		if (buffer[4] == 1)
			file->type = ELF32;
		else if (buffer[4] == 2)
			file->type = ELF64;
		else
		{
			close_file(*file);
			return false;
		}
	}
	if (file->type == NONE)
	{
		close_file(*file);
		return false;
	}
	return true;
}

bool get_specific_file(t_file *file)
{
	if (file->type == ELF64)
	{
		if (!parse_elf64(file->fd, file))
		{
			close_file(*file);
			return false;
		}
		return true;
	}
	return false;
}
