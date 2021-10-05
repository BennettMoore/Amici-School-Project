/*
 * file: amici.c
 *
 * A new social network that can add and remove users, and manage friendships between users
 *
 * @author Bennett Moore bwm7637@rit.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hash.h"
#include "table.h"

#define _POSIX_C_SOURCE 200809L //Allows for strdup to work
#define BUF_SIZE 1024
#define MAX_COMMANDS 3
#define FRIEND_BLOCK 10

typedef struct person_s{
	char *first_name;		// First name
	char *last_name;		// Last name
	char *handle;			// Username
	struct person_s **friends;	// Dynamic collection of friends
	size_t friend_count;		// Current number of friends
	size_t max_friends;		// Maximum number of friends
} person_t;

Table t;


/*
 * Print function to initialize table
 *
 * @param key The key of the table entry
 * @param value The value of the table entry
 *
 */
void tablePrint(const void* key, const void* value){
	(void)key;
	person_t* p1 = (person_t*)value;
	printf("%s, %s (%s)\n", p1->first_name, p1->last_name, p1->handle);
}

/*
 * Delete table entry
 *
 * @param key The key of the table entry
 * @param value The value of the table entry
 *
 */
void tableDel(void* key, void* value){
	(void)key;
	person_t* p1 = (person_t*)value;

	free(p1->first_name);
	free(p1->last_name);
	free(p1->handle);
	for(size_t i = 0; i < p1->max_friends; i++){
		free(p1->friends[i]);
	}
	free(p1->friends);
}

/*
 * Clears database and reinitializes storage
 *
 */
void reformat(bool is_quit){
	if(is_quit) return; //To stop "Unused parameter" error
	//Free contents of table
	//Free table
	//If is_quit is false, create new, empty table
}

/*
 * Prints overall stats of the site
 *
 */
void printStats(){
	//If only one user, print that there is one user
	//Else print that there are N users
	//If only one friendship, print that there is one friendship
	//Else print that there are N friendships
}

/*
 * Prints info about a user
 *
 * @param handle The handle of a user
 *
 */
void printInfo(char * handle){
	if(handle != NULL) return; //To stop "Unused parameter" error
	//If user has no friends, print that they have no friends
	//Else if user has 1 friend, print that they have 1 friend and show friend
	//Else print that user has N friends and show each friend
}

/*
 * Adds or removes two users to/from each others' friend networks
 *
 * @param handles The handles of the two users
 *
 */
void friend(char * handles[], bool is_friendly){
	if(handles != NULL) return; //To stop "Unused parameter" error
	if(is_friendly) return; //To stop "Unused parameter" error
	//If is_friendly is true, then check if the users are already friends
		//If not friends, add them as friends and print to console
	//If is_friendly is false, then check if the uers are not friends
		//If friends, remove them as friends and print to console	
}

/*
 * Determines what command the user is attempting to perform, and run the function prescribed to it
 *
 * @param data An array of string commands
 *
 */
void parseCommands(char * data[MAX_COMMANDS]){
	if(strcmp(data[0], "add") == 0){
		//Format: add first_name last_name handle
		//If valid and vacant, add new person to hash table	
	}
	else if(strcmp(data[0], "friend") == 0){
		//Format: friend handle1 handle2
		//If valid, send to friend function (specify friend)
	}
	else if(strcmp(data[0], "init") == 0){
		//Format: init
		//Send to reformat function (specify not quitting)
	}
	else if(strcmp(data[0], "print") == 0){
		//Format: print handle
		//If valid, send to printInfo function
	}
	else if(strcmp(data[0], "quit") == 0){
		//Format: quit
		//Send to reformat function, then exit program (specify quitting)
	}
	else if(strcmp(data[0], "size") == 0){
		//Format: size handle
		//If valid, print friend_count of user
	}
	else if(strcmp(data[0], "stats") == 0){
		//Format: stats
		//Send to stat function
	}
	else if(strcmp(data[0], "unfriend") == 0){
		//Format: unfriend handle1 handle2
		//If valid, send to friend function (specify unfriend)
	}
		//Else do nothing
}

/*
 * The main method
 *
 * @return Whether the code ran successfully or not
 */
int main(void){
	char buffer[BUF_SIZE];
	char *test;
	char **input = (char **)calloc(MAX_COMMANDS, sizeof(char *));
	const char *delim = " ";
	
	t = ht_create(str_hash, str_equals, tablePrint, tableDel);

	while(true){
		printf("amici> ");
		fgets(buffer, BUF_SIZE, stdin);
		for(int i = 0; i < MAX_COMMANDS; i++){
			test = strtok(buffer, delim);
			if(test != NULL){
				input[i] = (char *)realloc(input[i], strlen(test)+1);
				strcpy(input[i], test);
				test = strtok(NULL, delim);
			}
			else break;
		}
		if(strcmp(input[0], "quit") == 0){ //Temporary exit statement
			break;
		}
		parseCommands(input);
	}
	for(int i = 0; i < MAX_COMMANDS; i++){
		free(input[i]);
	}
	free(input);
	ht_destroy(t);
	return 0;
}
