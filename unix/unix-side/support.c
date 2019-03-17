#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "demand.h"
#include "../shared-code/simple-boot.h"
#include "support.h"

#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

// read entire file into buffer.  return it, write totat bytes to <size>
unsigned char *read_file(int *size, const char *name) {
	// unimplemented();
	int fd = open(name, O_RDONLY);
	if(fd == -1) panic("Invalid filename");
	struct stat st;
	if(stat(name, &st) != 0) panic("Bad Stat");
	size_t fileSz_unrounded = st.st_size;

	size_t fileSz = roundup(fileSz_unrounded, 4);

	unsigned char *buffer = calloc(fileSz, 1);
	if(buffer == NULL) panic("Bad Calloc");
	size_t readSz = read(fd, buffer, fileSz_unrounded);
	if(readSz != fileSz_unrounded) panic("Read wrong num bytes");

	*size = fileSz;

	close(fd);
	return buffer;
}

#define _SVID_SOURCE
#include <dirent.h>
const char *ttyusb_prefixes[] = {
	"ttyUSB",	// linux
	// "tty.SLAB_USB", // mac os
	"cu.SLAB_USB", // mac os

	0
};


int file_select(const struct dirent *entry) {
	unsigned int i = 0;
	while(ttyusb_prefixes[i] != NULL) {
		if(strncmp(ttyusb_prefixes[i], entry->d_name, 
			strlen(ttyusb_prefixes[i])) == 0) return 1;
		i++;
	}
	return 0;
}

// open the TTY-usb device:
//	- use <scandir> to find a device with a prefix given by ttyusb_prefixes
//	- returns an open fd to it
// 	- write the absolute path into <pathname> if it wasn't already
//	  given.
int open_tty(const char **portname) {
	// unimplemented();
	struct dirent **namelist;
	if (scandir("/dev", &namelist, file_select, alphasort)!=1) panic("Wrong num files");
	// printf("%s\n", namelist[0]->d_name);
	fflush(stdout);
	char* buffer = calloc(6 + strlen(namelist[0]->d_name), 1);
	sprintf(buffer, "/dev/%s", namelist[0]->d_name);
	*portname = buffer;
	printf("%s\n", buffer);
	int fd = open(*portname, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd == -1) panic("Invalid filename");
	// printf("Opened File!");
	fflush(stdout);
	return fd;
}
