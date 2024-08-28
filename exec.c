/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:13:24 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/28 17:40:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	exec_absolute_or_relative_path(char *cmd, char **envp,
													int fd[2], int pipex[2])
{
	char	**argv;

	argv = ft_split(cmd, ' ');
	if (!argv)
		error_split(fd, pipex);
	execve(argv[0], argv, envp);
	free_set(argv);
	error_nofile(cmd, 127, fd, pipex);
}

void	execute_command(char *cmd, char **envp, int fd[2], int pipex[2])
{
	if (!cmd || cmd[0] == '\0')
		error_permission(cmd, 126, fd, pipex);
	while (*cmd == ' ')
		cmd++;
	if (cmd[0] == '\0')
		error_command(cmd, fd, pipex);
	if (cmd[0] == '/' || cmd[0] == '.')
		exec_absolute_or_relative_path(cmd, envp, fd, pipex);
	else
		exec_with_zsh(cmd, envp, fd, pipex);
}
