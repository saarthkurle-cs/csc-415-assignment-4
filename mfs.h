/**************************************************************
* Class::  CSC-415-0# Spring 2024
* Name:: Jessica, Kalise, Kayla, and Saarth 
* Student IDs:: 921113528, 922199158, 921913613, 922266706
* GitHub-Name:: icebearnose (main git-hub)
* Group-Name:: The Task Masters 
* Project:: Basic File System
*
* File:: mfs.h
*
* Description:: 
*	This is the file system interface.
*	This is the interface needed by the driver to interact with
*	your filesystem.
*
**************************************************************/


#ifndef _MFS_H
#define _MFS_H
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "b_io.h"

#include <dirent.h>
#define FT_REGFILE	DT_REG
#define FT_DIRECTORY DT_DIR
#define FT_LINK	DT_LNK
#define DIR_ENTRY_SIZE 60


#ifndef uint64_t
typedef u_int64_t uint64_t;
#endif
#ifndef uint32_t
typedef u_int32_t uint32_t;
#endif


enum FileType { DIRECTORY_TYPE, FILE_TYPE }; //Values for entry -> enum fileType

struct VCB{
   char name[81]; 	//Name for our Volume Control Block
                  	// Can store up to 80 characters including a null terminator
   int total; 	  	//Total amount of blocks
   int free; 		//Amount of blocks that are free
   int blockSize; 	//Size of each block
   int signature;
   int location; 	//Location of where the VCB will be mounted  
};

// This structure is returned by fs_readdir to provide the caller with information
// about each file as it iterates through a directory
struct fs_diriteminfo
	{
    unsigned short d_reclen;    /* length of this record */
    unsigned char fileType;    
    char d_name[256]; 			/* filename max filename is 255 characters */
	};

// This is a private structure used only by fs_opendir, fs_readdir, and fs_closedir
// Think of this like a file descriptor but for a directory - one can only read
// from a directory.  This structure helps you (the file system) keep track of
// which directory entry you are currently processing so that everytime the caller
// calls the function readdir, you give the next entry in the directory
typedef struct
	{
	/*****TO DO:  Fill in this structure with what your open/read directory needs  *****/
	unsigned short  d_reclen;		/* length of this record */
	unsigned short	dirEntryPosition;	/* which directory entry position, like file pos */
	//DE *	directory;			/* Pointer to the loaded directory you want to iterate */
	struct fs_diriteminfo * di;		/* Pointer to the structure you return from read */
	} fdDir;

//directory entry structure
typedef struct DE {
   int id; 					//id for the file
   char name[256]; 			//path name can be 256 characters or less
   int location; 			// block num
   time_t modified; 		// timestamp date last modified
   time_t accessed; 		// timestamp date last accessed
   time_t created; 			// timestamp date created
   char owner[256]; 		// owner of file can be up to 256 characters
   enum FileType fileType; 	// indicates file type being used
} DE;

//struct for parsePath information
struct ppReturn {
	char* lastElementName;
	int lastElementIndex;
	DE * parent;
};

//global directory variables
extern DE* loadedCWD;
extern DE* root;
extern char* loadedCWDString;

// Key directory functions
int fs_mkdir(const char *pathname, mode_t mode);
int fs_rmdir(const char *pathname);

// Directory iteration functions
fdDir * fs_opendir(const char *pathname);
struct fs_diriteminfo *fs_readdir(fdDir *dirp);
int fs_closedir(fdDir *dirp);

// Misc directory functions
char * fs_getcwd(char *pathname, size_t size);
int fs_setcwd(char *pathname);   			//linux chdir
int fs_isFile(char * filename);				//return 1 if file, 0 otherwise
int fs_isDir(char * pathname);				//return 1 if directory, 0 otherwise
int fs_delete(char* filename);				//removes a file

// Helper functions (written in fsHelpers.c)
bool DEisDirectory(struct DE *de); 			//return true if directory, false if file/other
DE* LoadedDir(DE * entry);

struct DE* setDotEntry(struct DE *entry);   //helper for setting "." entry
void setDotDotEntry(struct DE *entry);		// helper for setting ".." entry
int initDirectoryEntry(struct DE *entry);   //initializes a directory entry struct

int parsePath(char* path, struct ppReturn* info);




// This is the strucutre that is filled in from a call to fs_stat
struct fs_stat
	{
	off_t     st_size;    		/* total size, in bytes */
	blksize_t st_blksize; 		/* blocksize for file system I/O */
	blkcnt_t  st_blocks;  		/* number of 512B blocks allocated */
	time_t    st_accesstime;   	/* time of last access */
	time_t    st_modtime;   	/* time of last modification */
	time_t    st_createtime;   	/* time of last status change */
	
	/* add additional attributes here for your file system */
	};

int fs_stat(const char *path, struct fs_stat *buf);

#endif

