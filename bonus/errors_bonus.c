/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 23:02:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/22 16:25:36 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	error_msg(char *msg, char *name)
{
	write(2, "zsh:1: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, ": ", 2);
	if (name && name[0] != '\0')
		write(2, name, ft_strlen(name));
	write(2, "\n", 1);
}

void	error_permission(char *name, int code, t_pipex *fds)
{
	error_msg("permission denied", name);
	free_pipex(fds);
	exit (code);
}

void	error_command(char *name, t_pipex *fds)
{
	error_msg("command not found", name);
	free_pipex(fds);
	exit (127);
}

void	error_nofile(char *name, int code, t_pipex *fds)
{
	error_msg("no such file or directory", name);
	free_pipex(fds);
	exit (code);
}

void	error_directory(char *name, int code, t_pipex *fds)
{
	error_msg("is a directory", name);
	free_pipex(fds);
	exit (code);
}
