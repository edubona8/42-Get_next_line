/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonamic <ebonamic@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 21:22:14 by ebonamic          #+#    #+#             */
/*   Updated: 2021/10/14 22:45:04 by ebonamic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*make_backup(int fd, char *backup, char *buffer);
static char	*make_line(char *backup);
static char	*get_final(char *backup);

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup[FD_MAX];

	line = NULL;
	if (fd < 0 || fd > FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buffer == NULL)
	{
		free(buffer);
		return (NULL);
	}
	backup[fd] = make_backup(fd, backup[fd], buffer);
	if (backup[fd] == NULL)
		return (NULL);
	line = make_line(backup[fd]);
	backup[fd] = get_final(backup[fd]);
	return (line);
}

static char	*make_backup(int fd, char *backup, char *buffer)
{
	int		read_bytes;
	int		control;
	char	*temp;

	read_bytes = 1;
	control = 0;
	while (control == 0 && read_bytes != 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[read_bytes] = '\0';
		if (backup == NULL)
			backup = ft_strdup("");
		temp = backup;
		backup = ft_strjoin(temp, buffer);
		free(temp);
		if (ft_strchr(backup, '\n') != NULL)
			control = 1;
	}
	free(buffer);
	return (backup);
}

static char	*make_line(char *backup)
{
	int		i;
	char	*line;

	i = 0;
	while (backup[i] != '\0' && backup[i] != '\n')
		i++;
	line = (char *) malloc(sizeof(char) * (i + 2));
	if (line == NULL)
		return (NULL);
	ft_strlcpy(line, backup, i + 2);
	if (line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*get_final(char *backup)
{
	int		i;
	char	*newbackup;

	i = 0;
	while (backup[i] != '\0' && backup[i] != '\n')
		i++;
	if (backup[i] == '\0')
	{
		free(backup);
		return (NULL);
	}
	newbackup = (char *)malloc(sizeof(char) * (ft_strlen(backup) - i + 1));
	if (newbackup == NULL)
		return (NULL);
	ft_strlcpy(newbackup, backup + i + 1, ft_strlen(backup) - i + 1);
	free(backup);
	return (newbackup);
}
