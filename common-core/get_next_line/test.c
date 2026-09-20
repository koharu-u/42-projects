/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 20:32:56 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/20 21:19:15 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*buffer;
	int		del;

	buffer = (char *)calloc(72, sizeof(char));
	fd = open("touhou.txt", O_RDONLY);
	del = read(fd, buffer, 20);
	printf("%s", buffer);
	// del = read(fd, buffer, 20);
	// printf("%s", buffer);
	(void)del;
	return (0);
}
