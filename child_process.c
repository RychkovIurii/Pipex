/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:59:12 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/29 11:55:58 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	fd_out_init(int *pipex, char *av[], int fd[2])
{
	int	temp_fd;

	temp_fd = open(av[3], O_DIRECTORY);
	if (temp_fd >= 0)
	{
		close(temp_fd);
		error_permission(av[3], 126, fd, pipex);
	}
/* 	if (access(av[3], F_OK) == 0 && access(av[3], X_OK) == -1)
		error_permission(av[3], 126, fd, pipex); */
	if (access(av[4], F_OK) == 0 && access(av[4], W_OK) == -1)
		error_permission(av[4], 1, fd, pipex);
	else
	{
		pipex[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex[1] < 0)
			error_nofile(av[4], 1, fd, pipex);
	}
	if (access(av[3], F_OK) == 0 && access(av[3], X_OK) == -1)
		error_permission(av[3], 126, fd, pipex);
}

static void	fd_in_init(int *pipex, char *av[], int fd[2])
{
	int	temp_fd;

	if (access(av[2], F_OK) == 0 && access(av[2], X_OK) == -1)
		error_permission(av[2], 126, fd, pipex);
	temp_fd = open(av[2], O_DIRECTORY);
	if (temp_fd >= 0)
	{
		close(temp_fd);
		error_permission(av[2], 126, fd, pipex);
	}
	if (access(av[1], F_OK) == 0 && access(av[1], R_OK) == -1)
		error_permission(av[1], 126, fd, pipex);
	else
	{
		pipex[0] = open(av[1], O_RDONLY);
		if (pipex[0] < 0)
			error_nofile(av[1], 127, fd, pipex);
	}
}

void	execute_command(char *cmd, char **envp, int fd[2], int pipex[2])
{
	if (!cmd || cmd[0] == '\0')
		error_permission(cmd, 126, fd, pipex);
	while (*cmd == ' ')
		cmd++;
	if (cmd[0] == '\0')
		error_command(cmd, fd, pipex, 0);
	exec_with_zsh(cmd, envp, fd, pipex);
}

void	first_child(char *av[], int *pipex, int *fd, char **envp)
{
	fd_in_init(pipex, av, fd);
	if (dup2(pipex[0], STDIN_FILENO) == -1)
		error_dup(fd, pipex);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_dup(fd, pipex);
	close(fd[0]);
	close(fd[1]);
	close(pipex[0]);
	execute_command(av[2], envp, fd, pipex);
}

void	second_child(char *av[], int *pipex, int *fd, char **envp)
{
	fd_out_init(pipex, av, fd);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_dup(fd, pipex);
	if (dup2(pipex[1], STDOUT_FILENO) == -1)
		error_dup(fd, pipex);
	close(fd[0]);
	close(fd[1]);
	close(pipex[1]);
	execute_command(av[3], envp, fd, pipex);
}
