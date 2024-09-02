/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:16 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/02 22:30:10 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void handle_file_errors(const char *filename)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd >= 0)
	{
		line = get_next_line(fd);
		while (line != NULL)
		{
			write(2, line, ft_strlen(line));
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
	}
	else
		error_open();
}

static void check_error_files(const char *file1, const char *file2)
{
	handle_file_errors(file1);
	handle_file_errors(file2);
}

static int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	waitstatus1;
	int	waitstatus2;
	int	signal_number;

	if (waitpid(pid1, &waitstatus1, 0) == -1)
		error_waitpid();
	if (waitpid(pid2, &waitstatus2, 0) == -1)
		error_waitpid();
	check_error_files("/tmp/error1.log", "/tmp/error2.log");
	unlink("/tmp/error1.log"); //check failed
	unlink("/tmp/error2.log"); //check failed
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
	t_pipex	fds;
	pid_t	pid1;
	pid_t	pid2;

	ft_memset(&fds, -1, sizeof(t_pipex));
	if (pipe(fds.fd) == -1)
		error_pipe();
	fds.error_fd1 = open("/tmp/error1.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds.error_fd1 < 0)
		error_open();
	fds.error_fd2 = open("/tmp/error2.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds.error_fd2 < 0)
	{
		close(fds.error_fd1);
		error_open();
	}
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
