/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_filename.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 22:17:23 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/24 22:17:41 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*pointer_to_hex(t_pipex *fds)
{
	uintptr_t	fds_addr;
	char		*hex_digits;
	char		*buffer;
	int			i;

	i = 16;
	fds_addr = (uintptr_t)fds;
	hex_digits = "0123456789abcdef";
	buffer = (char *)malloc(18);
	if (buffer == NULL)
		return (NULL);
	buffer[0] = '0';
	buffer[1] = 'x';
	while (i >= 2)
	{
		buffer[i--] = hex_digits[fds_addr % 16];
		fds_addr /= 16;
	}
	buffer[17] = '\0';
	return (buffer);
}

static char	*generate_filename(char *base, char *index_str,
		char *address_str, t_pipex *fds)
{
	size_t	total_len;
	char	*filename;

	total_len = ft_strlen(base) + ft_strlen(index_str)
		+ ft_strlen(address_str) + 1;
	filename = (char *)malloc(total_len);
	if (!filename)
	{
		free(index_str);
		free(address_str);
		error_err_filename(fds);
	}
	ft_strlcpy(filename, base, total_len);
	ft_strlcat(filename, index_str, total_len);
	ft_strlcat(filename, address_str, total_len);
	return (filename);
}

void	create_error_filename(char **filename, int index, t_pipex *fds)
{
	char		*base;
	char		*index_str;
	char		*address_str;

	base = "/tmp/error";
	index_str = ft_itoa(index);
	if (index_str == NULL)
		error_err_filename(fds);
	address_str = pointer_to_hex(fds);
	if (address_str == NULL)
	{
		free(index_str);
		error_err_filename(fds);
	}
	*filename = generate_filename(base, index_str, address_str, fds);
	free(index_str);
	free(address_str);
}
