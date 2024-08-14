/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:59 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/14 14:05:52 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <errno.h>
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

#endif