/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:40:02 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/21 13:01:12 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	allocate_pipes(t_pipex *fds)
{
	int	i;

	i = 0;
	fds->pipes = (int **)malloc(sizeof(int *) * (fds->num_cmds - 1));
	if (!fds->pipes)
		error_malloc(fds);
	while (i < fds->num_cmds - 1)
	{
		fds->pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!fds->pipes[i])
			error_malloc(fds);
		if (pipe(fds->pipes[i]) == -1)
			error_pipe();
		i++;
	}
}

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
	total_len = ft_strlen(base) + ft_strlen(index_str) + ft_strlen(extension) + 1;
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

static void	allocate_errors(t_pipex *fds)
{
	int		i;
	char	*error_filename;

	i = 0;
	error_filename = NULL;
	fds->error_fds = (int *)malloc(sizeof(int) * fds->num_cmds);
	if (!fds->error_fds)
		error_malloc(fds);
	while (i < fds->num_cmds)
	{
		create_error_filename(&error_filename, i + 1);
		fds->error_fds[i] = open(error_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		free(error_filename); //free back
		if (fds->error_fds[i] < 0)
		{
			while (i > 0)
			{
				i--;
				close(fds->error_fds[i]);
			}
			free(fds->error_fds);
			error_open(fds);
		}
		i++;
	}
}

static int	count_commands(int ac, char **av, t_pipex *fds)
{
	int	num_cmds;

	if (ft_strncmp(av[1], "here_doc\0", 9) == 0)
		num_cmds = ac - 4;
	else
		num_cmds = ac - 3;
	fds->num_cmds = num_cmds;
	return (num_cmds);
}

void	close_unused_pipes(t_pipex *fds, int cmd_pos)
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
	ft_memset(&fds, 0, sizeof(t_pipex));
	if (ft_strncmp(av[1], "here_doc\0", 9) == 0)
		handle_here_doc(&fds, av);
	else
		fd_in_init(&fds, av);

	count_commands(ac, av, &fds);
	allocate_pipes(&fds);
	allocate_errors(&fds);
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
