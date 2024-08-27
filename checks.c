/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:09:27 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/27 18:24:30 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* static int	files_check(int *pipex, char *av[])
{
	pipex[0] = open(av[1], O_RDONLY);
	if (pipex[0] == -1 && access(av[1], F_OK))
		error_nofile(av[1]);
	else if (pipex[0] == -1 && !access(av[1], F_OK))
		error_permission(av[1], 126);
	pipex[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex[1] == -1 && access(av[4], F_OK))
		error_nofile(av[4]);
	else if (pipex[1] == -1 && !access(av[4], F_OK))
		error_permission(av[4],126);
	if (pipex[0] == -1 && pipex[1] == -1)
		return (1);
	return (0);
}
 */

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

int	is_cmd_valid(char **path, char *cmd)
{
	char *full_path;
	char *cmd_path;
	char **cmd_main;
	int i = 0;

	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == -1)
	{
		error_permission(cmd, 126);
		free_set(path);
	}
	cmd_main = ft_split(cmd, ' ');
	while (path[i])
	{
		cmd_path = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(cmd_path, cmd_main[0]);
		free(cmd_path);
		if((access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0))
		{
			free_set(cmd_main);
			free_set(path);
			free(full_path);
			return (1);
		}
		free(full_path);
		i++;
	}
	free_set(cmd_main);
	return (0);
	
}

char	**path_init(char **envp)
{
	int	i;
	char	**path;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i] + 5, ':');
			if (!path)
			{
				ft_putstr_fd("ft_split\n", 2);
				exit (1);
			}
			return (path);
		}
		i++;
	}
	return (path);
}

/* irychkov@c1r4p1 ~/Desktop/Pipex_dev
 % ./pipex infile "xxx" "/xxx/xxx" outfile1
zsh:1: no such file or directory: /xxx/xxx
zsh:1: command not found: xxx
irychkov@c1r4p1 ~/Desktop/Pipex_dev
 % < infile xxx | /xxx/xxx > outfile2
zsh: command not found: xxx
zsh: no such file or directory: /xxx/xxx
 */