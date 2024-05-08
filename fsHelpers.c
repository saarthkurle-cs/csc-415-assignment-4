/**************************************************************
* Class::  CSC-415-02 Spring 2024
* Name:: Jessica, Kalise, Kayla, and Saarth 
* Student IDs:: 921113528, 922199158, 921913613, 922266706
* GitHub-Name:: icebearnose (main git-hub)
* Group-Name:: The Task Masters 
* Project:: Basic File System
*
* File:: fsHelpers.c
*
* Description:: Helper functions for file system.
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

/*
	Determines if DE is a directory type. Returns true if 
	is directory, false if it is a file or other.
*/
bool DEisDirectory(struct DE *de){
	if(de->fileType == DIRECTORY_TYPE){
		return true;
	} else {
		return false;
	}
}

/*
	Finds an entry in a DE and returns it's index. 
	Returns -1 if does not exist
*/
int FindInDir(DE * de, char * entry){
	for(int i = 0; i < DIR_ENTRY_SIZE; i++){
		if(strcmp(de[i].name, entry) == 0){
			return i;
		}
	}
	return -1;
}

DE* LoadedDir(DE * entry){
	/*
		if(entry == NULL) return NULL;

		DE * loaded = malloc(entry->location)
	*/	
}

//the . for root directory is always the first entry
struct DE* setDotEntry(struct DE *entry) {
	strcpy(entry->name, ".");
	entry->id = 0;
	entry->location = 1;
	time_t now = time(NULL);
    entry->modified = now;
    entry->accessed = now;
    entry->created = now;

	strcpy(entry->owner, "NONE"); 
    entry->fileType = DIRECTORY_TYPE;
	return entry;
}

//the .. for root directory is always the second entry
void setDotDotEntry(struct DE *entry) {
	strcpy(entry->name, "..");
	entry->id = 1;
	entry->location = 1;
	time_t now = time(NULL);
    entry->modified = now;
    entry->accessed = now;
    entry->created = now;

	strcpy(entry->owner, "NONE"); 
    entry->fileType = DIRECTORY_TYPE;
}

/*
Initializes a Directory Entry. Returns the location of the entry (block number).
*/
int initDirectoryEntry(struct DE *entry){
	u_int64_t DEBytes = DIR_ENTRY_SIZE;
	return -1;
}

int parsePath(char* path, struct ppReturn* info){
	char* token;
	char* token2;
	char* saveptr;
	DE * parent;
	DE * startParent = NULL;
	DE * tempParent;
	int index = -1;
	//if path or info is invalid return error
	if(path == NULL){return -1;}
	if(info == NULL){return -1;}

	if(path[0] == '/'){	//case of absolute path
		startParent = root;
	} else {
		startParent = loadedCWD;
	}

	parent = startParent;

	token = strtok_r(path, "/", &saveptr);

	if(token == NULL){
		//in the case there is only "/" entered returns error
		if(path[0] !='/'){
			return -1;
		} else {
			info->lastElementIndex = -2;
			info->lastElementName = NULL;
			info->parent = parent;
			return 0;
		}
	}
	while(1){
		index = FindInDir(parent, token);
		token2 = strtok_r(NULL, '/', &saveptr);

	
		if(token2 == NULL){	//on last element
			info->lastElementName = token; 
			info->lastElementIndex = index;
			info->parent = parent;
			return 0;
		}
		if(index == -1){
			return -1;	//invalid path
		}	

		if(!DEisDirectory(&parent[index])){
			return -1;	//invalid path
		}

		tempParent = LoadedDir(&parent[index]);
		if(parent != startParent){
			free(parent);
		}
		parent = tempParent;
		token = token2;
	}
}