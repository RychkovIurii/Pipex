/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:40:02 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/22 17:09:57 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	create_error_filename(char **filename, int index, t_pipex *fds)
{
	size_t		total_len;
	const char	*base;
	const char	*extension;
	char		*index_str;

	base = "/tmp/error";
	extension = ".log";
	index_str = ft_itoa(index);
	if (index_str == NULL)
		error_err_filename(fds);
	total_len = ft_strlen(base) + ft_strlen(index_str)
		+ ft_strlen(extension) + 1;
	*filename = (char *)malloc(total_len);
	if (*filename == NULL)
	{
		free(index_str);
		error_err_filename(fds);
	}
	ft_strlcpy(*filename, base, total_len);
	ft_strlcat(*filename, index_str, total_len);
	ft_strlcat(*filename, extension, total_len);
	free(index_str);
}

void	execute_command(char *cmd, char **envp, t_pipex *fds)
{
	if (!cmd || cmd[0] == '\0')
		error_permission(cmd, 126, fds);
	if (cmd[0] == ' ')
		error_command(cmd, fds);
	exec_with_zsh(cmd, envp, fds);
}

static void	close_unused_pipes(t_pipex *fds, int cmd_pos)
{
	if (cmd_pos > 0)
	{
		close(fds->pipes[cmd_pos - 1][0]);
		close(fds->pipes[cmd_pos - 1][1]);
	}
}

static int	pipex(int ac, char *av[], char **envp)
{
	t_pipex	fds;
	int		status;
	int		i;

	i = 0;
	initialize_pipex(&fds, av, ac);
	while (i < fds.num_cmds)
	{
		fds.pids[i] = fork();
		if (fds.pids[i] == -1)
			error_fork(&fds);
		if (fds.pids[i] == 0)
			exec_child(i, &fds, av, envp);
		close_unused_pipes(&fds, i);
		i++;
	}
	close_pipes(&fds);
	status = wait_for_children(fds.pids, &fds);
	free_pipex(&fds);
	return (status);
}

int	main(int ac, char *av[], char **envp)
{
	int	status;

	status = 1;
	if (ac > 5)
		status = pipex(ac, av, envp);
	else
		ft_putstr_fd("Usage: ./pipex file1 cmd1 ... cmd2 file2\n", 2);
	return (status);
}
