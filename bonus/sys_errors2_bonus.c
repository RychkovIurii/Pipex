/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 23:04:30 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/18 23:04:52 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	error_strjoin(t_pipex *fds, int flag, char *cmd)
{
	if (flag)
		free(cmd);
	write(2, "strjoin failed\n", 15);
	close_pipes(fds);
	exit(1);
}

void	error_open(t_pipex *fds)
{
	write(2, "open failed\n", 12);
	close_pipes(fds);
	exit(1);
}

void	error_pipe(void)
{
	write(2, "pipe failed\n", 12);
	exit(1);
}

void	error_unlink(void)
{
	write(2, "unlink failed\n", 14);
	exit(1);
}
