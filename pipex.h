/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:59 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/27 14:40:25 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"

void	first_child(char *av[], int *pipex, int *fd, char **envp);
void	second_child(char *av[], int *pipex, int *fd, char **envp);
void	error_permission(char *name, int code);
void	error_command(char *name);
void	error_nofile(char *name, int code);
void	error_directory(char *name);
/* int	is_cmd_valid(char **path, char *cmd);
char	**path_init(char **envp);
void	free_set(char **set); */

#endif