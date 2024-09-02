/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:59 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/02 20:37:44 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"

void	first_child(char *av[], int *pipex, int *fd, char **envp, int error_fd);
void	second_child(char *av[], int *pipex, int *fd, char **envp, int error_fd);
void	exec_with_zsh(char *cmd, char **envp, int fd[2], int pipex[2]);
void	error_permission(char *name, int code, int fd[2], int pipex[2]);
void	error_command(char *name, int fd[2], int pipex[2], int flag);
void	error_nofile(char *name, int code, int fd[2], int pipex[2]);
void	error_directory(char *name, int code, int fd[2], int pipex[2]);
void	error_fork(int fd[2], int pipex[2]);
void	error_malloc(int fd[2], int pipex[2]);
void	error_dup(int fd[2], int pipex[2]);
void	error_execve(int flag, char *cmd);
void	error_waitpid(void);
void	close_pipes(int fd[2], int pipex[2]);
void	free_set(char **set);

#endif