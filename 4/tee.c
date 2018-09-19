#include<sys/stat.h>
#include<fcntl.h>
#include"tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#define MAXOUTPUT 64
#endif


int
main(int argc, char *argv[])
{
	int inputFd = 0, outputFd[MAXOUTPUT], openFlags, numOutputfile = argc, isappend = 0;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if (strcmp(argv[1], "--help") == 0)
		usageErr("%s 0 or multi file", argv[0]);

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	if (strcmp(argv[1], "-a") == 0)
	{
		openFlags |= O_APPEND;
		numOutputfile --;
		isappend = 1;
	}
	
	outputFd[0] = 1;
	int i;
	for(i = 1;i < numOutputfile; i++)
	{
		outputFd[i] = open(argv[i+isappend], openFlags, filePerms);
		if( outputFd[i]== -1)
			errExit("opening file %s",argv[i+isappend]);
	}

	while((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
	{
		for (i = 0;i < numOutputfile; i++)
		{
			if(write(outputFd[i], buf, numRead) != numRead)
				fatal("couldn't write whole buffer");
		}
	}
	
	if(numRead == -1)
		errExit("read");

	for(i = 1; i < numOutputfile; i++)
		if(close(outputFd[i]) == -1)
			errExit("close input %d",outputFd[i]);
	
	exit(EXIT_SUCCESS);
}
