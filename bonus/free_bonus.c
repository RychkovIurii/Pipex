/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:07:02 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/28 20:39:25 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_pipes(t_pipex *fds, int count)
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

static void	close_error_fds(t_pipex *fds)
{
	int	i;

	i = 0;
	if (fds->error_fds)
	{
		while (i < fds->num_cmds)
		{
			if (fds->error_fds[i] >= 0)
				close(fds->error_fds[i]);
			i++;
		}
		free(fds->error_fds);
		fds->error_fds = NULL;
	}
}

static void	close_middle_fds(t_pipex *fds)
{
	int	i;

	i = 0;
	if (fds->pipes)
	{
		while (i < fds->num_cmds - 1)
		{
			if (fds->pipes[i][0] >= 0)
				close(fds->pipes[i][0]);
			if (fds->pipes[i][1] >= 0)
				close(fds->pipes[i][1]);
			free(fds->pipes[i]);
			i++;
		}
		free(fds->pipes);
		fds->pipes = NULL;
	}
}

void	close_pipes(t_pipex *fds)
{
	close_middle_fds(fds);
	close_error_fds(fds);
	if (fds->pipex[0] >= 0)
		close(fds->pipex[0]);
	if (fds->pipex[1] >= 0)
		close(fds->pipex[1]);
}

void	free_pipex(t_pipex *fds)
{
	if (!fds)
		return ;
	if (fds->pids)
		free(fds->pids);
	close_pipes(fds);
	free_error_filenames(fds, fds->num_cmds);
}
