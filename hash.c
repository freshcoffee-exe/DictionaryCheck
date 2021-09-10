//============================================================================= 
// PROGRAMMER:    Eren Ozbay      
// PANTHER ID:    2452687
// CLASS:         COP4338, Online Section  
// SEMESTER:      Summer 2021 
// Project:       Assignment 6
// DUE:           07/16/2021
//=============================================================================
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<string.h>
#define HASH_SIZE 10000
struct bucket {
	char* string;
	struct bucket* next;
};
typedef struct bucket Bucket;
Bucket dictionary[HASH_SIZE];//hash table...

unsigned hash(char* key) {
	unsigned val = 0;
	while (*key)//as long as key is not pointing to the \0 at the end of the string
		val = *(key++) + 17 * val;
	return val % HASH_SIZE;
}
void init() {//initialize the buckets of my hash table...
	int i;
	for (i = 0; i < HASH_SIZE;i++)
		dictionary[i].string = NULL;
}
void printDictionary() {
	int i = 0;
	Bucket* iterator;
	for (; i < HASH_SIZE; i++)
		if (dictionary[i].string) {//not an empty bucket
			iterator = &dictionary[i];//head of linked list
			while (iterator) {
				printf("%s\n", iterator->string);
				iterator = iterator->next;//advances the iterator...
			}
		}
}
int dictionaryInsert(char* word) {
	//if (dictionarySearch(word))
		//return 0;//duplicate value, do not add it to the hash table!
	unsigned index = hash(word);
	if (dictionary[index].string == NULL) {//empty bucket
		dictionary[index].string = (char*)malloc(strlen(word) + 1);
		strcpy(dictionary[index].string, word);
		dictionary[index].next = NULL;
		return 1;//successful insert
	}
	char* currentHeadOfLinkedList = dictionary[index].next;
	dictionary[index].next = (Bucket*)malloc(sizeof(Bucket));
	dictionary[index].next->string = (char*)malloc(strlen(word) + 1);
	strcpy(dictionary[index].next->string, word);
	dictionary[index].next->next = currentHeadOfLinkedList;
	return 1;
}
int dictionarySearch(char* word) {//returns 1 if successful and 0 if not
	int i = hash(word);
	Bucket* iterator;//link list iterator...
	if (dictionary[i].string) {//not an empty bucket
		iterator = dictionary + i;//head of linked list
		while (iterator) {
			if (!strcmp(iterator->string, word))//matched
				return 1;
			iterator = iterator->next;
		}
	}
	return 0;//unsuccessful search
}
//argv[0] is always the program name
//argv[1] is always the first argument
//argv[2] is always the second argument
int main(int argc, char** argv) {
	int temp;
	if (argc != 3) {
		printf("Error: the program needs two command line arguments: dictionary.txt input.txt");
		return 1;
	}

	printf("The raw textual data for spell checking is stored in file %s\n", argv[2]);
	printf("The mispelled words are: \n");


	init();

	FILE* dict = fopen(argv[1], "r");
	FILE* input = fopen(argv[2], "r");

	//your code comes here!

	char * find=(char *)malloc(100);
	int i=0;

   for (;;) {	//searches the dictionary for words and stores them
       char temp=fgetc(dict);
       if((temp>='A' && temp<='Z') || (temp>='a' && temp<='z')){
           find[i++]=temp;
       }
       else if(i>0){
           find[i++]=0;
           dictionaryInsert(find);
           find=(char *) malloc(100);
		   i = 0;
       }
       else if(feof(dict)) { 
           break;
       }
   }

   for (;;) {		//searches the input for words and stores them
       char temp=fgetc(input);
       if((temp>='A' && temp<='Z') || (temp>='a' && temp<='z')){
           find[i++]=temp;
       }
       else if(i>0){
           find[i]=0;
           int match=dictionarySearch(find);
           if(!match) printf("%s\n",find);	//prints out words that do not match
           find=(char *) malloc(100);
		   i = 0;
       }
       else if(feof(input)) {
           break;
       }
      
   }

	//Tokenizing process using strtok....
	printf("\nTokenizing process using strtok....\n");
	char* token = (char*)malloc(100);
	char* separator = (char*)malloc(2);
	char* delimiter = "*/?!. @0123456789\"";
	strcpy(token, "hi@How\"are.y9ou?The weat'her*is/ni-ce\n");
	char* part = strtok(token, delimiter);
	printf("The first part is %s\n", part);
	while (part = strtok(NULL, delimiter))
		printf("The next part is %s\n", part);


	//Tokenizing process using sscanf....
	printf("Tokenizing process using sscanf....\n");

	//after reading input.txt file, you have seen the following line in it:
	//hi@How!are.y9ou?The weat'her*is/ni-ce\n
	strcpy(token, "hi@How!are.y9ou?The weat'her*is/ni-ce\n");
	part = (char*)malloc(100);
	//FILE* file = fopen("input.txt", "r");
	//int len = 0;
	//fscanf(file, "%[^\n]\n", token);
	//token[len = strlen(token)] = '\n';
	//token[len + 1] = '\0';

	while (temp = sscanf(token, "%[a-zA-Z\'\-]%c%[^\n]\n", part, separator, token)) {
		printf("The new part is %s with the separator %s\n", part,
			temp != 3 ? "new line" : *separator == ' ' ? "space" : *separator == '\t' ? "tab" : (separator[1] = '\0', separator));
		if (temp != 3)
			break;
	}
}