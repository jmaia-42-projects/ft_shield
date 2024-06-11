/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/11 22:53:00 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "prepare_injection.h"
#include "injection.h"

int	woody(char *filename)
{
	t_file file;
	t_injection injection;

	if (! get_file_type(filename, &file))
		return (1);
	if (! get_specific_file(&file))
		return (1);
	if (! prepare_injection(file, &injection))
		return (1);
	close_file(file);
	inject(injection);
	end_injection(injection);
	return (0);
}
