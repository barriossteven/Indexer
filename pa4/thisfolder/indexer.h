#ifndef INDEXER_H
#define INDEXER_H
#include <stdlib.h>
#include"idk.h"


//typedef struct fileNode{
//	char* file;
//	int occurence;	
//	struct fileNode* next;
//	struct fileNode* prev;
//}fNode;

//typedef struct wordNode{
//	char* word;
//	struct wordNode* next;
//	fNode* fileList;
//}wNode;


//wNode* createWNode(char* name);
//fNode* createFNode();
void indexFile (char* filename, SortedListPtr list);
void indexDir(char* dirname, SortedListPtr list);
int checkFile();
int checkDir();

int compareWords(void* a, void* b);
void destructWord(void* a);



#endif
