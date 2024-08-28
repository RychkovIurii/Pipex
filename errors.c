/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:40:57 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/28 12:59:21 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	error_msg(char *msg, char *name)
{
	write(2, "zsh:1: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, ": ", 2);
	if (name && name[0] != '\0')
		write(2, name, ft_strlen(name));
	write(2, "\n", 1);
}

void	error_permission(char *name, int code, int fd[2], int pipex[2])
{
	error_msg("permission denied", name);
	close_pipes(fd, pipex);
	exit (code);
}

void	error_command(char *name, int fd[2], int pipex[2])
{
	error_msg("command not found", name);
	close_pipes(fd, pipex);
	exit (127);
}

void	error_nofile(char *name, int code, int fd[2], int pipex[2])
{
	error_msg("no such file or directory", name);
	close_pipes(fd, pipex);
	exit (code);
}
