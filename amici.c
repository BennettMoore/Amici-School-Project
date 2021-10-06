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
 * Clears database and reinitializes storage
 * 
 * @param is_quit whether the user is quitting or not
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
		if(is_friendly){			//Add friend
			//Adding new friend
			f1->friends[f1->friend_count] = f2;
			f2->friends[f2->friend_count] = f1;

			//Tallying friendships
			f1->friend_count++;
			f2->friend_count++;
			friendships++;
		}
		else{ //Error check for...		//Remove friend
			fprintf(stderr, "error: %s is not friends with %s\n", f1->handle, f2->handle);
			return;
		}
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
					f1->friend_count--; //Tallying friend removal
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
					f2->friend_count--; //Tallying friend removal
					friendships--;
					break;
				}

			}
		}
		if(is_friendly){		//Add friend
			//Adding new friend
			f1->friends[f1->friend_count] = f2;
			f2->friends[f2->friend_count] = f1;
			
			//Tallying friendships
			f1->friend_count++;
			f2->friend_count++;
			friendships++;
		}
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
	if(strcmp(data[0], "add") == 0){		//Add new user
		//Validate command
		if((data[1] != NULL && data[2] != NULL && data[3] != NULL)){
			//Copy data values
			char *f_name = strdup(data[1]);
			char *l_name = strdup(data[2]);
			char *handle = strdup(data[3]);
			//Remove newlien character
			if(handle[strlen(handle)-1] == '\n'){
				handle[strlen(handle)-1] = '\0';
			}
			if(!ht_has(t, (void*)handle)){ //Make sure handle is available
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
	else if(strcmp(data[0], "friend") == 0){		//Make two users friends
		//Validate command
		if(data[1] != NULL && data[2] != NULL){
			//Remove newline
			if(data[2][strlen(data[2])-1] == '\n'){
				data[2][strlen(data[2])-1] = '\0';
			}
			//Check whether both users exist
			if(ht_has(t, (void*)data[1]) && ht_has(t, (void*)data[2])){
				char * handles[] = {data[1], data[2]};
				//Add friendship
				friend(handles, true);
			}
			else{ //At least one handle could not be found
				fprintf(stderr, "error: users not found\n");
			}
		}
		else{ //Invalid command structure
			fprintf(stderr, "error: friend command usage: friend handle1 handle2\n");
		}
	}
	else if(strcmp(data[0], "init") == 0){			//Clear table and reset it
		//Format: init
		//Send to reformat function (specify not quitting)
	}
	else if(strcmp(data[0], "print") == 0){			//Print data on a specific user
		//Validate command
		if(data[1] != NULL){
			//Remove newline character
			if(data[1][strlen(data[1])-1] == '\n'){
				data[1][strlen(data[1])-1] = '\0';
			}
			if(ht_has(t, (void*)data[1])){ //Does user exist
				printInfo(data[1]);
				//ht_dump(t, true);	//Useful for error checking
			}
			else{ //Handle could not be found
				fprintf(stderr, "error: '%s' is not a valid user\n", data[1]);
			}
		}
		else{ //Invalid command structure
			fprintf(stderr, "error: print command usage: print handle\n");
		}
	}
	else if(strcmp(data[0], "quit") == 0){			//Clear table and exit program
		//Format: quit
		//Send to reformat function, then exit program (specify quitting)
	}
	else if(strcmp(data[0], "size") == 0){			//Print number of friends a user has
		//Format: size handle
		//If valid, print friend_count of user
	}
	else if(strcmp(data[0], "stats") == 0){			//Print cumulative user data
		//Format: stats
		//Send to stat function
	}
	else if(strcmp(data[0], "unfriend") == 0){		//Make two users not friends
		///Validate command
                if(data[1] != NULL && data[2] != NULL){
                        //Remove newline
                        if(data[2][strlen(data[2])-1] == '\n'){
                                data[2][strlen(data[2])-1] = '\0';
                        }
                        //Check whether both users exist
                        if(ht_has(t, (void*)data[1]) && ht_has(t, (void*)data[2])){
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
	else{ 							//Catch any unrecognizable commands
		fputs("error: not a valid command", stderr);
	}
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
		test = strtok(buffer, delim);
		for(int i = 0; i < MAX_COMMANDS; i++){
			if(test != NULL){
				input[i] = (char *)realloc(input[i], strlen(test)+1);
				strcpy(input[i], test);
				test = strtok(NULL, delim);
			}
			else break;
		}
		if(strcmp(input[0], "quit\n") == 0){ //Temporary exit statement
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
