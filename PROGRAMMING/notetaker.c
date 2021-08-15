#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

void usage( char *prog_name, char *filename)
{
	printf("Usage: %s <data to add to %s>\n", prog_name, filename);
	exit(0);
}

void fatal(char *); // A function for fatal errors
void *ec_malloc(unsigned int); // A error checked malloc() wrapper

int main(int argc, char *argv[])
{
	int userid, fd; // File descriptor
	char *buffer, *datafile;

	buffer = (char *) ec_malloc(100);
	datafile = (char *) ec_malloc(20);
	strcpy(datafile, "varnotes");

	if(argc < 2)
		usage(argv[0], datafile);
	strcpy(buffer, argv[1]);

	printf("[DEBUG] buffer @ %p: \'%s\'\n", buffer, buffer);
	printf("[DEBUG] data file @ %p: \'%s\'\n", datafile, datafile);

	strncat(buffer, "\n", 1); // Add a newline to the end

	// Opening file
	fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
	if(fd == -1)
		fatal("in main() while opening file");
	printf("[DEBUG] file descriptor is %d\n", fd);

	userid = getuid(); // Get real user ID

	// Writing data
	if(write(fd, &userid, 4) == -1) // Write userid before note data
		fatal("in main() while writing userid to file");
	write(fd,"\n", 1); // Terminate line

	if(write(fd, buffer, strlen(buffer)) == -1) // Write note
		fatal("in main() while writing buffer to file");
	write(fd,"\n", 1); // Terminate line

	//Closing file
	if(close(fd) == -1)
		fatal("in main() while closing file");

	printf("Note has been saved.\n");
	free(buffer);
	free(datafile);

	return 0;
}