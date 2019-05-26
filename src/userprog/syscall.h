#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "filesys/directory.h"

struct dir;

void syscall_init (void);
void syscall_exit (void);
struct dir * get_last_directory (const char *);
struct dir * get_last_directory_absolute (const char *);
struct dir * get_last_directory_relative (const char *);
int get_next_part (char part[NAME_MAX + 1], const char **);

#endif /* userprog/syscall.h */
