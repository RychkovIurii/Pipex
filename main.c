/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 13:53:16 by irychkov          #+#    #+#             */
/*   Updated: 2024/08/13 15:29:39 by irychkov         ###   ########.fr       */
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

int	argv_handler(char *av[])
{
	t_struct	data;
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
	data.cmd1 = split_argv2;
	data.cmd2 = split_argv3;
	print_split(data.cmd1);
	print_split(data.cmd2);
	free_set(data.cmd1);
	free_set(data.cmd2);
	return (0);
}

int	main(int ac, char *av[])
{
	if (ac == 5)
	{
		argv_handler(av);
		ft_printf("check %s", av[0]);
		return (0);
	}
	else
	{
		ft_printf("Pass 4 arguments!\n");
		return (1);
	}
}
