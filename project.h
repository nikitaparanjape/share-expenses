#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<fcntl.h>
typedef struct data{
	int amt;
	char des[64];
}data;
char * array_users[16];

int split_expense(char * a[16]);
int printcontri(char * fname);
int add_expense(char * fname);
int get_statement(char * fname);
int reset(char* a[16]);
int findmin(int a[16]);
int findmax(int a[16]);
/*many other functions to be included....*/
