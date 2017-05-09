# LinkedListWithPthreads
This is the code developed for Lab 2 of Concurrent Programming module.
## How to compile the program?
`gcc -g -Wall -o <output_file_name> <c_file_name>.c -lpthread -lm`<br />

Example: `gcc -g -Wall -o parallel_linked_list_1 parallel_linked_list_1.c -lpthread -lm`
## How to run the program?
`./<output_file_name> <n_threads> <n> <m> <mMember> <mInsert> <mDelete>`
 - n	is the number of initial unique values in the linked list
 - m	is the number of random Member, Insert, and Delete operations performed on the linked list
 - mMember	is the fraction of Member operations
 - mInsert	is the fraction of Insert operations
 - mDelete	is the fraction of Delete operations<br />
 
Example: `./parallel_linked_list_1 1 1000 10000 0.99 0.005 0.005`
