/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:36:49 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/29 16:35:43 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_fork(int fd[2], int pipex[2])
{
	perror("fork failed");
	close_pipes(fd, pipex);
	exit(1);
}

void	error_dup(int fd[2], int pipex[2])
{
	perror("dup2 failed");
	close_pipes(fd, pipex);
	exit(1);
}

void	error_malloc(int fd[2], int pipex[2])
{
	perror("malloc failed");
	close_pipes(fd, pipex);
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
