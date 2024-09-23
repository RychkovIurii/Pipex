/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:38:25 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/23 10:53:40 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	execute_command(char *cmd, char **envp, t_pipex *fds)
{
	if (!cmd || cmd[0] == '\0')
		error_permission(cmd, 126, fds);
	if (cmd[0] == ' ')
		error_command(cmd, fds);
	exec_with_zsh(cmd, envp, fds);
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
			handle_here_doc(fds);
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
