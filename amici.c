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

typedef struct person_s{
	char *first_name;		// First name
	char *last_name;		// Last name
	char *handle;			// Username
	struct person_s **friends;	// Dynamic collection of friends
	size_t friend_count;		// Current number of friends
	size_t max_friends;		// Maximum number of friends
} person_t;

/*
 * Clears database and reinitializes storage
 *
 */
void reformat(bool is_quit){
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
void parseCommands(char * data[]){
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
int main(){
	//While true
		//Print "amici> "
		//Read in command
		//Send to parseCommands
	return 0;
}
