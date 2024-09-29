/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 23:05:05 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/29 19:17:10 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
	remove_errorfiles(fds, fds->num_cmds);
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
	remove_errorfiles(fds, fds->num_cmds);
	free_pipex(fds);
	exit(1);
}
