/*
 Accepts the following arguments: the first argument is a full path to a file (including filename) on the filesystem, referred to below as writefile; the second argument is a text string which will be written within this file, referred to below as writestr
 Exits with value 1 error and print statements if any of the arguments above were not specified
 Creates a new file with name and path writefile with content writestr, overwriting any existing file and creating the path if it doesn’t exist. Exits with value 1 and error print statement if the file could not be created.
*/

#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  // Setup syslog
  openlog(NULL, 0, LOG_USER);

  // Verify the number of argc
  if (argc != 3) {
    syslog(LOG_ERR, "Invalid number of arguments: %d", argc);
    return 1;
  }

  // Assign 2 args.
  char *written_file = argv[1];
  char *written_str = argv[2];
  int fd;
  int written_size;

  printf("Writing %s to the file %s ...\n", written_str, written_file);

  // Open file
  fd = open(written_file, O_WRONLY | O_CREAT);
  if (fd < 0) {
    syslog(LOG_ERR, "Openning %s failed with fd=%d", written_file, fd);
    return 1;
  }

  // Write written_str to written_file
  written_size = write(fd, written_str, strlen(written_str));
  if (written_size != strlen(written_str)) {
    syslog(LOG_ERR, "Write to %s error, with returned written size = %d", written_file, written_size);
    return 1;
  }

  syslog(LOG_DEBUG, "Writing %s to %s", written_str, written_file);

  return 0;
}
