/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:07:02 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/19 00:21:31 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_pipes(t_pipex *fds)
{
	int	i;

	i = 0;
	while (i < fds->num_cmds - 1)
	{
		if(fds->pipes[i][0] >= 0)
			close(fds->pipes[i][0]);
		if(fds->pipes[i][1] >= 0)
			close(fds->pipes[i][1]);
		free(fds->pipes[i]);
		i++;
	}
	free(fds->pipes);
	
	i = 0;
	while (i < fds->num_cmds)
	{
		if(fds->error_fds[i] >= 0)
			close(fds->error_fds[i]);
		i++;
	}
	free(fds->error_fds);

	if(fds->fd[0] >= 0)
		close(fds->fd[0]);
	if(fds->fd[1] >= 0)
		close(fds->fd[1]);
	if(fds->pipex[0] >= 0)
		close(fds->pipex[0]);
	if(fds->pipex[1] >= 0)
		close(fds->pipex[1]);
}

void	free_set(char **set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		free(set[i]);
		i++;
	}
	free(set);
}
