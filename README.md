# Amici-School-Project
The code from a 2nd year school project where we had to write a rudimentary social media program in C

This program was written for a project in the class CSCI-243: Mechanics of Programming on October 7th, 2021. In this project, 
I was tasked with creating a fictional social media network called "AMICI", which would use a hash table to
store users along with all of their information. It uses a simplistic text-based interface, and does not store any information. Aside from two header files "table.h" and "hash.h" provided by the school, all files were written by me.

The commands to use this code are listed below, as they were written in the rubric of the project:

**add [first-name] [last-name] [handle]**
- Add the specified user having the indicated first and last names to the database with the specified handle. Handles must be unique; names, however, may be duplicated (e.g., there might be 5,000 "John Smith" users in the system, 
	but each would have a unique handle).
	
**friend [handle1] [handle2]**
- Create a friendship between the two users identified by the indicated handles. The handles must both exist, must be different (i.e., a user can't be their own "friend"), and there must not already be a friendship between these users.

**init**
- Delete the current collection of people and friendships in the network, returning it to an empty state.

**print [handle]**
- Find the entry for the specified user, and print the user's name and handle, followed by a list of the user's current friendships. 
	The specified handle must be in the system.
	
**quit**
- Delete the current collection of people and friendships in the network, and exit from the program.

**size [handle]**
- Count the number of existing friendships for the specified user, and report that. The specified handle must be in the system.

**stats**
- Report on the current contents of the network by printing the number of users in the system and 
	the number of unique friendships (i.e., if handles alpha42 and beta991 are friends, that should be counted as one friendship 
	even though each one appears in the other's list of friendships).
	
**unfriend [handle1] [handle2]**
- Dissolve the friendship that exists between the specified users. The two handles must exist, and there must be a friendship between the users.
