#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "idk.h"
#include"indexer.h"



TokenizerT *TKCreate( char * ts ) {
	if(ts == NULL){
		printf("ERROR: NULL STRING"); /*return error if there is no input string*/
	}else{
		struct TokenizerT_ *tokenizer_t = malloc(sizeof(struct TokenizerT_)); /*allocate space for the tokenizer struct*/
		tokenizer_t->stoken = ts; /*tokenizer string is the input string*/
		tokenizer_t->start = 0; /*start begins at the beggining of the input string, which at creation of tokenizer is 0*/
		tokenizer_t->end = 0; /*end will not be specified until getnexttoken so for now it is 0*/
		return tokenizer_t;
	}

	return NULL;
}


void TKDestroy( TokenizerT * tk ) { /*Frees Tokenizer Struct*/
	if(tk != NULL){
		free(tk);
	}
}

char *getmytype(char * ts){ /*getmytype returns the name of type of the token, so word, octal, hex etc, or the specific type of puntutation*/
	if(isalpha(ts[0])){
		return "word"; 
	}
	if(ispunct(ts[0])){
		char *tokenp = ts; /*create a new string to hold just the token*/
		if(strcmp(tokenp,"{") == 0){ /*checks all types of punctuation and returns type*/
			return "left brace";
		}else if(strcmp(tokenp,"}") == 0){
			return "right brace";
		}else if(strcmp(tokenp,"(") == 0){
			return "left parenthesis";
		}else if(strcmp(tokenp,")") == 0){
			return "right parenthesis";
		}else if(strcmp(tokenp,".") == 0){
			return "period";
		}else if(strcmp(tokenp,"?") == 0){
			return "question mark";
		}else if(strcmp(tokenp, "$") == 0){
			return "dollar sign";	
		}else if(strcmp(tokenp, "->") == 0){
			return "structure pointer";	
		}else if(strcmp(tokenp, "&&") == 0){
			return "logical and";	
		}else if(strcmp(tokenp, "||") == 0){
			return "logical or";
		}else if(strcmp(tokenp, "++") == 0){
			return "increment";	
		}else if(strcmp(tokenp, "--") == 0){
			return "decrement";		
		}else if(strcmp(tokenp, "-") == 0){
			return "minus";	
		}else if(strcmp(tokenp, "+") == 0){
			return "plus";	
		}else if(strcmp(tokenp, "[") == 0){
			return "left brace";	
		}else if(strcmp(tokenp, "]") == 0){
			return "right brace";	
		}else if(strcmp(tokenp, "<<") == 0){
			return "left shift";
		}else if(strcmp(tokenp, ">>") == 0){
			return "right shift";	
		}else if(strcmp(tokenp, "==") == 0){
			return "equal";
		}else if(strcmp(tokenp, "&") == 0){
			return "address";
		}else if(strcmp(tokenp, "*") == 0){
			return "multiply";
		}else if(strcmp(tokenp, "!=") == 0){
			return "equal";
		}else if(strcmp(tokenp, "~") == 0){
			return "1s  complement";
		}else if(strcmp(tokenp, "<=") == 0){
			return "less than or equal";
		}else if(strcmp(tokenp, ">=") == 0){
			return "greater than or equal";
		}else if(strcmp(tokenp, "/") == 0){
			return "divide";
		}else if(strcmp(tokenp, "<") == 0){
			return "less than";
		}else if(strcmp(tokenp, ">") == 0){
			return "greater than";
		}else if(strcmp(tokenp, "+=") == 0){
			return "plus equals";
		}else if(strcmp(tokenp, "\"") == 0){
			return "quotation mark";
		}else if(strcmp(tokenp, "%%") == 0){
			return "modulos";
		}else if(strcmp(tokenp, ";") == 0){
			return "semicolon";
		}else if(strcmp(tokenp, ":") == 0){
			return "colon";
		}else if(strcmp(tokenp, "~") == 0){
			return "1's complement";
		}else if(strcmp(tokenp, "'") == 0){
			return "apostrophe";
		}else if(strcmp(tokenp, "#") == 0){
			return "hash";
		}else if(strcmp(tokenp, "^") == 0){
			return "bitwise exclusive or";
		}else if(strcmp(tokenp, ",") == 0){
			return "commma";
		}else{
			return "punctuation";
		}	
	}if(isdigit(ts[0])){
		int countdecimal = 0; /*counts the number of . in the token*/
		int countx = 0; /*counts the number x's in the token*/
		int counte = 0; /*counts the number of e's in the token*/
		int countminus = 0; /*counts the number of minuses in the token*/
		int countplus = 0; /*counts the number of pluses in the token*/
		int amioctal = 0; /*counts how many digits in the token are 0 - 8 */
		int i = 0;/*index*/
		for(; i < strlen(ts);i++){
			if(ts[i] == '.'){
				countdecimal++; /*if decimal increase count*/
				if(ts[i+1] == '\0'){
					return "malformed floating point number";
				}if(ts[i+1] == 'e' || ts[i+1] == '+' || ts[i+1] == '-'){ /*checks to see that e,+,- is not last character in token*/
					return "malformed floating point number";
				}
			}else if(ts[i] == 'x' || ts[i] == 'X'){ /*if x increase countx*/
				countx++;
				if(ts[i+1] == '\0'){ /*checks to see that x is not last character in token*/
					return "malformed hexadecimal number";
				}else{
					continue;
				}
			}else if(ts[i] == '+'){ /*if plus increase countplus*/
				countplus++;
				if(ts[i+1] == '\0'){ /*checks to see that plus is not last character in token*/
					return "malformed floating point number";
				}else{
					continue;
				}
			}else if(ts[i] == 'e'){ /*if e increase counte*/
				counte++;
				if(ts[i+1] == '\0'){ /*if the token after is the end of the string, this is a malformed floating point number*/
					return "malformed floating point number";
				}else{
					continue;
				}
			}else if(ts[i] > 8 || ts[i] == 8){ /*if the digit is between 0 and 8, icrease the number of octal numbers in string*/
				amioctal++;
			}else if(ts[i] == '-'){ /*if minus increase countminus*/
				countminus++;
			}else{
				continue;
			}
		}
		if((counte == 1 && countdecimal == 0) || (counte == 0 && countplus > 0 ) || (counte == 0 && countminus > 0) || (countdecimal == 0 && countplus > 0) || (countdecimal == 0 && countminus > 0)){
			return "malformed floating point number";
		}if(countx == 1 && ts[0] != '0' && ts[1] != 'x'){
			return "malformed hex constant"; 
		}
		if(countdecimal > 1){ /*if the number of decimal points in the token is greater than one, this is a malformed decimal number*/
			return "malformed decimal number";
		}if(countx > 1){ /*if the number of x's is greater than one, this is a malformed hex number*/
			return "malformed hex number";
		}if(counte > 1){ /*if the number of e's in the token is greater than one, this is a malformed floating point number*/
			return "malformed floating point number";
		}if((countplus == 1 && countminus == 1) || countplus > 1 || countminus > 1){ /*if there is more than one plus, or more than one minus, or pluses and minuses, then this is a malformed floating point number*/
			return "malformed floating point number";
		}if((countdecimal == 1 && counte == 1 && countplus == 1) || (countdecimal == 1 && counte == 1 && countminus == 1) || countdecimal == 1){ /*if there is one e, one decimal, one minus/plus, or one decimal this is a floating point number*/
			return "floating point number";
		}if(ts[0] == '0' && ts[1] == 'x'){ /*if the first character is a zero and the second character is an x, then this is a hex constant*/
			return "hex constant";
		}if(ts[0] == '0' && amioctal == strlen(ts)){ /*if all the charcters are between 0 and 8 and the first digit is 0, this is an octal constant*/
			return "octal constant";
		}else{
			return "integer"; /*if none of the above apply, this is just a string of digits*/
		}
	}else{
		return "not valid type"; /*if token does not fall under one of the above punctuation, word or digits, than it is not a valid type*/
	}


}

char *TKGetNextToken( TokenizerT * tk ){
	if(tk == NULL){
		printf("ERROR: RECEIVED NULL TOKENIZER");
	}
	int current = tk->start;
	if(isspace(tk->stoken[current])){
		current = current +1;
		tk->start = current;
	}
	while(!isspace(tk->stoken[current])){
		/* ALPHABET METHOD THIS GETS WORD TOKENS SEPARATED BY WHITE SPACE OR NON-ALPHABETIC CHARACTERS***************/
		if(isalpha(tk->stoken[current])){ /*if the current character is alpha*/
			while(isalpha(tk->stoken[current]) || isdigit(tk->stoken[current])){ /*while the tokens after it are still alpha, continue moving pointer*/
				current = current + 1;
			}
			tk->end = current - 1; /*characters are no longer alpha, end of the word is one before current*/
			char *newToken;
			int wSize = current-tk->start;
			newToken = (char*)malloc(sizeof(char)*(wSize+1));
			strncpy(newToken,tk->stoken+tk->start,(current - tk->start)); /*insert the token into new char array*/
			newToken[wSize] = '\0';
			if(isspace(tk->stoken[current])){ /*if the character after the word is space, move the pointer beyond the whitespace*/
				tk->start = current + 1;
				return newToken;
			}else{
				tk->start = current; /*if the character after the word is not space, the next token begins at current*/
				return newToken;
			}
			/* END OF ALPHABET METHOD ***********************************************************************************/
			/*BEGIN PUNCTUATION IF STATEMENT CREATES PUNCTUATION OR OPERATOR TOKENS SEPARATED BY ANY OTHER TOKEN OR WHITE SPACE******/
		}else if(ispunct(tk->stoken[current])) { /*if the character at current is punctuation or operator*/
			char *mytokenone; /*create string to hold substring*/
			mytokenone = (char*)malloc(2);
			strncpy(mytokenone,tk->stoken+current,2); /*copy two characters out of the string*/
			char *newToken; /*create string, new token to return*/
			if(strcmp(mytokenone,"<<") == 0 || strcmp(mytokenone, ">>") == 0 || strcmp(mytokenone, "++") == 0 || strcmp(mytokenone, "+=") == 0 
					|| strcmp(mytokenone, "==") == 0 || strcmp(mytokenone, "!=") == 0 || strcmp(mytokenone, "&&") == 0 || strcmp(mytokenone, "||") == 0
					|| strcmp(mytokenone, ">=") == 0 || strcmp(mytokenone, "<=") == 0 || strcmp(mytokenone, "--") == 0 || strcmp(mytokenone, "->") == 0){
				newToken = (char*)malloc(4); /*check if the substring of length 2 contains any of the punctuation that have length 2*/
				strncpy(newToken,tk->stoken+current, 2); /*if it is a punctuation of length 2 create a new token with the punctuation*/
				if(isspace(tk->stoken[current+3])){ /*if there is a space after the token, move the current index past the space and reset start*/
					current = current + 3;
					tk->start = current;
					return newToken;
				}else{
					current = current + 2; /*if there is no space after the token, move current index to the next token and reset start*/
					tk->start = current;
					return newToken;
				}

			}else{
				newToken = (char*)malloc(1); /*if the punctuation is of length 1*/
				strncpy(newToken,tk->stoken+current,1); /*put punctuation into new string and returns the token*/
				if(isspace(tk->stoken[current+1])){ /*if the character after the token is a space, move the current index past the space*/
					current = current + 2;
					tk->start = current; /*reset start to current index*/
					return newToken;
				}else{
					current = current + 1; /*if the next character is not a space, move the current index to the next token*/ 
					tk->start = current;
					return newToken;
				}
			}
			/* USE \"hello\" TO USE CHARACTERS FOR " */

			/*END OF PUNCTUATION IF-STATEMENT******************************************************************************/
			/*START OF OCTAL,HEX, DECIMAL,FLOATING POINT CONSTANT***********************************************************/
		}if(isdigit(tk->stoken[current])){
			while(isdigit(tk->stoken[current]) || tk->stoken[current] == '.' || tk-> stoken[current] == 'e' || tk->stoken[current] == '-' || tk->stoken[current] == '+' || tk->stoken[current] == 'x' || tk->stoken[current] == 'X'
					|| tk->stoken[current] == 'a' || tk->stoken[current] == 'b' || tk->stoken[current] == 'c' || tk->stoken[current] == 'd' || tk->stoken[current] == 'e'
					|| tk->stoken[current] == 'f' || tk->stoken[current] == 'A' || tk->stoken[current] == 'B' || tk->stoken[current] == 'C' || tk->stoken[current] == 'D'
					|| tk->stoken[current] == 'E' || tk->stoken[current] == 'F'){
				current = current + 1;
			}

			tk->end = current - 1; /*characters are no longer alpha, end of the word is one before current*/
			char *newToken;
			newToken = (char*)malloc(current - tk->start);
			strncpy(newToken,tk->stoken+tk->start,(current - tk->start)); /*insert the token into new char array*/
			if(isspace(tk->stoken[current])){ /*if the character after the word is `space, move the pointer beyond the whitespace*/
				tk->start = current + 1;
				return newToken;
			}else{
				tk->start = current; /*if the character after the word is not space, the next token begins at current*/
				return newToken;
			}
		}else{ 
			current = current + 1;

		}
	}
	char* invalidtoken = "INVALID CHARACTERS";
	return invalidtoken;
}

/*================================================================================================================================================
  =====================================================================================================================================================\
  =====================================================================================================================================================*/

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
	SortedListPtr tmp = malloc(sizeof(struct SortedList));
	memset(tmp,0,sizeof(struct SortedList));
	tmp->d = df;
	tmp->c = cf;
	return tmp;
}


/***************************************************
  traverse and free.
 **************************************************/
void SLDestroy(SortedListPtr list){
	Node* curr = list->head;
	if(curr == NULL){
		//list is empty
		free(list);
		return;
	}
	FNode* fcurr = curr->flist;
	FNode* fdel = fcurr;
	Node* del = curr;
	while(curr !=0){
		while(fcurr!= NULL){
			fdel = fcurr;
			fcurr = fcurr->next;
			free(fdel->file);
			free(fdel);
		}
		del = curr;
		curr = curr->next;
		(list->d)(del->data);
		free(del);
	}
	free(list);
	return;

}


/***************************************************
 * Helper function for SLInsert, allocates space on heap for Node* and initializes values. n1->data points to newObj, next points to 0 or NULL,
 * and the amount of pointers pointing to this node is set to 1.
 **************************************************/
Node* create_node(void *newObj){

	Node* n1 = malloc(sizeof(Node));
	n1->data = newObj;
	n1->next = 0;
	n1->pointCount = 1;
	n1->flist = NULL;
	return n1;


}

FNode* create_fnode(void *newFile){

	FNode* n2 = malloc(sizeof(FNode));
	n2->file = newFile;
	n2->occurence = 1;
	n2->next = 0;
	n2->prev = 0;
	return n2;


}

void rearrangeFiles(FNode* currFile){
	FNode* ptr = currFile->prev;
	if(ptr == NULL){
		return;
	}
	if(ptr->occurence> currFile->occurence){
		return;
	}
	while(ptr !=NULL ){
		if(ptr->occurence< currFile->occurence){
			ptr->next = currFile->next;
			if(currFile->next!=NULL){
				currFile->prev = ptr;
			}
			currFile->prev = ptr->prev;
			if(ptr->prev != NULL){
				ptr->prev->next = currFile;
			}
			currFile->next = ptr;
			ptr->prev = currFile;
		}else {
			return;
		}
		ptr = currFile->prev;
	}	

}

/***************************************************
 *SLInsert is simple linked list insertion, it takes a list ptr, and void pointer to newObj. Node* tmp points to the node created in create_node helper function.
 * Four cases are covered in this function which are: head ==  NULL, if new node > head, new node belongs somewhere in the middle of the list, and new node 
 * belongs at the tail. Also, if something is a duplicate then it will immediately be freed upon detection.
 **************************************************/

int SLInsert(SortedListPtr list, void *newWord, void *newFile){
	//printf("in insert\n");
	//return 0;
	//Node* tmp = create_node(newWord);

	//base case if head == NULL
	if(list->head == 0){
		Node* tmp = create_node(newWord); 
		tmp->flist = create_fnode(newFile);
		//printf("in first case\n");
		list->head = tmp;
		return 1;
	}


	//if new node is bigger than head
	int comp = list->c(list->head->data,newWord);
	if(comp == 999){
		return 0;
	}
	if( comp > 0){
		Node* tmp = create_node(newWord);
		tmp->flist = create_fnode(newFile); 
		tmp->next = list->head;
		list->head = tmp;
		return 1;

	}else if( comp  == 0){
		//update occurences and file
		FNode* fptr = list->head->flist;
		FNode* prevptr;
		while(fptr!=NULL){
			if(strcmp(fptr->file, (char*)newFile) == 0){
				fptr->occurence++;
				rearrangeFiles(fptr);
				if( list->head->flist->prev != NULL){
					while(list->head->flist->prev !=NULL){
						list->head->flist = list->head->flist->prev;
					}
				}
				return 1;
			}else {
				prevptr = fptr;
				fptr = fptr->next;
			}
		}
		//here then create 
		prevptr->next = create_fnode(newFile);
		prevptr->next->prev = prevptr->next;
		return 1;
	}
	//if node is somewhere other than first or null
	Node* curr = list->head->next;
	Node* prev = list->head;
	while(curr != 0){
		if(list->c(curr->data,newWord) < 0){
			prev = curr;
			curr = curr->next;
			continue;
		}else if(list->c(curr->data,newWord) == 0){
			FNode* fptr = curr->flist;
			FNode* prevptr;
			while(fptr!=NULL){
				if(strcmp(fptr->file, (char*)newFile) == 0){
					fptr->occurence++;
					rearrangeFiles(fptr);
					if(curr->flist!= NULL){
						while(curr->flist->prev !=NULL){
							curr->flist = curr->flist->prev;
						}
					}
					return 1;
				}else {
					prevptr = fptr;
					fptr = fptr->next;
				}
			}
			//here then create 
			prevptr->next = create_fnode(newFile);
			prevptr->next->prev = prevptr;
			return 1;
		}else if(list->c(curr->data,newWord) > 0){
			Node* tmp = create_node(newWord);
			tmp->flist = create_fnode(newFile);
			prev->next = tmp;
			tmp->next = curr;
			return 1; 
		}
	}
	//inserts at tai	
	Node* tmp = create_node(newWord);
	tmp->flist = create_fnode(newFile);
	prev->next = tmp;
	return 1;


}


//need to delete***************************************************************************************
void print(SortedListPtr list){
	Node* curr = list->head;
	while(curr != 0){
		printf("%s  ", (char*)curr->data);
		curr = curr->next;
	}
	printf("\n");


}

/***************************************************
 * SLRemove takes list and void pointer to data, create Node* del to keep access to obj being removed. First case is if head is target and then if target is 
 * somewhere else. If pointCount == 0 then that means no iterator is pointing to it so we can remove and free memory immediately otherwise if it does not == 0 
 * then we simply remove it from list. 
 **************************************************/
int SLRemove(SortedListPtr list, void *newObj){
	Node* del;

	//if head is target
	if((list->c)(list->head->data,newObj)== 0){
		list->head->pointCount--;
		del = list->head;
		list->head = list->head->next;
		if(del->pointCount == 0){
			(list->d)(del->data);
			free(del);
		}
		return 1;
	}

	Node* curr = (Node*)list->head->next;
	Node* prev = list->head;

	while(curr != 0){
		if(list->c(curr->data,newObj)== 0){
			prev->next = curr->next;
			((Node*)prev->next)->pointCount++;
			curr->pointCount--;
			if(curr->pointCount == 0){
				((Node*)curr->next)->pointCount--;
				(list->d)(curr->data);
				free(curr);
				return 1;
			}
			return 1;
		}
		prev = curr;
		curr = curr->next;

	}

	return 0;
}

/***************************************************
 * SLCreateIterator takes list and allocates memory for struct SortedListIterator. Sets curr equal to the head of the list. If if head is not NULL then the 
 * pointCount to head is increased by one since that node has 1 more pointer to it.
 **************************************************/

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr tmp = malloc(1*sizeof(struct SortedListIterator));
	tmp->curr = list->head;
	if(tmp->curr != 0){
		list->head->pointCount++;
	}
	return tmp;
}

/***************************************************
 *SLDestroy takes iterator and has three cases: when the iterator points to nothing we simply free the allocated iterator memory,
 when the iterator points to something already removed from the sorted list and no other iterator points to it so the only pointer keeping access is the current
 iterator, and case 3 is when its pointCount is more than 1 we can simply free the iterator but not the data.
 **************************************************/

void SLDestroyIterator(SortedListIteratorPtr iter){
	if(iter->curr == NULL){
		free(iter);
		return;
	}
	//points to nothing
	if(iter->curr == 0){
		free(iter);
		return;
	}
	//points to something that is only accessible through this iterator
	if(iter->curr->pointCount == 1 ){
		free(iter->curr->data);
		free(iter->curr);
		free(iter);
		return;
	}
	//points to something currently in list or pointed by other iterator but you only want to get rid of iterator
	iter->curr = NULL;
	free(iter);

}

/***************************************************
 * SLNextItem takes an iterator, checks to see if curr is NULL, if not then moves node pointer to the next position. A temp node pointer is used to maintain access
 * to node. If tmp node pointCount is 0 then it has been removed from sorted list and no other iterator points to it so we can free the memory it takes up
 * otherwise we leave it alone. If the new node that iterator points to is not NULL then its pointCount increases by one and void* obj points to the new node's 
 * data. If new node is NULL then void pointer is NULL and it is returned.
 *
 **************************************************/
void* SLNextItem(SortedListIteratorPtr iter){
	void* obj;
	Node* tmp;
	if(iter->curr == 0){
		return NULL;
	}
	tmp = iter->curr;
	iter->curr = iter->curr->next;

	tmp->pointCount--;
	if(tmp->pointCount == 0){
		free(tmp->data);
		free(tmp);
	}
	if(iter->curr!=0){
		iter->curr->pointCount++;
		obj = iter->curr->data;
	}else{
		obj = 0;
		return obj ;
	}


	return obj;
}

/***************************************************
 *SLGetItem takes iterator and if current node is NULL then it is pointing to nothing so we return NULL, otherwise void* obj points to current node's data and 
 * returns obj.
 **************************************************/
void* SLGetItem(SortedListIteratorPtr iter){
	void* obj;
	if(iter->curr == 0){
		///iter is pointing to something null
		obj = 0;
		return obj;
	}
	obj = iter->curr->data;
	return obj;
}
/*============================================================================================================================================
  ==============================================================================================================================================
  ============================================================================================================================================*/


