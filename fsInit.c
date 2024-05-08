/**************************************************************
* Class::  CSC-415-02 Spring 2024
* Name:: Jessica, Kalise, Kayla, and Saarth 
* Student IDs:: 921113528, 922199158, 921913613, 922266706
* GitHub-Name:: icebearnose (main git-hub)
* Group-Name:: The Task Masters 
* Project:: Basic File System
*
* File:: fsInit.c
*
* Description:: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "fsLow.h"
#include "mfs.h"

#define NUM_DIR_ENTRIES 51
#define MAGICNUM 0x55555555 

DE* loadedCWD = NULL;
char* loadedCWDString = NULL;
	
int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize){
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", 
	numberOfBlocks, blockSize);

	/* TODO: Add any code you need to initialize your file system. */
	
	//initialize VCB pointer for the VCB structure
	struct VCB *VCB = (struct VCB*)malloc(blockSize); 

	//initialize the root directory
	struct DE *RD = (struct DE*)malloc(DIR_ENTRY_SIZE * blockSize); 
	
	LBAread(VCB,1,0);

	loadedCWDString = strdup("/");
	root = setDotEntry(&RD[0]);
	loadedCWD = root;
	
	setDotDotEntry(&RD[1]);

	if(VCB->signature != MAGICNUM){
	  //Initialize values from VCB
	  strcpy(VCB->name, "Name of the file system");

	  VCB->signature = 0x55555555;
	  VCB->total = numberOfBlocks;
	  VCB->free = numberOfBlocks; // Initially, all blocks are free
          VCB->blockSize = blockSize;
          VCB->location = 0; // Example location
	}

	LBAwrite(VCB,1,0); //write the VCB to the disk

	//*Freespacemap should be here*

	// Read the root directory from the disk
	LBAread(RD, NUM_DIR_ENTRIES, 1);

	// Initialize directory entries
    for (int i = 2; i < NUM_DIR_ENTRIES; i++) {
        // Initialize each directory entry to be in a known free state
        // Set other attributes as needed

		// initDirectoryEntry(&RD[i]); 
    }

	//Number of blocks needed for the root directory
	//I would look over this, it might be wrong

	u_int64_t numBytes = NUM_DIR_ENTRIES * DIR_ENTRY_SIZE; 

	// simple calc, before we determine if more can fit in the block
	int numRootDirBlocks = (numBytes / blockSize); 

	// totalBytes is more than enough that is needed, so we need to determine what can fit
	// in the space that is left over
	u_int64_t totalBytes = numRootDirBlocks * 512; // total space we have in bytes

	u_int64_t unused_bytes = totalBytes - numBytes;
	
	//LBAwrite(RD, numRootDirBlocks, 1); // Write the root directory to the disk

	return 0;
	}

	//for these functions, maybe we can declare a variable as a startingBlock?

	//Helper function to allocate free space

	
void exitFileSystem ()
	{
	printf ("System exiting\n");
	}
