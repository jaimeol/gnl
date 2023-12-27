/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mian.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jolivare <jolivare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:00:02 by jolivare          #+#    #+#             */
/*   Updated: 2023/12/18 16:23:02 by jolivare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int	fd = open("pruebas.txt", O_RDONLY);
	int	fd2 = open("p2.txt", O_RDONLY);
    if (fd == -1)
	{
        perror("Error al abrir el archivo");
		return 1;
    }

    char *line;
	char *line2;
    while ((line = get_next_line(fd)) != NULL && (line2 = get_next_line(fd2)) != NULL) {
        // Procesa la línea como desees, por ejemplo, imprímela
		printf("%s\n %s\n", line, line2);
		free(line2);
        // Libera la memoria asignada a la línea
        free(line);
    }
    close(fd);
	close(fd2);
    return 0;
}