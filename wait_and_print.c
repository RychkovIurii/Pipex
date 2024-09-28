/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_and_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 22:45:09 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/28 21:15:04 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	handle_file_errors(const char *filename, t_pipex *fds)
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
		error_open(fds);
}

static void	check_error_files(t_pipex *fds)
{
	handle_file_errors(fds->error_filename1, fds);
	handle_file_errors(fds->error_filename2, fds);
}

int	wait_for_children(pid_t pid1, pid_t pid2, t_pipex *fds)
{
	int	waitstatus1;
	int	waitstatus2;
	int	signal_number;

	if (waitpid(pid1, &waitstatus1, 0) == -1)
		error_waitpid(fds);
	if (waitpid(pid2, &waitstatus2, 0) == -1)
		error_waitpid(fds);
	check_error_files(fds);
	remove_errorfiles(fds);
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
