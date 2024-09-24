/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 22:03:32 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/24 23:32:15 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_strjoin(t_pipex *fds, int flag, char *cmd)
{
	if (flag)
		free(cmd);
	write(2, "strjoin failed\n", 15);
	close_pipes(fds);
	remove_errorfiles(fds);
	exit(1);
}

void	error_open(t_pipex *fds)
{
	write(2, "open failed\n", 12);
	close_pipes(fds);
	remove_errorfiles(fds);
	exit(1);
}

void	error_pipe(void)
{
	write(2, "pipe failed\n", 12);
	exit(1);
}

void	error_unlink(t_pipex *fds)
{
	write(2, "unlink failed\n", 14);
	remove_errorfiles(fds);
	exit(1);
}

void	error_err_filename(t_pipex *fds)
{
	write(2, "error filename failed\n", 22);
	close_pipes(fds);
	remove_errorfiles(fds);
	exit(1);
}
