/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:36:49 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/29 01:12:02 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_execve(t_pipex *fds, int flag, char *cmd, char *argv)
{
	if (argv)
		free(argv);
	if (flag)
		free(cmd);
	write(2, "execve failed\n", 14);
	free_pipex(fds);
	exit(1);
}

void	error_fork(t_pipex *fds)
{
	write(2, "fork failed\n", 12);
	remove_errorfiles(fds);
	free_pipex(fds);
	exit(1);
}

void	error_dup(t_pipex *fds)
{
	write(2, "dup2 failed\n", 12);
	free_pipex(fds);
	exit(1);
}

void	error_malloc(t_pipex *fds)
{
	write(2, "malloc failed\n", 14);
	free_pipex(fds);
	exit(1);
}

void	error_waitpid(t_pipex *fds)
{
	write(2, "waitpid failed\n", 15);
	remove_errorfiles(fds);
	free_pipex(fds);
	exit(1);
}
