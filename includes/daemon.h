/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 17:48:55 by jmaia             #+#    #+#             */
/*   Updated: 2024/06/02 14:34:05 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SHIELD_DAEMON_H
# define FT_SHIELD_DAEMON_H

# include <stdbool.h>

# define LOCK_FILE_PATH "/var/lock/ft_shield"
# define BIN_PATH "/bin/ft_shield"

bool acquire_lock();
bool daemonize();
bool install(char const *program_name);
bool release_lock();
void setup_cleanup();

#endif
