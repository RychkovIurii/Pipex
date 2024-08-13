/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:16 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/13 16:16:07 by irychkov         ###   ########.fr       */
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

void print_split(char **split_array)
{
	int i = 0;
	
	while (split_array[i] != NULL)
	{
		ft_printf("split_argv2[%d]: %s\n", i, split_array[i]);
		i++;
	}
}

int	cmd_init(t_struct *data, char *av[])
{
	char		**split_argv2;
	char		**split_argv3;

	split_argv2 = ft_split(av[2], ' ');
	if (!split_argv2)
		{
			ft_putstr_fd("Error\n", 2);
			return (1);
		}
	split_argv3 = ft_split(av[3], ' ');
	if (!split_argv3)
		{
			ft_putstr_fd("Error\n", 2);
			free_set(split_argv2);
			return (1);
		}
	data->cmd1 = split_argv2;
	data->cmd2 = split_argv3;
	print_split(data->cmd1);
	print_split(data->cmd2);
	free_set(data->cmd1);
	free_set(data->cmd2);
	return (0);
}

int	file_init(t_struct *data, char *av[])
{
	char		*argv1;
	char		*argv4;

	argv1 = ft_strdup(av[1]);
	if (!argv1)
		{
			ft_putstr_fd("Error\n", 2);
			return (1);
		}
	argv4 = ft_strdup(av[4]);
	if (!argv4)
		{
			ft_putstr_fd("Error\n", 2);
			free(argv1);
			return (1);
		}
	data->file1 = argv1;
	data->file2 = argv4;
	ft_printf("data->file1: %s\n", data->file1);
	ft_printf("data->file2: %s\n", data->file2);
	free(data->file1);
	free(data->file2);
	return (0);
}

int	init_data(char *av[])
{
	t_struct	data;

	cmd_init(&data, av);
	file_init(&data, av);
	return (0);
}

int	main(int ac, char *av[])
{
	if (ac == 5)
	{
		init_data(av);
		ft_printf("check %s", av[0]);
		return (0);
	}
	else
	{
		ft_printf("Pass 4 arguments!\n");
		return (1);
	}
}
