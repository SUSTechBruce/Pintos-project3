#ifndef FILESYS_INODE_H
#define FILESYS_INODE_H

#include <stdbool.h>
#include "filesys/off_t.h"
#include "devices/block.h"
#include "lib/kernel/list.h"
#include "threads/synch.h"

struct bitmap;

/* On-disk inode.
   Must be exactly BLOCK_SECTOR_SIZE bytes long. */
struct inode_disk
  {
    block_sector_t start[100];          /* Direct pointers to sectors. */
    block_sector_t doubly_indirect;     /* Doubly indirect pointer. */
    off_t length;                       /* File size in bytes. */
    bool is_dir;                        /* Inode belongs to directory */
    uint32_t unused[24];                /* Not used. */
    unsigned magic;                     /* Magic number. */
  };

/* In-memory inode. */
struct inode
  {
    struct list_elem elem;              /* Element in inode list. */
    block_sector_t sector;              /* Sector number of disk location. */
    int open_cnt;                       /* Number of openers. */
    bool removed;                       /* True if deleted, false otherwise. */
    int deny_write_cnt;                 /* 0: writes ok, >0: deny writes. */
    struct inode_disk data;             /* Inode content. */
    struct semaphore inode_lock;        /* Lock the inode */
  };


void inode_init (void);
void lock_inode_list (void);
void release_inode_list (void);
bool inode_create (block_sector_t, off_t, bool);
struct inode *inode_open (block_sector_t);
struct inode *inode_reopen (struct inode *);
block_sector_t inode_get_inumber (const struct inode *);
void inode_close (struct inode *);
void inode_remove (struct inode *);
off_t inode_read_at (struct inode *, void *, off_t size, off_t offset);
off_t inode_write_at (struct inode *, const void *, off_t size, off_t offset);
void inode_deny_write (struct inode *);
void inode_allow_write (struct inode *);
off_t inode_length (const struct inode *);
void flush_cache (void);
void clear_cache (void);
void cache_write_block (block_sector_t sector, void *buffer);
void cache_get_block (block_sector_t sector, void *buffer);

#endif /* filesys/inode.h */
