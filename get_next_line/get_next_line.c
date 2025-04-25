/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcauchy <gcauchy@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:07:25 by gcauchy           #+#    #+#             */
/*   Updated: 2025/04/25 14:17:48 by gcauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
//#define BUFFER_SIZE 42

static int	is_backstab(char *stash)
{
	int	i;

	i = 0;
	if (!stash)
		return (0);
	while (stash[i])
	{
		if (stash[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static char	*read_lines(int fd, char *stash)
{
	char	*buffer;
	char	*tmp;
	int		bytes;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes = 1;
	while (!is_backstab(stash) && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes] = '\0';
		tmp = ft_strjoin(stash, buffer);
		free(stash);
		stash = tmp;
	}
	free(buffer);
	return (stash);
}

static char	*get_line(char *stash)
{
	char	*word;
	int		i;
	int		j;

	i = 0;
	while (stash[i] != '\n' && stash[i])
		i++;
	if (stash[i] == '\n')
		i++;
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	j = 0;
	while (j < i)
	{
		word[j] = stash[j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

static char	*new_stash(char *stash)
{
	char	*new;
	char	*temp;

	temp = ft_strchr(stash, '\n');
	new = NULL;
	if (temp && *(temp + 1))
		new = ft_strdup(temp + 1);
	free(stash);
	stash = new;
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	stash = read_lines(fd, stash);
	if (!stash || !stash[0])
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = get_line(stash);
	stash = new_stash(stash);
	return (line);
}

#include <fcntl.h>      // pour open()
#include <stdio.h>      // pour printf()
#include <stdlib.h>     // pour free()

char *get_next_line(int fd); // Ta fonction

int main(void)
{
    int fd = open("test.txt", O_RDONLY);
    char *line;

    if (fd < 0)
    {
        perror("open");
	return 1;
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Ligne lue : %s", line);
		free(line); // Important pour éviter les fuites mémoire
	}
	close(fd);
	return 0;
}