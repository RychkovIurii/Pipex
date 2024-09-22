/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:41:12 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/22 16:38:48 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_pipex
{
	int		num_cmds;
	int		here_doc;
	char	*delimiter;
	int		**pipes;
	int		pipex[2];
	int		*error_fds;
	pid_t	*pids;
}	t_pipex;

void	fd_in_init(t_pipex *fds, char *av[]);
void	initialize_pipex(t_pipex *fds, char *av[], int ac);
void	create_error_filename(char **filename, int index, t_pipex *fds);
void	execute_command(char *cmd, char **envp, t_pipex *fds);
void	exec_child(int cmd_pos, t_pipex *fds, char *av[], char **envp);
void	exec_with_zsh(char *cmd, char **envp, t_pipex *fds);
int		wait_for_children(pid_t *pids, t_pipex *fds);
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
void	error_pipe(t_pipex *fds);
void	error_unlink(t_pipex *fds);
void	error_err_filename(t_pipex *fds);
void	close_pipes(t_pipex *fds);
void	free_pipex(t_pipex *fds);
void	handle_here_doc(t_pipex *fds, char *av[]);

#endif