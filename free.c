/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:02:06 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/02 21:17:19 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	close_pipes(t_pipex *fds)
{
	if (fds->fd[0] >= 0)
		close(fds->fd[0]);
	if (fds->fd[1] >= 0)
		close(fds->fd[1]);
	if (fds->pipex[0] >= 0)
		close(fds->pipex[0]);
	if (fds->pipex[1] >= 0)
		close(fds->pipex[1]);
	if (fds->error_fd1 >= 0)
		close(fds->error_fd1);
	if (fds->error_fd2 >= 0)
		close(fds->error_fd2);
}
