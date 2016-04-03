#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include"idk.h"
#include"indexer.h"

/***********************************************************************
 *indexFile takes char* filenae and SortedListPtr list and calls tokenizer
 * to tokenize the file and insert the word tokens into the list provided
 **********************************************************************/
void indexFile (char* filename, SortedListPtr list){
	char* buffer = NULL;
	FILE* f1 = fopen(filename,"r");
	if(f1==NULL){
		printf("file pointer NULL\n");
		return;
	}
	fseek(f1,0,SEEK_END);
	size_t fsize = ftell(f1);
	rewind(f1);
	buffer = (char*)malloc(sizeof(char)*(fsize+1));
	if(buffer == NULL){
		printf("could not allocate\n");
		return;
	}

	if(fread(buffer,fsize,1,f1) == 0){
		//printf("could not read\n");
		return;
	}

	char* fname = (char*)malloc(sizeof(char)*(strlen(filename)+1));
	strcpy(fname,filename);
	fname[strlen(filename)] = '\0';
	buffer[fsize] = '\0';
	fclose(f1);


	struct TokenizerT_ *tokens = TKCreate(buffer); /*create struct, with command line arguement as string*/
	int index = tokens->start; /*create an index that will keep track of where the tokenizer is in the string*/
	if(tokens->stoken[0] == '\0'){ /*if user has inserted an empty string, print out error message and return*/
		//printf("ERROR: CANNOT HAVE NULL TOKENIZER STRING\n");
		return;
	}
	//printf("Tokenizer \"%s\"\n",tokens->stoken); /*print out the full string of tokens*/
	while(index < strlen(buffer)){ /*while we have not reached the end of the string*/
		char *singletoken = TKGetNextToken(tokens); /*continue to get new tokens*/
		if(isdigit(singletoken[0]) || ispunct(singletoken[0])){
			index = tokens->start;
		}else{
			//char *type = getmytype(singletoken); /*identify the type of token*/
			//printf("%s \"%s\"\n", type,singletoken); /*print out token and its type*i
			SLInsert(list,singletoken, fname);
			index = tokens->start;/*move the index*/
		}
	}
	TKDestroy(tokens); /*free the space allocated for the the tokenizer*/
}


/***********************************************************************
 *indexDir takes char* dirname and SortedListPtr list and checks to see if dir
 * is NULL, if not NULL then it will cycle through the files calling indexFile 
 * to insert its data into the list provided. If while checking for files it finds
 * a directory it will recursively go through all of its files and directories.
 **********************************************************************/
void indexDir(char* dirname, SortedListPtr list){
	DIR* dir;
	struct dirent* dentry;
	dir = opendir(dirname);
	if(dir == NULL){
		printf("failed to open directory\n");
		return;
	}
	while((dentry = readdir(dir))!=NULL){
		size_t size = strlen(dentry->d_name) + strlen(dirname) +2;
		char* name = calloc(size, sizeof(char));
		snprintf(name, size, "%s/%s", dirname, dentry->d_name);


		if(strcmp(dentry->d_name,".") == 0 || strcmp(dentry->d_name, "..") == 0){
			continue;

		}

		if(checkDir(name)){
			indexDir(name, list);
		}else if(checkFile(name)){
			indexFile(name, list);
		}

	}
	closedir(dir);



}

/***********************************************************************
 *checkFile takes arguement and checks to see whether it is a file, if yes it will
 * return a positive int. 
 **********************************************************************/
int checkFile(char* filename){
	struct stat path_stat;
	stat(filename, &path_stat);
	int status = S_ISREG(path_stat.st_mode);
	return status;
}

/***********************************************************************
 *checkDir takes arguement and checks to see whether it is a dir, if yes it will
 * return a positive int. 
 **********************************************************************/
int checkDir(char* dirname){
	struct stat path_stat;
	stat(dirname, &path_stat);
	int status = S_ISDIR(path_stat.st_mode);
	return status;
}

/***********************************************************************
 *compareWords takes two void pointers and casts them to char* and compares them
 * it uses strcmp and returns -1, 0, or 1 if a is bigger less than b, equal to be, or greater than b. 
 **********************************************************************/
int compareWords(void* a, void* b){
	char* aa = (char*)a;
	char* bb = (char*)b;
	int comp = strcmp(aa,bb);
	if(comp < 0){
		return -1;
	}else if(comp  > 0){
		return 1;
	}else if(comp == 0){
		return 0;
	}
	printf("error\n");
	return 999;	
}

void destructWord(void* a){
	free(a);
}

/***********************************************************************
 *printfFile takes a sorted list and the filename the user wants to write to.
 * it traverses the list of word nodes then traverses list of file nodes to print out. 
 **********************************************************************/
void printFile(SortedListPtr list, char* filename){
	FILE *fp;
	char *mode = "w";
	fp = fopen(filename, mode);
	if(fp == NULL){
		printf("failed to open\n");
	}
	Node* ptr = list->head;
	if(ptr == NULL){
		fclose(fp);
		return;
	}
	FNode* fptr;
	fprintf(fp,"{\"list\" : [\n");
	while(ptr!=NULL){
		fprintf(fp,"\t{\"%s\" : [\n", (char*)ptr->data);
		fptr = ptr->flist;
		while(fptr!= NULL){
			fprintf(fp,"\t\t{%s : %d},\n",fptr->file, fptr->occurence);
			fptr = fptr->next;
			if(fptr ==NULL){
				fprintf(fp,"\t]},\n");
			}
		}
		ptr = ptr->next;
		if(ptr ==NULL){
			fprintf(fp, "]}");	
		}
	}
	fclose(fp);
	//printf("\n");


}

/***********************************************************************
 *arrangeFiles takes a sortedlist and traverses the list of word nodes and rearraanges the common occurence
 * files in alphabetical order. 
 **********************************************************************/
void arrangeFiles(SortedListPtr list){

	Node* ptr = list->head;
	FNode* curr;
	FNode* behind;
	while(ptr!=NULL){
		curr = ptr->flist->next;
		behind = ptr->flist;
		while(curr!= NULL){
			if(behind->occurence == curr->occurence){
				if(compareWords(behind->file,curr->file) > 0){//means curr needs to go before behind
					behind->next = curr->next;
					if(curr->next!=NULL){
						curr->next->prev = behind;
					}
					curr->prev = behind->prev;
					if(behind->prev!=NULL){
						behind->prev->next = curr;
					}
					curr->next = behind;
					behind->prev = curr;
					curr = behind->next;
					continue;
				}	
				behind = behind->next;
				curr = curr->next;
				continue;
			}else{
				behind = behind->next;
				curr = curr->next;
				continue;
			}	
		}
		if(ptr->flist->prev != NULL){
			while(ptr->flist->prev !=NULL){
				ptr->flist = ptr->flist->prev;
			}
		}


		ptr = ptr->next;
	}

}

int main(int argc, char **argv) {
	if(argc != 3){
		printf("not enough args\n");
		return 0;
	}

	SortedListPtr s1;
	s1 = SLCreate(compareWords, destructWord);

	if(checkFile(argv[2])){
		indexFile(argv[2],s1);

	}else if(checkDir(argv[2])){
		indexDir(argv[2],s1);

	}else{
		printf("its nothing\n");
	}
	if(s1 != NULL){
		//call function to alphabetically order the files of same occurence
		arrangeFiles(s1);
	}	
	if(s1!=NULL){
		printFile(s1, argv[1]);
	}
	if(s1 != NULL){

		SLDestroy(s1);
	}

	return 0;
}



