
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
### filesys.c
```c
void filesys_init(bool format);
```
- Initializes the file system module.If FORMAT is true, reformats the file system.
## II. Algorithms and implementations
### Main Algorithm
- The main purpose of this Buffer cache is to design a cache in the filesystem to improve the performance of reading and writing, so the main algorithm is we can respond to reads with cached data and we can coalesce multiple writes into a single disk operation. And We choose a block replacement policy and we design a write-back cache efficiently.
### Specific Algorithms
- **Step1:** First, we need to design a function to initialize the cache called `initialize_cache()`. This function first initializes the lock, the purpose is to lock the `cache_update_lock` variable. Second, for each `block` in the cache, we set its valid bit to false and call `lock_init()` to lock the block variable.
- **Step2:** Second, we need to modify `filess_init()` and add `initialize_cache()` in it in order to initialize cache every time when you want to start filesystem.
- **Step3:** in this step, it is time to implement a function called`search_cache_index()`in order to find a cache entry  to pop and return its index. So we choose clock algorithm to achieve this goal. first, we a pointer called `clock_p`, and then we call `lock_acquire()` to lock this variable `cache_update_lock`. the next step is call lock_acquire to lock blocks in `cache_block` and we need to iterate cache to make sure there is no `num_sector`. Next, if the `num_sector` is in this cache, we release the lock for this `block`. We have reached the key steps of the clock algorithm. First, we check if the valid bit is legal. If it is legal, return the index of the block, then check the remaining chance to see if it is 0. If it is 0, break. Next we should release this lock, as well as the `cache_update_lock`. In the outer loop, if the block is detected as `dirty`, set the valid to false and return the `index`.
- **Step4:** In this step, we need to design a cache write function according to task1 requirements. The purpose of this function is write `chunk_size` bytes of data into cache starting from `numsector` at position offest from source. The core algorithm is to use `cache_get_block_index()` acquires `cache_block_lock` at `index` i. Then set the block's `dirty bit` to true and change the remaining_chance.
- **Step5:**  Read operations and write operations have similarities, the purpose is read chunk_size bytes of data from cache starting from numsector at position offest, into destination. We also need to complete `cache_get_block_index ()` acquires `cache_block_lock` at `index` i. Then call 
```c
memcpy (destination, cache[i ].data + offset, chunk_size);
```.

- **Step6** Next, implement a function replace cache(), the purpose is to replace the cache entry at index to contain data from sector_index. The core algorithm is Read in and write from device at `disk_sector_index` to data. We need to call 
```c
block_read (fs_device, sector_index, cache[index ].data);
```
- **Step6:** Next we need to implement another important function, the purpose of this function is to write the block at index to disk, the core algorithm step is very simple, that is, write from data to device at disk sector index. So we need to call
```c
block_write (fs_device, cache[index].disk_sector_index, cache[index].data);
```
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

