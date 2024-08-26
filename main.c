/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:16 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/26 17:52:02 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_msg(char *msg, char *name)
{
	write(2, "pipex: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, ": ", 2);
	if (name && name[0] != '\0')
		write(2, name, ft_strlen(name));
	write(2, "\n", 1);
}

int	pipex(char *av[], char **envp)
{
	int		pipex[2];
	int		fd[2];
	int		pid1;
	int		pid2;
	int		waitstatus1;
	int		waitstatus2;

	if (pipe(fd) == -1)
		exit (1);
	pid1 = fork();
	if (pid1 == -1)
		exit (1);
	if (pid1 == 0)
		first_child(av, pipex, fd, envp);
	pid2 = fork();
	if (pid2 == -1)
		exit (1);
	if (pid2 == 0)
		second_child(av, pipex, fd, envp);
	close(fd[0]);
	close(fd[1]);
	close(pipex[0]);
	close(pipex[1]);

	if (waitpid(pid1, &waitstatus1, 0) == -1)
	{
		perror("waitpid for pid1 failed");
		exit (1);
	}
	if (waitpid(pid2, &waitstatus2, 0) == -1)
	{
		perror("waitpid for pid2 failed");
		exit (1);
	}
	if (WIFSIGNALED(waitstatus2))
	{
		int signal_number = WTERMSIG(waitstatus2);
		return (128 + signal_number);
	}
	if (WIFEXITED(waitstatus2))
	{
		return WEXITSTATUS(waitstatus2);
	}
	else
	{
		return 1;
	}
}

int	main(int ac, char *av[], char **envp)
{
	int	status;

	status = 1;
	if (ac == 5)
	{
		status = pipex(av, envp);
		return (status);
	}
	else
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (status);
	}
}
