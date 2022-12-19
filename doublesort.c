/*================================================================================
 * Project Info Header
 *================================================================================
 *================================================================================
 *  Author: Kevin Penton 
 *  Panther ID: 6173069
 *
 *  Certification: I understand FIU's academic policies, and I certify
 *                 that this work is my own and that none of it is the work of any 
 *                 other person.
 *
 *  Program Description: The purpose of this program is to sort the words in an
 *                       input file (or from standard input) and print the sorted
 *                       words  to an output file (or standard output).
 *  
 *  Usage: % doublesort [-cd] [-o output_file_name] [input_file_name]
 *
 *================================================================================
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

//node struct
typedef struct node {
	char* data;

	struct node* prev;
	struct node* next;
}node;

//creates the tail and head of the node
node* head = NULL; 
node* tail = NULL;

// sets the size of the list
int listSize = 0;


//function to create a node
struct node* createNode(char* data) {
	struct node* newNode = (struct node*)malloc(sizeof(node));
	newNode->data = malloc(sizeof(char) * 100);
	strcpy(newNode->data, data);
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}


void insertNext(char* data) {
	//creates a node using data as parameters
	struct node* newNode = createNode(data);
       
       //implements head and tail to advance on the sequence
	if(head == NULL) {
		head = newNode;
	}
	else {
		if(tail == NULL) {
			tail = head;
			head = newNode;
			tail->prev = head;
			head->next = tail;
		}
		else {
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
		}
		
	}
	listSize++;
}

//function to create the double linked list using the head and tail 

void createList() {
	struct node* current = head;
	printf("%s\n", head->data);
	while(current->next) {
		if(current->next == tail) {
			break;
		}
		current = current->next;
		printf("%s\n", current->data);
	}
	printf("%s\n", tail->data);
}


//function to free the nodes
void freeNodes() {
	struct node* current = head;
	while(current->next) {
		if(current->next == tail) {
			free(tail->data);
			free(tail);
			break;
		}
		current = current->next;
		free(current->data);
		free(current);
	}
}

// getter for the total of letters in Ascii
int getAsciiTotal(char letter, bool ignoreCase) {
	int const ASCII_DIFFERENCE = 32;
	int total = 0;
	int letterAscii = (int) letter;


	if (letterAscii >= 65 && letterAscii <= 90) { //uppercase
		total += letterAscii;
	}
	else if(letterAscii >= 97 && letterAscii <= 122 && ignoreCase) { //lowercase
		total += (letterAscii - ASCII_DIFFERENCE);
	}
	else if(letter >= 97 && letter <= 122 && !ignoreCase) { //lowercase
		total += letterAscii;
	}
	return total;
}

//uses a boolean to check if a word is in caps 
bool isInCaps(char* word) {
	int const ASCII_DIFFERENCE = 32;
	int total = 0;
	int size = strlen(word);
	for(int i = 0; i < size; i++) {
		int letterAscii = (int) word[i];
		
		if (letterAscii >= 65 && letterAscii <= 90) { //if is in caps = true 
			total += letterAscii;
		}
		else if(letterAscii >= 97 && letterAscii <= 122) { //if is in lowercase = false
			return false;
		}
	}

	return true;
}


//getter for  word using Ascii
int getWordInAscii(char* word, bool ignoreCase) {
	int const ASCII_DIFFERENCE = 32;
	int total = 0;
	int size = strlen(word);
	for(int i = 0; i < size; i++) {
		total += getAsciiTotal(word[i], ignoreCase);
	}
	return total;
}

//string comparison function using ASCii values
int asciicmp(char* str1, char* str2, bool ignoreCase) {
	int total1 = 0, total2 = 0;
	while( *str1 != '\0') {
		total1 += getAsciiTotal(*str1, ignoreCase);
		str1++;
	}
	while(*str2 != '\0') {
		total2 += getAsciiTotal(*str2, ignoreCase);
		str2++;
	}
	if(total1 < total2)
		return -1;
	else if(total1 > total2)
		return 1;
	else		
  		return 0;
}


//sets bubble sort in lexicographically ascending order

void lexiAscending(struct node* head, bool ignoreCase) {
	struct node* current = head;
	for(int i = 0; i < listSize-1; i++) {
		for(int j = 0; j < listSize - i - 1; j++) {
			int nextWordValue = getWordInAscii(current->next->data, ignoreCase);
			int currentWordValue = getWordInAscii(current->data, ignoreCase);

			if(currentWordValue < nextWordValue) {

				char* tmpWord = malloc(sizeof(char) * 100);
				strcpy(tmpWord, current->next->data);

				strcpy(current->next->data, current->data);
				strcpy(current->data, tmpWord);

			}
			current = head->next;
			if(current->next == tail || current->next == NULL)
				break;
		}
	}
}

//sets bubble sort in lexicographically descending order
void lexiDescending(struct node* head, bool ignoreCase) {
	struct node* current = head;
	for(int i = 0; i < listSize-1; i++) {
		for(int j = 0; j < listSize - i - 1; j++) {
			int nextWordValue = getWordInAscii(current->next->data, ignoreCase);
			int currentWordValue = getWordInAscii(current->data, ignoreCase);

			if(currentWordValue > nextWordValue) {

				char* tmpWord = malloc(sizeof(char) * 100);
				strcpy(tmpWord, current->next->data);
				strcpy(current->next->data, current->data);
				strcpy(current->data, tmpWord);
			}
			current = head->next;
			if(current->next == tail || current->next == NULL)
				break;
		}
	}
}


void bubbleSort(struct node* head, bool ignoreCase, bool ascending) {
	if(ascending) {
		lexiAscending(head, ignoreCase);
	}
	else {
		lexiDescending(head, ignoreCase);
	}
}


//process stdin input or file input
void processInput(char* inputFileName, bool ignoreCase) {
	char *tmpString = (char*)malloc(sizeof(char) * 100);
	tmpString[0] = '\0';
	
	if(inputFileName == NULL || inputFileName == "" || inputFileName == " ") {

		char* userInput = (char*)malloc(sizeof(char) * 100);
		//while(scanf("%s", userInput) == 1) {
		while(fgets(userInput, 100, stdin)) {
			if(asciicmp(userInput, "", false) == 0) 
				break;
		

			if(ignoreCase) {
				if(tmpString[0] == '\0') {
					insertNext(userInput);
					strcpy(tmpString, userInput);
				}
				else if(asciicmp(userInput, tmpString, ignoreCase) != 0) {
					strcpy(tmpString, userInput);
					insertNext(userInput);
				}
			}
			else {
				if(isInCaps(userInput) && tmpString[0] == '\0') {
					insertNext(userInput);
					strcpy(tmpString, userInput);
				}
				if(isInCaps(userInput) && asciicmp(userInput, tmpString, ignoreCase) != 0) {
					strcpy(tmpString, userInput);
					insertNext(userInput);
				}
			}
		}
		//free strings
		free(tmpString);
		free(userInput);
	}
	else {
		//read from file
		FILE* inFile = NULL;
		char* strRead = (char*)malloc(sizeof(char) * 100);
		
		inFile = fopen(inputFileName, "r"); //read mode

		if(inFile == NULL) {
			printf("Error: Could not open file %s.\n", inputFileName);
			return; //return error
		}

		char* tmp = (char*)malloc(sizeof(char) * 100);
		tmp[0] = '\0';
		while(!feof(inFile)) {
			fgets(strRead, 100, inFile);

			if(strRead[strlen(strRead)-1] == '\n') {
				strRead[strlen(strRead)-1] = '\0';
			}

			if(ignoreCase) {
				if(tmp[0] == '\0') {
					insertNext(strRead);
					strcpy(tmp, strRead);
				}
				else if(asciicmp(strRead, tmp, !ignoreCase) != 0) {
					strcpy(tmp, strRead);
					insertNext(strRead);
				}
			}
			else {
				if(isInCaps(strRead) && tmp[0] == '\0') {
					insertNext(strRead);
					strcpy(tmp, strRead);
				}
				else if(isInCaps(strRead) && asciicmp(strRead, tmp, ignoreCase) != 0) {
					strcpy(tmp, strRead);
					insertNext(strRead);
				}
			}
		}
		fclose(inFile);
		//free strings
		free(tmp);
		free(strRead);	
	}
}


int main(int argc, char **argv) {

	extern char *optarg;
	extern int optind;
	int option;
    int cflag = 0, oflag = 0, dflag = 0;
	char* inputFileName = NULL;
	char* outputFileName = NULL;
	static char usage[] = "usage: %s [-cd] [-o output_file_name] [input_file_name]";
	
	
        //Flags Section
	while ((option = getopt(argc, argv, "cdo:")) != -1) {
		switch (option) {
                //If -c is present the program needs to compare the strings case sensitive otherwise insensitive (optional)
		case 'c':
			cflag = 1;
			break;
		//output will be lexicographically descending order otherwise it will be lexicographically ascending order
		case 'd':
			dflag = 1;
			break;
               //print sorted words to output files (optional)
		case 'o':
			oflag = 1;
			break;
		}
	}


	//Validation Section
	if(opterr > 1) {
		return -1;
	}

	if(optind < argc) {
		outputFileName = malloc(strlen(argv[optind-1]));

		strcpy(outputFileName, argv[optind-1]);
		inputFileName = malloc(strlen(argv[optind]));

		strcpy(inputFileName, argv[optind]);

	}

	if(cflag == 1) { //compare the strings case sensitive
		processInput(inputFileName, true);
		if(dflag == 1) 
			bubbleSort(head, true, false);
		else
			bubbleSort(head, true, true);
	}
	else {//not case sensitive
		processInput(inputFileName, false);
		if(dflag == 1)
			bubbleSort(head, false, true);
		else
			bubbleSort(head, false, false);
	}
	if(oflag == 1) {
		FILE* outFile = fopen(outputFileName, "wb");
		if(!outFile) {
			printf("Error: Could not open file %s.\n", outputFileName);
			return -1; //return error
		}

		struct node* current = head;
		fprintf(outFile, "%s\n", current->data);
		while(current->next) {
			current = current->next;
			fprintf(outFile, "%s\n", current->data);
			if(current->next == tail) {
				break;
			}
		}

		fclose(outFile);

	}
	else {
		createList();
	}

	//free nodes
        freeNodes();
}
