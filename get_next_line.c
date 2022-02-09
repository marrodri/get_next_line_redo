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

	while (str[i] == c)
		i++;
	if (!str[i])
		return (0);
	return (1);
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

char *get_remaining_buff(char *buff, char *linebreak_pointer){

	char	*remainder_buff;
	char	*non_linebreak_pointer;
	int		address_diff;
	remainder_buff = NULL;
	
	if (ft_strissame(buff, '\n'))
	{
		return (NULL);
	}

	non_linebreak_pointer = ft_strnotchr(buff, '\n');
	// 'hello\n\n test'
	//        0 123456
	address_diff = (int)(non_linebreak_pointer) - (int)(linebreak_pointer);
	
	return (remainder_buff);
}

int set_new_line(char *buff, char **new_line, int readed_bytes){
	//static char could be helpful for storing the data
	//that appears after the new_line.
	char *new_line_pointer;

	new_line_pointer = ft_strchr(buff, '\n');
	if (new_line_pointer || readed_bytes <= 0){
		// if a new_line char is founded, set the whole new line,
		// and return 1, indicating that a new line has been set. 
		concatenate_readed_buff(new_line, buff);
		return (1);
	}
	else{
		//join both strings and store it on the new_line double pointer 
		// variable.
		concatenate_readed_buff(new_line, buff);
		return (0);
	}
}

int get_next_line(const int fd, char **line)
{
		int			readed_bytes;
		char		buff[BUFF_SIZE + 1];
		static char	*stored_buff[128];
		char		*linebreak_pointer;

		linebreak_pointer = NULL;
		readed_bytes = read(fd, buff, BUFF_SIZE);
		buff[BUFF_SIZE] = '\0';
		while(readed_bytes >= 0){
			readed_bytes = read(fd, buff, BUFF_SIZE);
			buff[BUFF_SIZE] = '\0';
			linebreak_pointer = ft_strchr(buff, '\n');
			if(linebreak_pointer){


				if(stored_buff[fd]){
					free(stored_buff[fd]);
				}
				stored_buff[fd] = get_remaining_buff(buff, linebreak_pointer);
				break ;
			}
			concatenate_readed_buff(line, buff);
		}
		if (readed_bytes == -1){
			return (-1);
		}
		if(!readed_bytes){
			return (0);
		}
		return (1);
}

int main(int argc, char **argv){
	int fd;
	char *line = NULL;

	fd = open(argv[1], O_RDONLY);
	get_next_line(fd,&line);
	printf("main |%s|\n", line);
	// free(line);
	// get_next_line(fd,&line);
	// printf("main |%s|\n", line);
	// free(line);
	return (0);
}