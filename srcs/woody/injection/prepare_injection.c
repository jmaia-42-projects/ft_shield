/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:52:59 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/13 22:56:06 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

bool	prepare_injection(t_file file, t_injection *injection)
{
	*injection = (t_injection) {-1, NULL, 0, 0, -1, 0, 0, 0};
	if (file.type == ELF64)
		return (prepare_injection_elf64(file, injection));
	return (false);
}

void end_injection(t_injection injection)
{
	if (injection.file_map != NULL)
		munmap(injection.file_map, injection.file_size);
	if (injection.fd != -1)
		close(injection.fd);
}
