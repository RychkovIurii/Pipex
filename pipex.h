/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:59 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/22 17:04:39 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_struct
{
	char	**path;
	char	**cmd1;
	char	**cmd2;
	char	*file1;
	char	*file2;
	int		fd_in;
	int		fd_out;
}	t_struct;

void error_msg(char *msg, char *name);

#endif