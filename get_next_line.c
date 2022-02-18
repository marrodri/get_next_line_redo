// TODO
/*
Logic:
Read the text file, and get the first line, when the function is called.
When the function get_next_line is being called again, 
get the next new line from the same text file. 

the get_next_line will return 3 values.
--  1: if there are more bytes to read from FD.
-- -1: when there is an error/
--  0: when the get_next_line, has reached 0 bytes to read.
	aka: End Of File (EOF)

//Pseudo code:
first read the text file; when a new line has been founded,
stop and store the whole line with the new line break. If there 
are characters after the new line, store them for the next function
call. Also return 1, for signaling that there is more bytes to read from
the same FD.
*/
#include "get_next_line.h"
#include <stdio.h>


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void concatenate_readed_buff(char **curr_buff, char *new_buff){
	char *concatenated_buff;
	
	if (!*curr_buff)
	{
		*curr_buff = ft_strdup(new_buff);
		return ;
	}
	concatenated_buff = ft_strjoin(*curr_buff, new_buff);
	free(*curr_buff);
	*curr_buff = concatenated_buff;
}

int ft_strissame(char *str, int c){
	int i;

	i = 0;
	while (str[i] == c && str[i])
		i++;
	if (!str[i])
		return (1);
	return (0);
}

char *ft_strnotchr(const char *s, int c){
	int	i;

	i = 0;
	while (s[i] == c && s[i])
		i++;
	if (s[i] != c && s[i])
		return ((char *)(&s[i]));
	return (NULL);
}

	
char *set_newline(char **line, char *buff)
{
	char	*remainder_buff;
	char	*linebreak_pointer;
	int		line_length;
	int		linebreak_diff;

	remainder_buff = NULL;
	linebreak_pointer = NULL;
	//TODO: this function is called first, without initializing the line.

	line_length = ft_strlen(buff);
	linebreak_pointer = ft_strchr(buff, '\n');
	if(!linebreak_pointer){
		*line = ft_strdup(buff);
		return (NULL);
	}
	linebreak_diff = linebreak_pointer - buff;
	printf("linebreak_diff |%d|\n", linebreak_diff);
	if(linebreak_diff == 0)
	{
		/*store the remainder of the linebreak and keep the original line
		as how it is*/
		
		//move the buff pointer to where the first non linebreak character is
		buff = ft_strnotchr(buff, '\n');
		remainder_buff = ft_strdup(buff);

		if (!remainder_buff)
			return (NULL);
		return (remainder_buff);
	}
	if (linebreak_diff == (line_length - 1)){
		/*return null to the remainder buffer and update the line
		without the line breaker.*/

		//this shows that the linebreak is only located at the end of the buff,
		// iterate from the end of it so it can trim
		*line = ft_strtrim(buff, "\n");
		return (NULL);
	}
	/* return the remainder of the buff after the line break
	and update the line.*/
	// free(prev_line);
	*line = buff;
	remainder_buff = ft_strnotchr(linebreak_pointer, '\n');
	remainder_buff = ft_strdup(remainder_buff);
	*linebreak_pointer = '\0';
	return (remainder_buff);
}

int get_next_line(const int fd, char **line)
{
		int			readed_bytes;
		char		buff[BUFF_SIZE + 1];
		static char	*stored_buff[128];
		char		*concatenated_line;
		char		*prev_line;

		if(*line)
			prev_line = ft_strdup(*line);
		readed_bytes = read(fd, buff, BUFF_SIZE);
		buff[readed_bytes] = '\0';
		*line = NULL;
		concatenated_line = NULL;
		//the stored_buff can be moved to the buff variable
		if (stored_buff[fd])
		{
			concatenated_line = ft_strdup(stored_buff[fd]);
			free(stored_buff[fd]);
			stored_buff[fd] = NULL;
		}
		concatenate_readed_buff(&concatenated_line, buff);
		while ((readed_bytes > 0 || concatenated_line) && !*line)
		{
			readed_bytes = read(fd, buff, BUFF_SIZE);
			buff[readed_bytes] = '\0';
			concatenate_readed_buff(&concatenated_line, buff);
			if (ft_strchr(concatenated_line, '\n') || !readed_bytes) {
				stored_buff[fd] = set_newline(line, concatenated_line);
				if(!*line)
					*line = prev_line;
				// free(concatenated_line);
				concatenated_line = NULL;
				// break ;
			}
			//concatenate_line can be called once at the beggining of each
			//iteration.
		}
		if (readed_bytes == -1)
			return (-1);
		if (!readed_bytes && !stored_buff[fd])
			return (0);
		return (1);
}

int main(int argc, char **argv)
{
	int fd;
	char *line = NULL;

	fd = open(argv[1], O_RDONLY);
	get_next_line(fd,&line);
	printf("1 main |%s|\n", line);
	free(line);
	get_next_line(fd,&line);
	printf("2 main |%s|\n", line);
	free(line);
	return (0);
}
