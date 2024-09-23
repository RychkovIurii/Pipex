/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 23:04:30 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/22 16:35:38 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	error_strjoin(t_pipex *fds, int flag, char *cmd)
{
	if (flag)
		free(cmd);
	write(2, "strjoin failed\n", 15);
	free_pipex(fds);
	exit(1);
}

void	error_open(t_pipex *fds)
{
	write(2, "open failed\n", 12);
	free_pipex(fds);
	exit(1);
}

void	error_pipe(t_pipex *fds)
{
	write(2, "pipe failed\n", 12);
	free_pipex(fds);
	exit(1);
}

void	error_unlink(t_pipex *fds)
{
	write(2, "unlink failed\n", 14);
	free_pipex(fds);
	exit(1);
}

void	error_err_filename(t_pipex *fds)
{
	write(2, "error filename failed\n", 22);
	free_pipex(fds);
	exit(1);
}
