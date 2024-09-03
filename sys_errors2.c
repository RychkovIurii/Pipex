/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 22:03:32 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/03 10:05:52 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_strjoin(t_pipex *fds, int flag, char *cmd)
{
	if (flag)
		free(cmd);
	perror("strjoin failed");
	close_pipes(fds);
	exit(1);
}

void	error_open(t_pipex *fds)
{
	perror("open failed");
	close_pipes(fds);
	exit(1);
}

void	error_pipe(void)
{
	perror("pipe failed");
	exit(1);
}

void	error_unlink(void)
{
	perror("unlink failed");
	exit(1);
}
