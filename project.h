#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<fcntl.h>
typedef struct data{
	int amt;
	char des[64];/* should i add a time part in the struct?*/
}data;
char * array_users[16];

void print_home();
int login(char * grpname, char * mem[16], char * pass[16], int * user);
int split_expense(char * a[16]);/*tested*/
int printcontri(char * fname);
int add_expense(char * fname);// check if you can add many expnses at once so that file does not have to be opened again and again.. done..checkto see if working...
int get_statement(char * fname);
int reset(char* a[16]);/*tested*/
int findmin(int a[16]);/* tested */
int findmax(int a[16]);/*tested*/
int load_file(char* grpname, char* password, char* mem[16], int * num_mem, char* pass[16]);
/*many other functions to be included....*/
