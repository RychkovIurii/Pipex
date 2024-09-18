/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_and_print_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 23:03:56 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/19 00:03:15 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
	int		i;
	char	*error_filename;

	i = 0;
	while (i < fds->num_cmds)
	{
		create_error_filename(&error_filename, i + 1);
		handle_file_errors(error_filename, fds);
		if (unlink(error_filename) == -1)
			error_unlink();
		free(error_filename);
		i++;
	}
}

int	wait_for_children(pid_t *pids, t_pipex *fds)
{
	int	i;
	int	waitstatus;
	int	signal_number;

	i = 0;
	while (i < fds->num_cmds)
	{
		if (waitpid(pids[i], &waitstatus, 0) == -1)
			error_waitpid();
		i++;
	}
	check_error_files(fds);
	if (WIFSIGNALED(waitstatus))
	{
		signal_number = WTERMSIG(waitstatus);
		return (128 + signal_number);
	}
	if (WIFEXITED(waitstatus))
		return (WEXITSTATUS(waitstatus));
	else
		return (1);
}
