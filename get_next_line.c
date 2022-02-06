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

int set_new_line(char *buff, char **new_line){
	//static char could be helpful for storing the data
	//that appears after the new_line.
	static char static_buff[BUFF_SIZE + 1];
	char *new_line;
	//if a new_line is set, return 1;

	//otherwise, return 0;
	return (0);
}

int get_next_line(const int fd, char **line){
		int		readed_bytes;
		char	*buff[BUFF_SIZE];

		readed_bytes = read(fd, buff, BUFF_SIZE);
		//iterate the file until a new line is founded or
		// the bytes has finished.
		while(readed_bytes > 0 && !set_new_line(buff, line)){
			readed_bytes = read(fd, buff, BUFF_SIZE);
		}
		// if there
		if (readed_bytes == -1){
			return (-1);
		}
		// printf("get_next |%s|, bytes:|%d|\n", buff, readed_bytes);
		*line = buff;
		if(!readed_bytes){
			return (0);
		}
		return (1);
}

int main(int argc, char **argv){
	int fd;
	char *line;
	fd= open(argv[1], O_RDONLY);
	get_next_line(fd,&line);
	printf("main |%s|\n", line);
	free(line);
	get_next_line(fd,&line);
	printf("main |%s|\n", line);
	free(line);
	return (0);
}