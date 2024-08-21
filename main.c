/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:16 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/21 16:51:37 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_set(char **set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		free(set[i]);
		i++;
	}
	free(set);
}

/* void print_split(char **split_array)
{
	int i = 0;
	
	while (split_array[i] != NULL)
	{
		ft_printf("split_argv2[%d]: %s\n", i, split_array[i]);
		i++;
	}
} */

void	cmd_init(t_struct *data, char *av[])
{
	data->cmd1 = ft_split(av[2], ' ');
	if (!data->cmd1)
	{
		ft_putstr_fd("ft_split\n", 2);
		exit (1);
	}
	data->cmd2 = ft_split(av[3], ' ');
	if (!data->cmd2)
	{
		ft_putstr_fd("ft_split\n", 2);
		free_set(data->cmd1);
		exit (1);
	}
}

int	is_space(char *str)
{
	while (*str != '\0')
	{
		if (*str == ' ')
		{
			return (1);
		}
		str++;
	}
	return (0);
}

void	fd_init(t_struct *data)
{
	/* if (access(data->file1, R_OK) == -1) // Not sure! I should ask
	{
		perror("Error: file1 cannot be accessed for reading");
		return 1;
	} */
	data->fd_in = open(data->file1, O_RDONLY);
	if (data->fd_in < 0)
	{
		perror("Error opening file1");
		free(data->file1);
		free(data->file2);
		exit (1);
	}
	/* if (access(data->file2, F_OK) == 0 && access(data->file2, W_OK) == -1)
	{ // Not sure! I should ask
		perror("Error: file2 cannot be accessed for writing");
		return 1;
	} */
	data->fd_out = open(data->file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fd_out < 0)
	{
		perror("Error opening file2");
		close(data->fd_in);
		free(data->file1);
		free(data->file2);
		exit (1);
	}
}

void	file_init(t_struct *data, char *av[])
{
	data->file1 = ft_strdup(av[1]);
	if (!data->file1)
	{
		ft_putstr_fd("Error: Memory allocation for argv1 failed\n", 2);
		exit (1);
	}
	data->file2 = ft_strdup(av[4]);
	if (!data->file2)
		{
			free(data->file1);
			ft_putstr_fd("Error: Memory allocation for argv4 failed\n", 2);
			exit (1);
		}
	if (is_space(data->file2) || is_space(data->file1)) // Not sure
	{
		free(data->file1);
		free(data->file2);
		ft_putstr_fd("Error: file contains a space\n", 2);
		exit (1);
	}
/* 	ft_printf("data->file1: %s\n", data->file1);
	ft_printf("data->file2: %s\n", data->file2);
	free(data->file1);
	free(data->file2); */
}

void	execute_command(char **cmd, char **envp, char **path)
{
	char *cmd_path;
	int i = 0;

	if (cmd[0][0] == '/' || cmd[0][0] == '.')
		execve(cmd[0], cmd, envp);
	if (!path)
	{
		ft_putstr_fd("Error: PATH not found\n", 2); //HAVE to check env -i
		exit(1);
	}
	while (path[i])
	{
		cmd_path = ft_strjoin(path[i], "/");
		char *full_path = ft_strjoin(cmd_path, cmd[0]);
		free(cmd_path);
		if (execve(full_path, cmd, envp) != -1)
		{
			free(full_path);
			return;
		}
		free(full_path);
		i++;
	}
	perror("execve failed");
	exit(1);
}

void	path_init(t_struct *data, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			data->path = ft_split(envp[i] + 5, ':');
			if (!data->path)
			{
				ft_putstr_fd("ft_split\n", 2);
				exit (1);
			}
			return ;
		}
		i++;
	}
	data->path = NULL;
}

void	data_init(t_struct *data, char *av[], char **envp)
{
	path_init(data, envp);
	file_init(data, av);
	fd_init(data);
	cmd_init(data, av); // check free file
	//TEST
/* 	execute_command(data->cmd1, envp, data->path);
	free_set(data->path);
	free_set(data->cmd1);
	free_set(data->cmd2);
	free(data->file1);
	free(data->file2); */
}

void	pipex(char *av[], char **envp)
{
	t_struct	data;
	int			pid1;
	int			pid2;

	data_init(&data, av, envp);

	int	fd[2];

	if (pipe(fd) == -1)
		exit (1);
		
	pid1 = fork();
	if (pid1 == -1)
		exit (1);
	
	if (pid1 == 0)
	{
		//Child proccess 1
		if (dup2(data.fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 failed for STDIN");
			exit (1);
		}
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for STDOUT");
			exit (1);
		}
		close(fd[0]);
		close(fd[1]);
		execute_command(data.cmd1, envp, data.path);
	}
	pid2 = fork();
	if (pid2 == -1)
		exit (1);

	if (pid2 == 0)
	{
		//Child proccess 2

		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed for STDIN");
			exit (1);
		}
		if (dup2(data.fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for STDOUT");
			exit (1);
		}
		close(fd[0]);
		close(fd[1]);
		execute_command(data.cmd2, envp, data.path);
	}

	//Parent proccess
	close(fd[0]);
	close(fd[1]);
	close(data.fd_in);
	close(data.fd_out);
	int	waitstatus1;
	int	waitstatus2;

	if (waitpid(pid1, &waitstatus1, 0) == -1)
	{
		perror("waitpid for pid1 failed");
		exit (1);
	}
	if (waitpid(pid2, &waitstatus2, 0) == -1)
	{
		perror("waitpid for pid2 failed");
		exit (1);
	}
	free_set(data.cmd1);
	free_set(data.cmd2);
	free_set(data.path);
}

int	main(int ac, char *av[], char **envp)
{
	if (ac == 5)
	{
		pipex(av, envp);
		return (0);
	}
	else
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (1);
	}
}
