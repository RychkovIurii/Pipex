/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:36:49 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/02 21:38:30 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_fork(t_pipex *fds)
{
	perror("fork failed");
	close_pipes(fds);
	exit(1);
}

void	error_dup(t_pipex *fds)
{
	perror("dup2 failed");
	close_pipes(fds);
	exit(1);
}

void	error_malloc(t_pipex *fds)
{
	perror("malloc failed");
	close_pipes(fds);
	exit(1);
}

void	error_execve(int flag, char *cmd)
{
	if (flag)
		free(cmd);
	perror("execve failed");
	exit(1);
}

void	error_waitpid(void)
{
	perror("waitpid failed");
	exit(1);
}
