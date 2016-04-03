#ifndef IDK_H
#define IDK_H
/*
 * sorted-list.h
 */

#include <stdlib.h>
//#include"indexer.h"

 struct TokenizerT_ { /*tokenizer struct holds the input string and two indexes, start and end which are the beginning and the end of tokens*/ 
	char *stoken;
    int start;
    int end;
};

typedef struct TokenizerT_ TokenizerT;


TokenizerT *TKCreate( char * ts ); 

void TKDestroy( TokenizerT * tk );

char *getmytype(char * ts);

char *TKGetNextToken( TokenizerT * tk );



typedef int (*CompareFuncT)( void *, void * );
typedef void (*DestructFuncT)(void*);


typedef struct node{
	void* data;
	struct node* next;
	int pointCount;
	struct fileNode* flist;
}Node;

typedef struct fileNode{
	char* file;
	int occurence;	
	struct fileNode* next;
	struct fileNode* prev;
}FNode;


struct SortedList
{
	int count;
	Node* head;
	CompareFuncT c;
	DestructFuncT d;

};

typedef struct SortedList* SortedListPtr;
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df);
void SLDestroy(SortedListPtr list);
int SLInsert(SortedListPtr list, void *newWord, void *newfile);
int SLRemove(SortedListPtr list, void *newObj);

struct SortedListIterator
{
	Node* curr;
};

typedef struct SortedListIterator* SortedListIteratorPtr;
SortedListIteratorPtr SLCreateIterator(SortedListPtr list);
void SLDestroyIterator(SortedListIteratorPtr iter);
void * SLNextItem(SortedListIteratorPtr iter);
void * SLGetItem( SortedListIteratorPtr iter );
void print(SortedListPtr list);

#endif
