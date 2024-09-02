/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:16 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/02 22:51:58 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	get_error_fds(t_pipex *fds)
{
	fds->error_fd1 = open("/tmp/error1.log",
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds->error_fd1 < 0)
		error_open();
	fds->error_fd2 = open("/tmp/error2.log",
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds->error_fd2 < 0)
	{
		close(fds->error_fd1);
		error_open();
	}
}

static int	pipex(char *av[], char **envp)
{
	t_pipex	fds;
	pid_t	pid1;
	pid_t	pid2;

	ft_memset(&fds, -1, sizeof(t_pipex));
	if (pipe(fds.fd) == -1)
		error_pipe();
	get_error_fds(&fds);
	pid1 = fork();
	if (pid1 == -1)
		error_fork(&fds);
	if (pid1 == 0)
		first_child(av, &fds, envp);
	pid2 = fork();
	if (pid2 == -1)
		error_fork(&fds);
	if (pid2 == 0)
		second_child(av, &fds, envp);
	close_pipes(&fds);
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
