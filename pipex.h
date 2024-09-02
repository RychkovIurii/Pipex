/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:59 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/02 21:22:14 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_pipex
{
	int	pipex[2];
	int	fd[2];
	int	error_fd1;
	int	error_fd2;
} t_pipex;

void	first_child(char *av[], t_pipex *fds, char **envp);
void	second_child(char *av[], t_pipex *fds, char **envp);
void	exec_with_zsh(char *cmd, char **envp, t_pipex *fds);
void	error_permission(char *name, int code, t_pipex *fds);
void	error_command(char *name, t_pipex *fds, int flag);
void	error_nofile(char *name, int code, t_pipex *fds);
void	error_directory(char *name, int code, t_pipex *fds);
void	error_fork(t_pipex *fds);
void	error_malloc(t_pipex *fds);
void	error_dup(t_pipex *fds);
void	error_execve(int flag, char *cmd);
void	error_waitpid(void);
void	close_pipes(t_pipex *fds);
void	free_set(char **set);

#endif