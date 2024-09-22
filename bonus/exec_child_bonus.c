/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:38:25 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/22 14:27:05 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	fd_out_init(t_pipex *fds, char *av[])
{
	int	file_index;

	if (fds->here_doc)
		file_index = fds->num_cmds + 3;
	else
		file_index = fds->num_cmds + 2;
	fds->pipex[1] = open(av[file_index], O_DIRECTORY);
	if (fds->pipex[1] >= 0)
		error_directory(av[file_index], 126, fds);
	if (access(av[file_index], F_OK) == 0 && access(av[file_index], W_OK) == -1)
		error_permission(av[file_index], 1, fds);
	if (fds->here_doc)
		fds->pipex[1] = open(av[file_index], O_WRONLY
				| O_CREAT | O_APPEND, 0644);
	else
		fds->pipex[1] = open(av[file_index], O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
	if (fds->pipex[1] < 0)
		error_nofile(av[file_index], 1, fds);
}

static void	redirect_error(t_pipex *fds, int cmd_pos)
{
	if (dup2(fds->error_fds[cmd_pos], STDERR_FILENO) == -1)
		error_dup(fds);
	close(fds->error_fds[cmd_pos]);
}

static void	read_from_pipe(t_pipex *fds, int cmd_pos, char *av[])
{
	if (cmd_pos == 0)
	{
		if (ft_strncmp(av[1], "here_doc\0", 9) == 0)
			handle_here_doc(fds, av);
		else
			fd_in_init(fds, av);
		if (dup2(fds->pipex[0], STDIN_FILENO) == -1)
			error_dup(fds);
		close(fds->pipex[0]);
	}
	else
	{
		close(fds->pipes[cmd_pos - 1][1]);
		if (dup2(fds->pipes[cmd_pos - 1][0], STDIN_FILENO) == -1)
			error_dup(fds);
		close(fds->pipes[cmd_pos - 1][0]);
	}
}

static void	write_to_pipe(t_pipex *fds, int cmd_pos, char *av[])
{
	if (cmd_pos == fds->num_cmds - 1)
	{
		fd_out_init(fds, av);
		if (dup2(fds->pipex[1], STDOUT_FILENO) == -1)
			error_dup(fds);
		close(fds->pipex[1]);
	}
	else
	{
		close(fds->pipes[cmd_pos][0]);
		if (dup2(fds->pipes[cmd_pos][1], STDOUT_FILENO) == -1)
			error_dup(fds);
		close(fds->pipes[cmd_pos][1]);
	}
}

void	exec_child(int cmd_pos, t_pipex *fds, char *av[], char **envp)
{
	int	cmd_index;

	if (fds->here_doc)
		cmd_index = cmd_pos + 3;
	else
		cmd_index = cmd_pos + 2;
	redirect_error(fds, cmd_pos);
	read_from_pipe(fds, cmd_pos, av);
	write_to_pipe(fds, cmd_pos, av);
	execute_command(av[cmd_index], envp, fds);
}
