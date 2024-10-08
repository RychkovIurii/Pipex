/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 13:55:55 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/29 21:37:21 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	count_commands(int ac, char **av, t_pipex *fds)
{
	int	num_cmds;

	if (ft_strncmp(av[1], "here_doc\0", 9) == 0)
	{
		num_cmds = ac - 4;
		fds->here_doc = 1;
		fds->delimiter = av[2];
	}
	else
		num_cmds = ac - 3;
	fds->num_cmds = num_cmds;
	return (num_cmds);
}

static void	allocate_pipes(t_pipex *fds)
{
	int	i;

	i = 0;
	fds->pipes = (int **)malloc(sizeof(int *) * (fds->num_cmds - 1));
	if (!fds->pipes)
		error_malloc(NULL);
	while (i < fds->num_cmds - 1)
	{
		fds->pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!fds->pipes[i])
		{
			free_pipes(fds, i);
			error_malloc(NULL);
		}
		if (pipe(fds->pipes[i]) == -1)
		{
			free_pipes(fds, i + 1);
			error_pipe(NULL);
		}
		i++;
	}
}

static void	free_error_fds(t_pipex *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(fds->error_fds[i]);
		i++;
	}
	free(fds->error_fds);
	fds->error_fds = NULL;
}

static void	allocate_errors(t_pipex *fds)
{
	int		i;

	i = 0;
	fds->error_fds = (int *)malloc(sizeof(int) * fds->num_cmds);
	if (!fds->error_fds)
		error_malloc(fds);
	fds->error_filenames = (char **)malloc(sizeof(char *) * fds->num_cmds);
	if (!fds->error_filenames)
		error_malloc(fds);
	while (i < fds->num_cmds)
	{
		create_error_filename(&(fds->error_filenames[i]), i + 1, fds);
		fds->error_fds[i] = open(fds->error_filenames[i], O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
		if (fds->error_fds[i] < 0)
		{
			free_error_fds(fds, i);
			remove_errorfiles(fds, i + 1);
			free_error_filenames(fds, i + 1);
			error_open(fds);
		}
		i++;
	}
}

void	initialize_pipex(t_pipex *fds, char *av[], int ac)
{
	ft_memset(fds, 0, sizeof(t_pipex));
	count_commands(ac, av, fds);
	allocate_pipes(fds);
	allocate_errors(fds);
	fds->pids = (pid_t *)malloc(sizeof(pid_t) * fds->num_cmds);
	if (!fds->pids)
	{
		remove_errorfiles(fds, fds->num_cmds);
		error_malloc(fds);
	}
}
