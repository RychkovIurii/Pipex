/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:40:02 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/21 14:23:37 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	create_error_filename(char **filename, int index)
{
	size_t		total_len;
	const char	*base;
	const char	*extension;
	char		*index_str;

	base = "/tmp/error";
	extension = ".log";
	index_str = ft_itoa(index);
	if (index_str == NULL)
		return ;
	total_len = ft_strlen(base) + ft_strlen(index_str)
		+ ft_strlen(extension) + 1;
	*filename = (char *)malloc(total_len);
	if (*filename == NULL)
	{
		free(index_str);
		return ;
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
	pid_t	*pids;
	int		i;

	i = 0;
	initialize_pipex(&fds, av, ac);
	pids = (pid_t *)malloc(sizeof(pid_t) * fds.num_cmds);
	if (!pids)
		error_malloc(&fds);
	while (i < fds.num_cmds)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			error_fork(&fds);
		if (pids[i] == 0)
			exec_child(i, &fds, av, envp);
		close_unused_pipes(&fds, i);
		i++;
	}
	close_pipes(&fds);
	return (wait_for_children(pids, &fds));
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
