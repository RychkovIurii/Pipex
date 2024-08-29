/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:16 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/29 11:22:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	waitstatus1;
	int	waitstatus2;
	int	signal_number;

	if (waitpid(pid1, &waitstatus1, 0) == -1)
		error_waitpid();
	if (waitpid(pid2, &waitstatus2, 0) == -1)
		error_waitpid();
	if (WIFSIGNALED(waitstatus2))
	{
		signal_number = WTERMSIG(waitstatus2);
		return (128 + signal_number);
	}
	if (WIFEXITED(waitstatus2))
		return (WEXITSTATUS(waitstatus2));
	else
		return (1);
}

static int	pipex(char *av[], char **envp)
{
	int		pipex[2];
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	ft_memset(pipex, -1, sizeof(pipex));
	ft_memset(fd, -1, sizeof(fd));
	if (pipe(fd) == -1)
	{
		perror("pipe failed");
		exit(1);
	}
	pid1 = fork();
	if (pid1 == -1)
		error_fork(fd, pipex);
	if (pid1 == 0)
		first_child(av, pipex, fd, envp);
	pid2 = fork();
	if (pid2 == -1)
		error_fork(fd, pipex);
	if (pid2 == 0)
		second_child(av, pipex, fd, envp);
	close_pipes(fd, pipex);
	return (wait_for_children(pid1, pid2));
}

int	main(int ac, char *av[], char **envp)
{
	int	status;

	status = 1;
	if (ac == 5)
		status = pipex(av, envp);
	else
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
	return (status);
}
