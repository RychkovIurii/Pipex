/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:59 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/28 16:47:34 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <stdint.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_pipex
{
	int		pipex[2];
	int		fd[2];
	int		error_fd1;
	int		error_fd2;
	char	*error_filename1;
	char	*error_filename2;
}	t_pipex;

void	create_error_filename(char **filename, int index, t_pipex *fds);
void	first_child(char *av[], t_pipex *fds, char **envp);
void	second_child(char *av[], t_pipex *fds, char **envp);
void	exec_with_zsh(char *cmd, char **envp, t_pipex *fds);
int		wait_for_children(pid_t pid1, pid_t pid2, t_pipex *fds);
void	error_permission(char *name, int code, t_pipex *fds);
void	error_command(char *name, t_pipex *fds);
void	error_nofile(char *name, int code, t_pipex *fds);
void	error_directory(char *name, int code, t_pipex *fds);
void	error_strjoin(t_pipex *fds, int flag, char *cmd);
void	error_execve(t_pipex *fds, int flag, char *cmd, char *argv);
void	error_fork(t_pipex *fds);
void	error_malloc(t_pipex *fds);
void	error_dup(t_pipex *fds);
void	error_waitpid(t_pipex *fds);
void	error_open(t_pipex *fds);
void	error_pipe(void);
void	error_unlink(t_pipex *fds);
void	error_err_filename(t_pipex *fds);
void	remove_errorfiles(t_pipex *fds);
void	close_pipes(t_pipex *fds);

#endif