/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:40:57 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/26 19:26:12 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	error_msg(char *msg, char *name)
{
	write(2, "pipex: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, ": ", 2);
	if (name && name[0] != '\0')
		write(2, name, ft_strlen(name));
	write(2, "\n", 1);
}

void	error_permission(char *name, int code)
{
	error_msg("permission denied", name);
	exit (code);
}

void	error_command(char *name)
{
	error_msg("command not found", name);
	exit (127);
}

void	error_nofile(char *name)
{
	error_msg("no such file or directory", name);
	exit (127);
}

void	error_directory(char *name)
{
	error_msg("is a directory", name);
	exit (127);
}
