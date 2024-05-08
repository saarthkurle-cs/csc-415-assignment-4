/**************************************************************
* Class::  CSC-415-0# Spring 2024
* Name:: Jessica, Kalise, Kayla, and Saarth 
* Student IDs:: 921113528, 922199158, 921913613, 922266706
* GitHub-Name:: icebearnose (main git-hub)
* Group-Name:: The Task Masters 
* Project:: Basic File System
*
* File:: mfs.c
*
* Description:: 
*	Implemented file system interface functions.
*
**************************************************************/

#include "mfs.h"

char * fs_getcwd(char *pathname, size_t size){
	memcpy(pathname, loadedCWD->name, size);
	return pathname;
}

int fs_setcwd(char *pathname){
	
}
