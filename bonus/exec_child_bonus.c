/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:38:25 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/20 21:37:14 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	fd_out_init(t_pipex *fds, char *av[])
{
	if (fds->here_doc)
	{
		fds->pipex[1] = open(av[fds->num_cmds + 3], O_DIRECTORY);
		if (fds->pipex[1] >= 0)
			error_directory(av[fds->num_cmds + 3], 126, fds);
		if (access(av[fds->num_cmds + 3], F_OK) == 0 && access(av[fds->num_cmds + 3], W_OK) == -1)
			error_permission(av[fds->num_cmds + 3], 1, fds);
		fds->pipex[1] = open(av[fds->num_cmds + 3], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fds->pipex[1] < 0)
		error_nofile(av[fds->num_cmds + 3], 1, fds);
	}
	else
	{
		fds->pipex[1] = open(av[fds->num_cmds + 2], O_DIRECTORY);
		if (fds->pipex[1] >= 0)
			error_directory(av[fds->num_cmds + 2], 126, fds);
		if (access(av[fds->num_cmds + 2], F_OK) == 0 && access(av[fds->num_cmds + 2], W_OK) == -1)
			error_permission(av[fds->num_cmds + 2], 1, fds);
		fds->pipex[1] = open(av[fds->num_cmds + 2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fds->pipex[1] < 0)
		error_nofile(av[fds->num_cmds + 2], 1, fds);
	}
}

void	fd_in_init(t_pipex *fds, char *av[])
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

void	exec_child(int cmd_pos, t_pipex *fds, char *av[], char **envp)
{
	int cmd_index;

	if (fds->here_doc)
		cmd_index = cmd_pos + 3;
	else
		cmd_index = cmd_pos + 2;
	if (dup2(fds->error_fds[cmd_pos], STDERR_FILENO) == -1)
		error_dup(fds);
	close(fds->error_fds[cmd_pos]);

	if (cmd_pos == 0)
	{
		dup2(fds->pipex[0], STDIN_FILENO); //check failed
		close(fds->pipex[0]);
	}
	else
	{
		close(fds->pipes[cmd_pos - 1][1]);
		dup2(fds->pipes[cmd_pos - 1][0], STDIN_FILENO);
		close(fds->pipes[cmd_pos - 1][0]);
	}
	if (cmd_pos == fds->num_cmds - 1)
	{
		fd_out_init(fds, av);
		dup2(fds->pipex[1], STDOUT_FILENO);
		close(fds->pipex[1]);
	}
	else
	{
		close(fds->pipes[cmd_pos][0]);
		dup2(fds->pipes[cmd_pos][1], STDOUT_FILENO);
		close(fds->pipes[cmd_pos][1]);
	}
	execute_command(av[cmd_index], envp, fds);
}
