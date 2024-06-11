/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:35:36 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/11 22:46:16 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>	
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <elf.h>
# include <unistd.h>

# include "elf_parser.h"

typedef enum e_file_type
{
	NONE,
	ELF64,
	ELF32,
}	t_file_type;

typedef struct s_file_elf64
{
	Elf64_Ehdr	*header;
	Elf64_Shdr	*sections;
	Elf64_Phdr	*programs;
}	t_file_elf64;

typedef struct s_file
{
	t_file_type	type;
	int			fd;
	void		*map;
	off_t		size;
	void		*specific_file;
}	t_file;

bool	get_file_type(char *name, t_file *file);
void close_file(t_file file);
bool get_specific_file(t_file *file);

#endif
