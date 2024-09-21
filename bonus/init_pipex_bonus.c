/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 13:55:55 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/21 14:24:33 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
		fds->error_fds[i] = open(error_filename, O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
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

void	initialize_pipex(t_pipex *fds, char *av[], int ac)
{
	ft_memset(fds, 0, sizeof(t_pipex));
	if (ft_strncmp(av[1], "here_doc\0", 9) == 0)
		handle_here_doc(fds, av);
	else
		fd_in_init(fds, av);
	count_commands(ac, av, fds);
	allocate_pipes(fds);
	allocate_errors(fds);
}
