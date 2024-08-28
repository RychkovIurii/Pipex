/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:09:27 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/28 17:35:52 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	new_len(char *str)
{
	int	len;

	len = 0;
	while (*str)
	{
		if (*str == '$')
			len++;
		len++;
		str++;
	}
	return (len);
}

static int	is_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

static char	*add_backslash(char *str, int fd[2], int pipex[2])
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = (char *)malloc(sizeof(char) * (new_len(str) + 1));
	if (!new_str)
		error_malloc(fd, pipex);
	while (str[i])
	{
		if (str[i] == '$')
			new_str[j++] = '\\';
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

static int	is_env(char **envp)
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

void	exec_with_zsh(char *cmd, char **envp, int fd[2], int pipex[2])
{
	char	*zsh_argv[4];

	zsh_argv[0] = "zsh";
	zsh_argv[1] = "-c";
	zsh_argv[2] = NULL;
	zsh_argv[3] = NULL;
	if (!is_env(envp))
		error_command(cmd, fd, pipex);
	if (is_dollar(cmd))
		cmd = add_backslash(cmd, fd, pipex);
	zsh_argv[2] = cmd;
	execve("/bin/zsh", zsh_argv, envp);
	free(cmd);
	perror("execve failed");
	exit(1);
}
