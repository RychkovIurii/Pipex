/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_and_print_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 23:03:56 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/29 19:23:34 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	handle_file_errors(char *filename, t_pipex *fds)
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
	{
		remove_errorfiles(fds, fds->num_cmds);
		error_open(fds);
	}
}

void	free_error_filenames(t_pipex *fds, int count)
{
	int	i;

	i = 0;
	if (!fds->error_filenames)
		return ;
	while (i < count)
	{
		free(fds->error_filenames[i]);
		i++;
	}
	free(fds->error_filenames);
	fds->error_filenames = NULL;
}

static void	check_error_files(t_pipex *fds)
{
	int		i;

	i = 0;
	while (i < fds->num_cmds)
	{
		handle_file_errors(fds->error_filenames[i], fds);
		if (unlink(fds->error_filenames[i]) == -1)
			error_unlink();
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
			error_waitpid(fds);
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
