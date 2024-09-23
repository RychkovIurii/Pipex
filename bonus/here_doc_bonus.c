/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:52:50 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/23 18:24:59 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	create_error_filename(char **filename, int index, t_pipex *fds)
{
	size_t		total_len;
	const char	*base;
	const char	*extension;
	char		*index_str;

	base = "/tmp/error";
	extension = ".log";
	index_str = ft_itoa(index);
	if (index_str == NULL)
		error_err_filename(fds);
	total_len = ft_strlen(base) + ft_strlen(index_str)
		+ ft_strlen(extension) + 1;
	*filename = (char *)malloc(total_len);
	if (*filename == NULL)
	{
		free(index_str);
		error_err_filename(fds);
	}
	ft_strlcpy(*filename, base, total_len);
	ft_strlcat(*filename, index_str, total_len);
	ft_strlcat(*filename, extension, total_len);
	free(index_str);
}

static int	is_continue(char *line, char *delimiter)
{
	if (ft_strlen(delimiter) == 0 && ft_strlen(line) == 1
		&& line[0] == '\n')
	{
		free(line);
		return (0);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& line[ft_strlen(delimiter)] == '\n')
	{
		free(line);
		return (0);
	}
	return (1);
}

static int	here_doc_input(char *delimiter, t_pipex *fds)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		error_pipe(fds);
	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(0);
		if (!is_continue(line, delimiter))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	handle_here_doc(t_pipex *fds)
{
	fds->pipex[0] = here_doc_input(fds->delimiter, fds);
}
