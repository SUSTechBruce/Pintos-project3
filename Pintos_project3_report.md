
# Pintos Project3 Report: File System
:+1: Pintos project_3  - for SUSTech OS  :shipit:

## Qustion and Reflection

## Task 1: Buffer cache
## I.Data structures and functions
### 

### inode.c
```c
#define INODE_MAGIC 0x494e4f44
```
- Identifies an inode.
```c
#define DIRECT_BLOCK_COUNT 123
#define INDIRECT_BLOCK_COUNT 128
```
```c
struct inode_disk
{
	bool is_dir;                        /* Indicator of directory file */
	off_t length;                       /* File size in bytes. */
	unsigned magic;                     /* Magic number. */
  block_sector_t direct_blocks[DIRECT_BLOCK_COUNT];
	block_sector_t indirect_block;
	block_sector_t doubly_indirect_block;
};
```
- This is On-disk inode and it must be exactly BLOCK_SECTOR_SIZE bytes long.
```c
struct inode
{
	struct list_elem elem;              /* Element in inode list. */
	block_sector_t sector;              /* Sector number of disk location. */
	int open_cnt;                       /* Number of openers. */
	struct lock inode_lock;             /* Inode lock. */
	bool removed;                       /* True if deleted, false otherwise. */
	int deny_write_cnt;                 /* 0: writes ok, >0: deny writes. */
};
```
- In-memory inode which is used in task1 mission.
### cache.c
- We creaete the cache.c to implement buffer cache in term of task1's demands
```c
void cache_read (struct block *fs_device, block_sector_t sector_index, 
void *destination,off_t offset, int chunk_size);
```
- Read the chunk_size byte data in the cache from the sector_index in the cache.
```c
void cache_write (struct block *fs_device, block_sector_t 
sector_index, void *source, off_t offset, int chunk_size);
```
- The chunk_size bytes of data are written to the cache, starting with the sector index offset from the source location.
```c
#define CACHE_NUM_ENTRIES 64
#define CACHE_NUM_CHANCES 1
```
- The cache num entries and cache num chances of cache.
```c
struct block_in_cache
  {
    struct lock cache_block_lock;
    block_sector_t disk_sector_index;
    uint8_t data[BLOCK_SECTOR_SIZE];
    bool valid;
    bool dirty;
    size_t chances_remaining;
  };
```
- This is cache block, each block can hold BLOCK_SECTOR_SIZE bytes of data. 
## II. Algorithms and implementations

## Synchronization

## Rationale

## Task2: Extensible files
## Data structure and functions
### syscall.c

### thread.h


## Algorithm and implenmentation

## Synchronization

## Rationale


## Task3: Subdirectories
## Data structure and functions
### syscall.h

### thread.h

### process.h

## Algorithm and implenmentation


## Synchronization

## Rationale

# Reference

