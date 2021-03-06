#include<sys/stat.h>
#include<fcntl.h>
#include<ctype.h>
#include"tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int
main(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if(argc != 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s source-file dest-file\n", argv[0]);
	
	inputFd = open(argv[1], O_RDONLY);
	if(inputFd == -1)
		errExit("opening file %s", argv[1]);

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
			S_IROTH | S_IWOTH;
	
	outputFd = open(argv[2], openFlags, filePerms);
	if(outputFd == -1)
		errExit("opening file %s", argv[2]);
	
	int i, j;
	while((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
	{
		for(i = 0; i < numRead; )
		{
			j = 0;
			while(i < numRead && buf[i] != '\0')
			{
				j++;
				i++;
			}
			if(j > 0)
			{		
				if(write(outputFd, buf+i-j, j) != j)
					fatal("couldn't write whole buffer");
			}
			
			while(i < numRead && buf[i] == '\0')
			{
				i++;
			}
			/*if(j > 0)
				if(lseek(outputFd, j, SEEK_CUR) == -1)
					errExit("lseek");*/
		}
	}	
	
	if(numRead == -1)
		errExit("read");

	if(close(inputFd) == -1)
		errExit("close input");
	if(close(outputFd) == -1)
		errExit("close output");

	exit(EXIT_SUCCESS);
}
