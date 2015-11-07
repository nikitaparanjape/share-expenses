/***************************************************************************** 
*    This file is part of project.
*
*    project is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    project is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with project.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<fcntl.h>
typedef struct data{
	int amt;
	char des[64];
	struct data * n;
}data;

typedef struct list{
	char * stmt;
	struct list * next;
}list;

typedef struct name{
	int num_stmt;
	int num_data;
	struct list * p;
	struct data * q;
}name;
int reset_1stmt(struct name * arruser[16], int user);
int reset_stmt(struct name * arruser[16]);
int delete_member(char * grpname, char * grppass, char * mem[16], char * pass[16], int * num_mem, struct name* arruser[16]);
void print_menu();
int add_member(char * grpname, char* grppass, char * mem[16], char * pass[16], int * num_mem,struct name * arruser[16]);
int delete_grp(char *grpname, char * grppass,char * mem[16], char * pass[16], int * num_mem, struct name* arruser[16]);
int view_my_contri(int user, struct name * arruser[16]);
int create_grp(char *grpname, char *grppass, char *mem[16],char * pass[16], int *num_mem, struct name* arruser[16]);
int check_grp(char * grpname);
void print_line();
void print_home();
int login(char * mem[16], char * pass[16], int * user);
int split_expense(char * mem[16], struct name * arruser[16]);
int add_expense(int user, struct name * arruser[16]);
int get_statement(int user, struct name * arruser[16]);
int reset_data(struct name * arruser[16]);
int findmin(int a[16]);
int findmax(int a[16]);
int load_file(char* grpname, char* password, char* mem[16], int * num_mem, char* pass[16], struct name * arruser[16]);
