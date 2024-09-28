/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:59:12 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/28 21:31:30 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	fd_out_init(t_pipex *fds, char *av[])
{
	fds->pipex[1] = open(av[4], O_DIRECTORY);
	if (fds->pipex[1] >= 0)
		error_directory(av[4], 126, fds);
	if (access(av[4], F_OK) == 0 && access(av[4], W_OK) == -1)
		error_permission(av[4], 1, fds);
	fds->pipex[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds->pipex[1] < 0)
		error_nofile(av[4], 1, fds);
}

static void	fd_in_init(t_pipex *fds, char *av[])
{
	if (access(av[1], F_OK) == 0 && access(av[1], R_OK) == -1)
		error_permission(av[1], 126, fds);
	else
	{
		fds->pipex[0] = open(av[1], O_RDONLY);
		if (fds->pipex[0] < 0)
			error_nofile(av[1], 127, fds);
	}
}

static void	execute_command(char *cmd, char **envp, t_pipex *fds)
{
	if (!cmd || cmd[0] == '\0')
		error_permission(cmd, 126, fds);
	if (cmd[0] == ' ')
		error_command(cmd, fds);
	exec_with_zsh(cmd, envp, fds);
}

void	first_child(char *av[], t_pipex *fds, char **envp)
{
	close(fds->fd[0]);
	if (dup2(fds->error_fd1, STDERR_FILENO) == -1)
		error_dup(fds);
	close(fds->error_fd1);
	fd_in_init(fds, av);
	if (dup2(fds->pipex[0], STDIN_FILENO) == -1)
		error_dup(fds);
	close(fds->pipex[0]);
	if (dup2(fds->fd[1], STDOUT_FILENO) == -1)
		error_dup(fds);
	close(fds->fd[1]);
	execute_command(av[2], envp, fds);
}

void	second_child(char *av[], t_pipex *fds, char **envp)
{
	close(fds->fd[1]);
	if (dup2(fds->error_fd2, STDERR_FILENO) == -1)
		error_dup(fds);
	close(fds->error_fd2);
	fd_out_init(fds, av);
	if (dup2(fds->fd[0], STDIN_FILENO) == -1)
		error_dup(fds);
	close(fds->fd[0]);
	if (dup2(fds->pipex[1], STDOUT_FILENO) == -1)
		error_dup(fds);
	close(fds->pipex[1]);
	execute_command(av[3], envp, fds);
}
