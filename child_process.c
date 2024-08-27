/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:59:12 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/27 21:51:23 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	fd_out_init(int *pipex, char *av[])
{
	int	temp_fd;

	temp_fd = open(av[3], O_DIRECTORY);
	if (temp_fd >= 0)
	{
		close(temp_fd);
		close(pipex[0]);
		error_permission(av[3], 126);
	}
	if (access(av[3], F_OK) == 0 && access(av[3], X_OK) == -1)
		error_permission(av[3], 126);
	if (access(av[4], F_OK) == 0 && access(av[4], W_OK) == -1)
	{
		close(pipex[0]);
		error_permission(av[4], 1);
	}
	else
	{
		pipex[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex[1] < 0)
		{
			close(pipex[0]);
			error_nofile(av[4], 1);
		}
	}
}

int	is_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (1);
		i++;
	}
	return (0);
}

static void	execute_command(char *cmd, char **envp)
{
	char **argv;
	
	if (!cmd || cmd[0] == '\0')
		error_permission(cmd, 126);
	while (*cmd == ' ')
		cmd++;
	if (cmd[0] == '\0')
		error_command(cmd);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		argv = ft_split(cmd, ' '); // free this
		if (!argv)
			perror("ft_split failed");
		execve(argv[0], argv, envp);
		error_nofile(cmd, 127);
	}
	if (!is_env(envp))
		error_command(cmd);
	if(is_dollar(cmd))
		cmd = add_backslash(cmd); // free this
	char	*zsh_argv[] = {"zsh", "-c", cmd, NULL};
	execve("/bin/zsh", zsh_argv, envp);
	perror("execve failed");
	exit(1);
}

void	first_child(char *av[], int *pipex, int *fd, char **envp)
{
	int	temp_fd;

	if (access(av[2], F_OK) == 0 && access(av[2], X_OK) == -1)
		error_permission(av[2], 126);
	temp_fd = open(av[2], O_DIRECTORY);
	if (temp_fd >= 0)
	{
		close(temp_fd);
		error_permission(av[2], 126);
	}
	if (access(av[1], F_OK) == 0 && access(av[1], R_OK) == -1)
		error_permission(av[1], 126);
	else
	{
		pipex[0] = open(av[1], O_RDONLY);
		if (pipex[0] < 0)
			error_nofile(av[1], 127);
	}
/* 	if (access(av[2], F_OK) == 0 && access(av[2], X_OK) == -1)
	{
		error_permission(av[2], 126);
		close(pipex[0]);
	} */
/* 	char	**path = path_init(envp);
	if (!is_cmd_valid(path, av[2]))
	{
		close(pipex[0]);
		free_set(path);
		error_command(av[2]);
	} */
	dup2(pipex[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(pipex[0]);
	execute_command(av[2], envp);
}

void	second_child(char *av[], int *pipex, int *fd, char **envp)
{
	fd_out_init(pipex, av);
/* 	char	**path2 = path_init(envp);
	if (!is_cmd_valid(path2, av[3]))
	{
		close(pipex[0]);
		free_set(path2);
		error_command(av[3]);
	} */
	dup2(fd[0], STDIN_FILENO);
	dup2(pipex[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(pipex[1]);
	execute_command(av[3], envp);
}
