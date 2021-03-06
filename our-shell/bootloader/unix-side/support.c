#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "demand.h"
#include "../shared-code/simple-boot.h"
#include "support.h"

#define PAD_MULTIPLE 4
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

// read entire file into buffer.  return it, write totat bytes to <size>
unsigned char *read_file(int *size, const char *name) {
	int desc = open(name, O_RDONLY);

	if(desc == -1){
		panic("Couldn\'t read file\n");
	}

	struct stat s;
	if(fstat(desc, &s) == -1){
		panic("Couldn\'t get file info\n");
	}

	int sizeUnrounded = s.st_size;
	*size = roundup(sizeUnrounded, PAD_MULTIPLE);

	unsigned char* buf = calloc(*size, sizeof(char));
	if(buf == NULL){
		panic("calloc failed\n");
	}

	int readBytes = read(desc, buf, sizeUnrounded);
	if(readBytes != sizeUnrounded){
		panic("read failed\n");
	}

	close(desc);

	return buf;
}

#define _SVID_SOURCE
#include <dirent.h>
const char *ttyusb_prefixes[] = {
	"ttyUSB",	// linux
	"cu.SLAB_USB", // mac os
	0
};

int filter(const struct dirent * ent){
	const char **prefix = ttyusb_prefixes;
	while(*prefix != 0){
		if(strncmp(*prefix, ent->d_name, strlen(*prefix)) == 0){
			return 1;
		}
		prefix++;
	}
	return 0;
}

// open the TTY-usb device:
//	- use <scandir> to find a device with a prefix given by ttyusb_prefixes
//	- returns an open fd to it
// 	- write the absolute path into <pathname> if it wasn't already
//	  given.
int open_tty(const char **portname) {
	struct dirent** nameList;
	int numFound = scandir("/dev", &nameList, filter, alphasort);

	if(numFound != 1){
		panic("Number of tty-usb found not equal to 1\n");
	}

	char *buff = malloc(6 + strlen(nameList[0]->d_name)); // +6 because /dev/ 
	sprintf(buff, "/dev/%s", nameList[0]->d_name);
	printf("Found tty <%s>\n", buff);
	*portname = buff;

	int desc = open(buff, O_RDWR | O_NOCTTY | O_SYNC);
	if(desc == -1){
		panic("Couldn't open found tty-usb file\n");
	}

	return desc;
}
