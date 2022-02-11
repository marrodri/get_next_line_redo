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

void concatenate_readed_buff(char **new_line, char *buff){
	char *tmp_line;
	char *concatenated_str;
	
	if(!*new_line){
		*new_line = ft_strdup(buff);
		return ;
	}
	tmp_line = *new_line;
	concatenated_str = ft_strjoin(*new_line, buff);

	//trim all the line breaks founded in this buff.
	//if its null, then store nothing and move on.
	// ft_strtrim();
	free(tmp_line);
	*new_line = concatenated_str;
}

int ft_strissame(char *str, int c){
	int i = 0;

	while (str[i] == c && str[i]){

		i++;
	}
	if (!str[i])
		return (1);
	return (0);
}

char *ft_strnotchr(const char *s, int c){
	int	i;

	i = 0;
	while (s[i] == c && s[i])
	{
		i++;
	}
	if (s[i] != c && s[i])
	{
		return ((char *)(&s[i]));
	}
	return (NULL);
}

char *get_remaining_buff(char *buff,int readed_bytes, char *linebreak_pointer){

	char	*remainder_buff;
	char	*non_linebreak_pointer;
	int		address_diff;
	remainder_buff = NULL;
	
	if (ft_strissame(buff, '\n'))
	{
		ft_bzero(buff, readed_bytes);
		return (NULL);
	}
	
	// 'hello\n\n test'
	//        0 123456
	non_linebreak_pointer = ft_strnotchr(buff, '\n');
	// address_diff = (non_linebreak_pointer) - (linebreak_pointer);
	remainder_buff = ft_strdup(non_linebreak_pointer);
	ft_bzero(buff, readed_bytes);
	printf("address_diff |%d|\n", address_diff);
	return (remainder_buff);
}


int get_next_line(const int fd, char **line)
{
		int			readed_bytes;
		char		buff[BUFF_SIZE + 1];
		static char	*stored_buff[128];
		char		*linebreak_pointer;

		linebreak_pointer = NULL;
		readed_bytes = read(fd, buff, BUFF_SIZE);
		buff[readed_bytes] = '\0';
		if(stored_buff[fd]){
			*line = ft_strdup(stored_buff[fd]);
			free(stored_buff[fd]);
			stored_buff[fd] = NULL;
		}
		while (readed_bytes >= 0 && !linebreak_pointer) {
			linebreak_pointer = ft_strchr(buff, '\n');
			if (linebreak_pointer) {
				//TODO BUG to fix:
				//if linebreak appears at the beggining/middle of the string. Store the remaining buffer after the line break.
				//if the line break shows at the very end, then concatenate and return NULL to the stored buffer.
				stored_buff[fd] = get_remaining_buff(buff, readed_bytes, linebreak_pointer);
				concatenate_readed_buff(line, buff);
				printf("break\n");
				break ;
			}
			concatenate_readed_buff(line, buff);
			readed_bytes = read(fd, buff, BUFF_SIZE);
			buff[readed_bytes] = '\0';
		}
		if (readed_bytes == -1)
			return (-1);
		if(!readed_bytes)
			return (0);
		return (1);
}

int main(int argc, char **argv){
	int fd;
	char *line = NULL;

	fd = open(argv[1], O_RDONLY);
	get_next_line(fd,&line);
	printf("main |%s|\n", line);
	free(line);
	get_next_line(fd,&line);
	printf("main |%s|\n", line);
	// free(line);
	return (0);
}