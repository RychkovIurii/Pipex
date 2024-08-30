/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:59:12 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/30 17:21:53 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	fd_out_init(int *pipex, char *av[], int fd[2])
{
	pipex[1] = open(av[4], O_DIRECTORY);
	if (pipex[1] >= 0)
		error_directory(av[4], 126, fd, pipex);
	if (access(av[4], F_OK) == 0 && access(av[4], W_OK) == -1)
		error_permission(av[4], 1, fd, pipex);
	pipex[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex[1] < 0)
		error_nofile(av[4], 1, fd, pipex);
}

static void	fd_in_init(int *pipex, char *av[], int fd[2])
{
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

void	first_child(char *av[], int *pipex, int *fd, char **envp/* , int sync_fd */)
{
	close(fd[0]);
	fd_in_init(pipex, av, fd);
	if (dup2(pipex[0], STDIN_FILENO) == -1)
		error_dup(fd, pipex);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_dup(fd, pipex);
	close(fd[1]);
	close(pipex[0]);
/* 	write(sync_fd, "", 1);
	close(sync_fd); */
	execute_command(av[2], envp, fd, pipex);
}

void	second_child(char *av[], int *pipex, int *fd, char **envp)
{
	close(fd[1]);
	fd_out_init(pipex, av, fd);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_dup(fd, pipex);
	if (dup2(pipex[1], STDOUT_FILENO) == -1)
		error_dup(fd, pipex);
	close(fd[0]);
	close(pipex[1]);
/* 	int increase = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		increase = i;
	} */
	execute_command(av[3], envp, fd, pipex);
}
