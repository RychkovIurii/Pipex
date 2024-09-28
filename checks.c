/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:09:27 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/28 21:43:27 by irychkov         ###   ########.fr       */
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

static char	*add_backslash(char *str, t_pipex *fds)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = (char *)malloc(sizeof(char) * (new_len(str) + 1));
	if (!new_str)
		error_malloc(fds);
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

void	exec_with_zsh(char *cmd, char **envp, t_pipex *fds)
{
	int		flag;
	char	*argv;
	char	*zsh_argv[4];

	flag = 0;
	argv = NULL;
	zsh_argv[0] = "zsh";
	zsh_argv[1] = "-c";
	zsh_argv[3] = NULL;
	if (is_dollar(cmd))
	{
		cmd = add_backslash(cmd, fds);
		flag = 1;
	}
	zsh_argv[2] = cmd;
	if (!is_env(envp) || (cmd[0] == '/' || cmd[0] == '.'))
	{
		argv = ft_strjoin("unset PATH; ", cmd);
		if (!argv)
			error_strjoin(fds, flag, cmd);
		zsh_argv[2] = argv;
	}
	/* free(fds->error_filename1);
	free(fds->error_filename2); */
	execve("/bin/zsh", zsh_argv, envp);
	error_execve(fds, flag, cmd, argv);
}
