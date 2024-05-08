#ifndef _FS_HELPERS_H
#define _FS_HELPERS_H

#include "mfs.h"  // Make sure this includes the DE structure and any necessary types

// Checks if the directory entry is a directory
bool DEisDirectory(struct DE *de);

// Loads a directory into memory (assumes DE is some kind of directory entry structure)
DE* LoadedDir(DE *entry);

// Initializes a directory entry, typically used for creating new directory entries
int initDirectoryEntry(struct DE *entry);

// Sets the '.' directory entry in a directory
struct DE* setDotEntry(struct DE *entry);

// Sets the '..' directory entry in a directory
void setDotDotEntry(struct DE *entry);

// Helper function to parse a filesystem path
int parsePath(char* path, struct ppReturn* info);

#endif // _FS_HELPERS_H

