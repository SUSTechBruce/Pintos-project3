/* Tell returns the position of the next byte to be read or written in
open file fd, expressed in bytes from the beginning of the file. We test
tell returns the correct position */

#include <stdio.h>
#include <syscall.h>
#include "tests/main.h"
#include "tests/lib.h"

void
test_main (void)
{
  char *file_name = "test_file";
  int fd;
  CHECK (create (file_name, 1), "create \"%s\"", file_name);
  CHECK ((fd = open (file_name)) > 1, "open \"%s\"", file_name);
  long pos = tell (fd);
  if (pos != 0)
    fail ("file position not set up correctly. should be 0, actually %ld", pos);
  else
  	msg ("file position is correct");
}