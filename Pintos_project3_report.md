
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
```

- **Step6** Next, implement a function replace cache(), the purpose is to replace the cache entry at index to contain data from sector_index. The core algorithm is Read in and write from device at `disk_sector_index` to data. We need to call 
```c
block_read (fs_device, sector_index, cache[index ].data);
```
- **Step7:** Next we need to implement another important function, the purpose of this function is to write the block at index to disk, the core algorithm step is very simple, that is, write from data to device at disk sector index. So we need to call
```c
block_write (fs_device, cache[index].disk_sector_index, cache[index].data);
```
## Synchronization
- In order to achieve the synchronization of the algorithm, we call the lock acquire() function and the lock release() function on important parameters when implementing the cache read, cache write, cache replace, and search index cache functions. For example, in the search index cache, each time the check to make sure sector_index is not already in the cache step. Call 
```c
lock_acquire (&cache[i].cache_block_lock)
```
and use it in the Perform clock algorithm to find slot to evict step, call
```c
lock_acquire (&cache[i].cache_block_lock);
```
After exiting the loop, you need to call lock_release () to release the lock of the parameter.
## Rationale
- The basic principle for implementing these steps is to implement `read` and `write`, and the `search_index_cache()` needs to ensure synchronization, so you need to use lock. In addition, we need to pay special attention to the search index cache function, the main purpose of the function is to find a cache entry to evict and return its index. We need to ensure that `num_sector` is not already in the cache. More importantly, execute the clock algorithm to find slot to evict. In these steps we have to synchronize the variables to ensure that no errors are encountered when accessing different threads.
## Task2: Extensible files
## Data structure and functions
### filesys.c
```c
bool filesys_create (const char *name, off_t initial_size, bool is_dir);
```
- Creates a file named NAME with the given INITIAL_SIZE.Returns true if successful, false otherwise.Fails if a file named NAME already exists, or if internal memory allocation fails.
### inode.c
```c
#define DIRECT_BLOCK_COUNT 123
#define INDIRECT_BLOCK_COUNT 128
```
- Block Sector Counts.
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
- inode of in memory
```c
struct inode_disk
  {
    block_sector_t direct_blocks[DIRECT_BLOCK_COUNT];
    block_sector_t indirect_block;
    block_sector_t doubly_indirect_block;

    bool is_dir;                        /* Indicator of directory file */
    off_t length;                       /* File size in bytes. */
    unsigned magic;                     /* Magic number. */
  };
```
- On-disk inode
```c
struct indirect_block_sector
  {
    block_sector_t block[INDIRECT_BLOCK_COUNT];
  };
```
- Indirect-block structure.
```c
bool allocate_inode (struct inode_disk *disk_inode, off_t length);
```
-  Attempts allocating sectors in the order of direct->indirect->d.indirect.
```c
bool allocate_indirect_inode (block_sector_t *sector_num, size_t cnt);
```
- Allocate indirect block sector.
```c
void deallocate_inode (struct inode *inode);
```
- Deallocate direct block.
```c
void deallocate_indirect_inode(block_sector_t sector_num, size_t cnt);
```
- Deallocate indirect block
## Algorithm and implenmentation
### Main Algorithm
- In this Extensible files, we focus on the use of indexed inode structure with direct, indirect, and doubly-indirect pointers, like the Unix file system does. Also need to implement this `inumber(int fd)`, which returns The unique inode number of file associated with a particular file descriptor.
### Specific Algorithm

- **Step1:** First of all, we need to implement the function of allocate direct blocks in the main function of allocate inode. First, we need to get number of sector needed and call `bytes_to_sectors (length)`. Second, the core algorithm is 
```c
for (i = 0; i < min (num_sectors, DIRECT_BLOCK_COUNT); i++)
    if (!inode_allocate_sector (&disk_inode->direct_blocks[i]))
      return false;
  sectors_num  = sectors_num - j;
if (num_sectors == 0) {
    return true;
}
```
- **Step2** Second, we need to implement the allocate indirect inode, the specific steps are: First, determine Allocate indirect block sector if it hasn't been, meet the condition, return false. Second, read in the indirect block from cache, call 
```c 
cache_read (fs_device, *sector_num, &indirect_block, 0, BLOCK_SECTOR_SIZE);
```
third, allocate number of sectors needed. Specific steps:
```c
for (i = 0; i < cnt; i++)
    if (!inode_allocate_sector (&indirect_block.block[i]))
      return false;
```
Fourth, write it to the disk, call 
```c
cache_write (fs_device, *sector_num, &indirect_block, 0, BLOCK_SECTOR_SIZE);
```
- **Step3:** Next, we need to implement the allocate Doubly-Indirect Block function. The specific steps are as follows: First, we need to determine the allocate doubly-indirect block sector if it hasn't been , if it meets the condition, it returns false. Second, read the indirect block in the cache, call 
```c
cache_read (fs_device, *sector_num, &indirect_block, 0, BLOCK_SECTOR_SIZE);
```
third, allocate number of indirect blocks needed.
```c
ize_t sector_num, i, j = DIV_ROUND_UP (cnt, INDIRECT_BLOCK_COUNT);
  for (i = 0; i < j; i++)
    {
      sector_num = min (cnt, INDIRECT_BLOCK_COUNT);
      if (!inode_allocate_indirect (&indirect_block.block[i], num_sectors))
        return false;
      cnt -= sector_num;
    }
```
fourth, write it to disk, call 
```c
cache_write (fs_device, *sector_num, &indirect_block, 0, BLOCK_SECTOR_SIZE);
```
- **Step4:** Next, we need to implement allocate inode (), first, we need to call `bytes_to_sectors (inode->data.length)` to calculate number of block sectors occupied, second, In the order of direct, indirect, doubly-indirect, call `Free_map_release ()` until all the block sectors used are freed.
- **Step5:** To implement `syscall_inumber (uint32_t *args UNUSED, uint32_t *eax UNUSED)`, the first step is to call `et_file (thread_current (), fd)` to get the file of the current thread, and determine the file. If the condition is true, call `file_get_inode()` and `inode_get_inumber()` to returns INODE's inode number. The implementation process is as follows
```c
struct file *file = get_file (thread_current (), fd);
  if (file)
    {
      struct inode *inode = file_get_inode (file);
      *eax = (int) inode_get_inumber (inode);
    }
  else
    *eax = -1;
}
```
## Synchronization
In Task 2, we will use the lock acquire and lock release to implement Acquire the lock of INODE, as follows:
```c
void
inode_acquire_lock (struct inode *inode)
{
  lock_acquire (&(inode->inode_lock));
}
```
- Acquire the lock of INODE.
```c
void
inode_release_lock (struct inode *inode)
{
  lock_release (&(inode->inode_lock));
}
```
- Release the lock of INODE.
At the same time, in order to ensure the synchronization of the program, we also use free_map_allocate (), free_map_release (),We created a lock for free_map that will get/release the lock when free_map_allocate() and free_map_release() are called. Since free_map is accessed as a static variable rather than as a file, it does not pass the cache, so it must have its own synchronization through the lock.
## Rationale
In order to ensure the correctness and rationality of the program, we implemented inode_allocate to replace free_map_allocate() according to the direct, indirect, and doule-link allocate methods. According to these implementations, the previous pintos version can only be used to complete each file. As a single contiguous set of blocks, can not extend the size of files.

## Task3: Subdirectories
## Data structure and functions

### thread.h
```c
struct fd
  {
    int fd_num;
    struct file *file;
    struct dir *dir;
  };
```
- struct fd contains struct dir.
```c
struct thread {
  struct dir *directory_working;
};
```
- new add struct to record woring diretory.
### directory.c
```c
struct dir
  {
    struct inode *inode;                /* Backing store. */
    off_t pos;                          /* Current position. */
    struct lock dir_lock;
  };
```
- A directory
```c
struct dir_entry
  {
    block_sector_t inode_sector;        /* Sector number of header. */
    char name[NAME_MAX + 1];            /* Null terminated file name. */
    bool in_use;                        /* In use or free? */
  };
```
- A single directory entry.
### inode.c
```c
struct inode_disk {
  bool isdir;
};
```
- Add boolean DIR to identify a directory inode.
## Algorithm and implenmentation
### Main Algorithm
In this task task, we mainly need to implement functions such as `chdir`, `mkdir`, `readdir`, and `isdir`, and complete user program to manipulate directories. Make first user process should have the root directory as its current working directory.
- **Step1：** Implementing `syscall_chir()` First we need to call dir_open_directory () to get the current dir, if dir is not null, call `dir_close (thread_current ()->working_dir)`, and make the thread's `working_dir` set to the current `dir`, the specific implementation is as follows:
```c
struct dir *direcory = dir_open_directory (name);
  if (direcory != NULL)
    {
      dir_close (thread_current ()->directory_working);
      thread_current ()->working_dir = direcory;
      *eax = true;
    }
  else
    *eax = false;
}
```
- **Step2:**
### Specific Algorithm

## Synchronization

## Rationale

# Reference

