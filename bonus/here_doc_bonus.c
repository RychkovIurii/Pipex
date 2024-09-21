/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:52:50 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/21 12:59:09 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	here_doc_input(char *delimiter)
{
	int pipe_fd[2];
	char *line;

	if (pipe(pipe_fd) == -1)
		error_pipe();

	while (1)
	{
		line = get_next_line(0);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[1]);
	return pipe_fd[0];
}

void	handle_here_doc(t_pipex *fds, char *av[])
{
	fds->here_doc = 1;
	fds->delimiter = av[2]; //check
	fds->pipex[0] = here_doc_input(fds->delimiter);
}
