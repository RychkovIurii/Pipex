/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 13:55:55 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/22 17:13:02 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

static void	free_pipes(t_pipex *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(fds->pipes[i][0]);
		close(fds->pipes[i][1]);
		free(fds->pipes[i]);
		i++;
	}
	free(fds->pipes);
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
			free_pipes(fds, i);
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
	char	*error_filename;

	i = 0;
	error_filename = NULL;
	fds->error_fds = (int *)malloc(sizeof(int) * fds->num_cmds);
	if (!fds->error_fds)
		error_malloc(fds);
	while (i < fds->num_cmds)
	{
		create_error_filename(&error_filename, i + 1, fds);
		fds->error_fds[i] = open(error_filename, O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
		free(error_filename);
		if (fds->error_fds[i] < 0)
		{
			free_error_fds(fds, i);
			error_open(fds);
		}
		i++;
	}
}

/* static void	initialize_fds(t_pipex *fds)
{
	int	i;

	i = 0;
	fds->pipex[0] = -1;
	fds->pipex[1] = -1;
	if (fds->pipes)
	{
		while (i < fds->num_cmds - 1)
		{
			fds->pipes[i][0] = -1;
			fds->pipes[i][1] = -1;
			i++;
		}
	}
} */

void	initialize_pipex(t_pipex *fds, char *av[], int ac)
{
	ft_memset(fds, 0, sizeof(t_pipex));
	fds->pipex[0] = -1;
	fds->pipex[1] = -1;
	count_commands(ac, av, fds);
	allocate_pipes(fds);
	/* initialize_fds(fds); */
	allocate_errors(fds);
	fds->pids = (pid_t *)malloc(sizeof(pid_t) * fds->num_cmds);
	if (!fds->pids)
		error_malloc(fds);
}
