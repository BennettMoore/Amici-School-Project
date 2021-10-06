/*
 * file: amici.c
 *
 * A new social network that can add and remove users, and manage friendships between users
 *
 * @author Bennett Moore bwm7637@rit.edu
 */

#define _POSIX_C_SOURCE 200809L //Allows strdup to work

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hash.h"
#include "table.h"

#define BUF_SIZE 1024
#define MAX_COMMANDS 4
#define FRIEND_BLOCK 10

typedef struct person_s{
	char *first_name;		// First name
	char *last_name;		// Last name
	char *handle;			// Username
	struct person_s **friends;	// Dynamic collection of friends
	size_t friend_count;		// Current number of friends
	size_t max_friends;		// Maximum number of friends
} person_t;

//Global Variables
Table t;
int friendships;
int people;
bool is_active;


/*
 * Print function to initialize table
 *
 * @param key The key of the table entry
 * @param value The value of the table entry
 * @pre value is of type person_t
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
 * @pre value is of type person_t
 */
void tableDel(void* key, void* value){
	(void)key;
	person_t* p1 = (person_t*)value;

	//Free allocated memory
	free(p1->first_name);
	free(p1->last_name);
	free(p1->handle);
	free(p1->friends);
	free(p1);
}

/*
 * Clears database and optionally reinitializes storage
 * 
 * @param is_quit whether the user is quitting or not
 *
 */
void reformat(bool is_quit){
	if(is_quit){
		is_active = false;
		return; //Let main clean the table
	}
	else{ 
		ht_destroy(t);
		t = ht_create(str_hash, str_equals, tablePrint, tableDel);
		return; //reinitializes t to be empty
	}
}

/*
 * Prints overall stats of the site
 *
 */
void printStats(){
	if(people == 1){ //If there's only 1 person, its impossible to have any friends
		printf("Statistics: 1 person, 0 friendships\n");
	}
	else if(friendships == 1){ //If there's 1 friendship, its impossible to have only 1 person
		printf("Statistics: %i people, 1 friendship\n", people);
	}
	else{
		printf("Statistics: %i people, %i friendships\n", people, friendships);
	}
}

/*
 * Prints info about a user
 *
 * @param handle The handle of a user
 * @pre handle is a valid user
 *
 */
void printInfo(char * handle){
	person_t* p1 = (person_t*)ht_get(t, (void*)handle);

	if(p1->friend_count == 0){	//User has no friends
		printf("User %s %s(%s) has no friends\n",p1->first_name, p1->last_name, p1->handle);
	}
	else if(p1->friend_count == 1){	//User has 1 friend
		printf("User %s %s(%s) has 1 friend\n", p1->first_name, p1->last_name, p1->handle);
		printf("\t%s %s(%s)\n", p1->friends[0]->first_name, p1->friends[0]->last_name, p1->friends[0]->handle); //List friend
	}
	else{				//User has 2+ friends
		printf("User %s %s (%s) has %i friends\n", p1->first_name, p1->last_name, p1->handle,(int)p1->friend_count);
		for(size_t i = 0; i < p1->friend_count; i++){ //List all friends
			printf("\t%s %s(%s)\n", p1->friends[i]->first_name, p1->friends[i]->last_name, p1->friends[i]->handle);
		}
	}
}

/*
 * Adds or removes two users to/from each others' friend networks
 *
 * @param handles[] The handles of the two users
 * @pre handles[] has two entries
 *
 */
void friend(char * handles[], bool is_friendly){
	person_t* f1 = (person_t*)ht_get(t, (void*)handles[0]);
	person_t* f2 = (person_t*)ht_get(t, (void*)handles[1]);
	
	if(f1->friend_count == 0 || f2->friend_count == 0){ //If either person has no friends, there's no way they can be friends already
		if(!is_friendly){ 			//Remove friend
			fprintf(stderr, "error: %s is not friends with %s\n", f1->handle, f2->handle);
			return;
		}
		//else skip to the end
	}
	else{ //If both people have at least 1 friend
		for(size_t i = 0; i < f1->friend_count; i++){ //If f1 is friends with f2
			if(f1->friends[i] == f2){
				if(is_friendly){	//Add friend
					fprintf(stderr, "error: %s is already friends with %s\n", f1->handle, f2->handle);
					return;
				}
				else{			//Remove friend
					for(size_t j = i; j < f1->friend_count + 1; j++){
						f1->friends[j] = f1->friends[j+1]; 
					}

					//Tallying friend removal
					f1->friend_count--;
					if((f1->friend_count + FRIEND_BLOCK) < f1->max_friends){ //Check for an unused block of friend slots
						f1->max_friends -= FRIEND_BLOCK;
						f1->friends = (person_t**)realloc(f1->friends, f1->max_friends*sizeof(person_t*));
					}
					break;
				}
			}
		}
		for(size_t i = 0; i < f2->friend_count; i++){ //If f2 is friends with f1
			if(f2->friends[i] == f1){
				if(is_friendly){	//Add friend
					f1->friends[f1->friend_count] = f2; //This will only trigger if there is an assymetrical friendship
					fprintf(stderr, "error: %s is already friends with %s\n", f2->handle, f1->handle);
					return;
				}
				else{			//Remove friend
					for(size_t j = i; j < f2->friend_count + 1; j++){
						f2->friends[j] = f2->friends[j+1];
					}

					//Tallying friend removal
					f2->friend_count--;
					friendships--;
					if((f2->friend_count + FRIEND_BLOCK) < f2->max_friends){ //Check for an unused block of friend slots
						f2->max_friends -= FRIEND_BLOCK;
						f2->friends = (person_t**)realloc(f2->friends, f2->max_friends*sizeof(person_t*));
					}
					return;
				}
			}
		}
	}
	if(is_friendly){			//Add friend
		//Check if either user is at their maximum friends
		if(f1->friend_count >= f1->max_friends-1){
			f1->max_friends += FRIEND_BLOCK;
			f1->friends = (person_t**)realloc(f1->friends, f1->max_friends*sizeof(person_t*));
		}
		if(f2->friend_count >= f2->max_friends-1){
			f2->max_friends += FRIEND_BLOCK;
			f2->friends = (person_t**)realloc(f2->friends, f2->max_friends*sizeof(person_t*));
		}

		//Adding new friend
		f1->friends[f1->friend_count] = f2;
		f2->friends[f2->friend_count] = f1;
		
		//Tallying friendships
		f1->friend_count++;
		f2->friend_count++;
		friendships++;
	}
	else{					//Remove friend
		fprintf(stderr, "error: %s is not friends with %s\n", f1->handle, f2->handle);
		return;
	}	
}

/*
 * Determines what command the user is attempting to perform, and run the function prescribed to it
 *
 * @param data An array of string commands
 * @pre Commands are separated into tokens
 *
 */
void parseCommands(char ** data){
	if(strcmp(data[0], "add") == 0){								//Add new user
		if((data[1] != NULL && data[2] != NULL && data[3] != NULL)){ //Validate command length
			//Copy data values
			char *f_name = strdup(data[1]);
			char *l_name = strdup(data[2]);
			char *handle = strdup(data[3]);
		
			//Validate command syntax
			if(handle[strlen(handle)-1] == '\n'){ //Remove newline character
				handle[strlen(handle)-1] = '\0';
			}
			if(strlen(handle) < 1){ //Check for empty handles
				fprintf(stderr, "error: add command usage: first-name last-name handle\n");
			}
			else if(!ht_has(t, (void*)handle)){ //Make sure handle is available
		
				//Person is initialized
				person_t* new_person = calloc(1, sizeof(person_t));
				new_person->first_name = strdup(f_name);
				new_person->last_name = strdup(l_name);
				new_person->handle = strdup(handle);
				new_person->friends = (person_t**)calloc(FRIEND_BLOCK, sizeof(person_t*));
				new_person->friend_count = 0;
				new_person->max_friends = FRIEND_BLOCK;
				
				//Put new person in table and add to people count
				ht_put(t, (void*)new_person->handle, (void*)new_person);
				people++;
			}
			else{ //Handle is taken
				fprintf(stderr, "error: The handle '%s' is taken by another user\n", data[3]);
			}
		
			//Free allocated memory
			free(f_name);
			free(l_name);
			free(handle);
		}
		else{ //Invalid command structure
			fprintf(stderr, "error: add command usage: first-name last-name handle\n");
		}
	}
	else if(strcmp(data[0], "friend") == 0){							//Make two users friends
		if(data[1] != NULL && data[2] != NULL){ //Validate command length
		
			//Validate command syntax
			if(data[2][strlen(data[2])-1] == '\n'){ //Remove newline character
				data[2][strlen(data[2])-1] = '\0';
			}
			if(strlen(data[1]) < 1 || strlen(data[2]) < 1){ //Check for empty handles
				fprintf(stderr, "error: friend command usage: friend handle1 handle2\n");
			}
			else if(ht_has(t, (void*)data[1]) && ht_has(t, (void*)data[2])){ //Check whether both users exist
				char * handles[] = {data[1], data[2]};
				//Add friendship
				friend(handles, true);
			}
			else{ //At least one handle could not be found
				fprintf(stderr, "error: one or more users not found\n");
			}
		}
		else{ //Invalid command structure
			fprintf(stderr, "error: friend command usage: friend handle1 handle2\n");
		}
	}
	else if(strcmp(data[0], "init") == 0 || strcmp(data[0], "init\n") == 0){			//Clear table and reset it
		reformat(false);
	}
	else if(strcmp(data[0], "print") == 0){								//Print data on a specific user
		if(data[1] != NULL){ //Validate command length
		
			//Validate command syntax
			if(data[1][strlen(data[1])-1] == '\n'){ //Remove newline character
				data[1][strlen(data[1])-1] = '\0';
			}
			if(ht_has(t, (void*)data[1])){ //Does user exist
				printInfo(data[1]);
			}
			else{ //Handle could not be found
				fprintf(stderr, "error: '%s' is not a valid user\n", data[1]);
			}
		}
		else{ //Invalid command structure
			fprintf(stderr, "error: print command usage: print handle\n");
		}
	}
	else if(strcmp(data[0], "quit") == 0 || strcmp(data[0], "quit\n") == 0){			//Clear table and exit program
		reformat(true);
	}
	else if(strcmp(data[0], "size") == 0){								//Print number of friends a user has
		if(data[1] != NULL){ //Validate command length
		
			//Validate command syntax
			if(data[1][strlen(data[1])-1] == '\n'){ //Remove newline character
				data[1][strlen(data[1])-1] = '\0';
			}
			if(ht_has(t, (void*)data[1])){ //Does user exist
				person_t* temp = (person_t*)ht_get(t, (void*)data[1]);
				if(temp->friend_count == 0){
					printf("User %s %s('%s') has no friends\n", temp->first_name, temp->last_name, temp->handle);
				}
				else if(temp->friend_count == 1){
					printf("User %s %s('%s') has 1 friend\n", temp->first_name, temp->last_name, temp->handle);
				}
				else{
					printf("User %s %s('%s') has %i friends\n", temp->first_name, temp->last_name, temp->handle, (int)temp->friend_count);
				}
 			}
			else{ //Handle could not be found
				fprintf(stderr, "error: '%s' is not a valid user\n", data[1]);
			}
		}
		else{ //Invalid command structure
			fprintf(stderr, "error: print command usage: print handle\n");
		}
	}
	else if(strcmp(data[0], "stats") == 0 || strcmp(data[0], "stats\n") == 0){			//Print cumulative user data
		printStats();
	}
	else if(strcmp(data[0], "unfriend") == 0){							//Make two users not friends
		if(data[1] != NULL && data[2] != NULL){ //Validate command length
		
			//Validate command syntax
			if(data[2][strlen(data[2])-1] == '\n'){ //Remove newline character
				data[2][strlen(data[2])-1] = '\0';
			}
			if(strlen(data[1]) < 1 || strlen(data[2]) < 1){ //Check for empty handles
				fprintf(stderr, "error: unfriend command usage: unfriend handle1 handle2\n");
			}
			else if(ht_has(t, (void*)data[1]) && ht_has(t, (void*)data[2])){ //Check whether both users exist
				char * handles[] = {data[1], data[2]};
				//Remove friendship
				friend(handles, false);
			}
			else{ //At least one handle could not be found
 				fprintf(stderr, "error: users not found\n");
 			}
		}
		else{ //Invalid command structure
			fprintf(stderr, "error: unfriend command usage: unfriend handle1 handle2\n");
		}
	}
	//Ignore any unrecognizable commands
}

/*
 * The main method
 *
 * @return Whether the code ran successfully or not
 */
int main(void){

	//Initialize varaibles
	char buffer[BUF_SIZE];
	char *test;
	char **input = (char **)calloc(MAX_COMMANDS, sizeof(char *));
	const char *delim = " ";
	is_active = true;
	
	//Create table
	t = ht_create(str_hash, str_equals, tablePrint, tableDel);

	while(is_active){ //Main loop
		printf("amici> ");
		fgets(buffer, BUF_SIZE, stdin);

		//Isolate tokens from command line
		test = strtok(buffer, delim);
		for(int i = 0; i < MAX_COMMANDS; i++){
			if(test != NULL){
				input[i] = (char *)realloc(input[i], strlen(test)+1);
				strcpy(input[i], test);
				test = strtok(NULL, delim);
			}
			else break;
		}

		//Parse commands
		parseCommands(input);
	}

	//Free allocated memory and quit
	for(int i = 0; i < MAX_COMMANDS; i++){
		free(input[i]);
	}
	free(input);
	ht_destroy(t);
	return 0;
}
