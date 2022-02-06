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

void concatenate_readed_buff(char **new_line, char *buff, int store_buff){
	// static char *static_str;
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
	ft_strtrim();
	free(tmp_line);
	*new_line = concatenated_str;

}

int set_new_line(char *buff, char **new_line, int readed_bytes){
	//static char could be helpful for storing the data
	//that appears after the new_line.
	char *new_line_pointer;

	new_line_pointer = ft_strchr(buff, '\n');
	if (new_line_pointer || readed_bytes <= 0){
		// if a new_line char is founded, set the whole new line,
		// and return 1, indicating that a new line has been set. 
		printf("'iterating ...'\n");
		concatenate_readed_buff(new_line, buff, 1);
		return (1);
	}
	else{
		//join both strings and store it on the new_line double pointer 
		// variable.
		printf("'concatenating ...'\n");
		concatenate_readed_buff(new_line, buff, 0);
		return (0);
	}
}

int get_next_line(const int fd, char **line){
		int		readed_bytes;
		char	buff[BUFF_SIZE + 1];
		// static char remained_buff[128][BUFF_SIZE + 1];

		readed_bytes = read(fd, buff, BUFF_SIZE);
		buff[BUFF_SIZE] = '\0';
		while(readed_bytes >= 0 && !set_new_line(buff, line, readed_bytes)){
			readed_bytes = read(fd, buff, BUFF_SIZE);
			buff[BUFF_SIZE] = '\0';
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
	fd= open(argv[1], O_RDONLY);
	get_next_line(fd,&line);
	printf("main |%s|\n", line);
	// free(line);
	// get_next_line(fd,&line);
	// printf("main |%s|\n", line);
	// free(line);
	return (0);
}