/**************************************************************
* Class::  CSC-415-02 Spring 2024
* Name:: Jessica, Kalise, Kayla, and Saarth 
* Student IDs:: 921113528, 922199158, 921913613, 922266706
* GitHub-Name:: icebearnose, k-g-s03, kaylaridesbikes, saarthkurle-cs
* Group-Name:: The Task Masters
* Project:: Basic File System
*
* File:: freeSpaceManager.c
*
* Description:: It creates and initalizes free space bitmap.
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_BLOCKS 1024 

unsigned char* bitmap;

// funciton to initialize the free space bitmap
void initBitmap() {
     // allocates memory for the bitmap. The calculation ensures one bit per block.
    bitmap = (unsigned char*) calloc(TOTAL_BLOCKS / 8 + (TOTAL_BLOCKS % 8 != 0), sizeof(unsigned char)); // Bitmap initialized with all bits set to 0
    if (!bitmap) { // checks for allocation failure
        perror("Failed to allocate free space bitmap");
        exit(EXIT_FAILURE);
    }
}

// returns the starting block number or -1 if insufficient space is found
int allocateBlocks(int num) {
    int start = -1, found = 0;

    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        // check if the block is free
        if (!(bitmap[i / 8] & (1 << (i % 8)))) {
            if (start == -1) start = i;
            found++;
            if (found == num) {
                // mark blocks as occupied
                for (int j = start; j < start + num; j++)
                    bitmap[j / 8] |= (1 << (j % 8));
                return start;
            }
        } else {
            start = -1; // reset if a block is not free
            found = 0;
        }
    }
    return -1; // return -1 if not enough space is found
}



int main() {
    initBitmap(); // Initialize the free space bitmap

    // attempt to allocate 10 blocks
    int start_block = allocateBlocks(10);
    if (start_block != -1) { // if allocation was successful 
        printf("Successfully allocated 10 blocks starting at block %d.\n", start_block);
    } else {
        printf("Failed to allocate 10 blocks. Insufficient free space.\n");
    }

    free(bitmap); // free space after run
    return 0;
}
