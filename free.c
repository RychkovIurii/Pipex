/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:02:06 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/29 17:44:09 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	free_error_filename(t_pipex *fds)
{
	if (fds->error_filename1)
	{
		free(fds->error_filename1);
		fds->error_filename1 = NULL;
	}
	if (fds->error_filename2)
	{
		free(fds->error_filename2);
		fds->error_filename2 = NULL;
	}
}

void	free_pipex(t_pipex *fds)
{
	close_pipes(fds);
	free_error_filename(fds);
}

void	remove_errorfiles(t_pipex *fds)
{
	if (fds->error_filename1)
		if (unlink(fds->error_filename1) == -1)
			error_unlink();
	if (fds->error_filename2)
		if (unlink(fds->error_filename2) == -1)
			error_unlink();
}
