
/*****************************************************************************
 * Copyright (C) Nikita Pramod Paranjape nikitaparanjape@gmail.com
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include"project.h"
#include<limits.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
/* this function will open all the user files, and erase all the data stored in them*/
int reset_data(struct name* arruser[16]){
	int i;
	struct data *ptr1, *ptr2;
	for(i = 0; (i < 16) && (arruser[i] != NULL); i++){
		if(arruser[i]->q != NULL){
			ptr1 = arruser[i]->q;
			ptr2 = arruser[i]->q->n;
			
			free(ptr1);
			while(ptr2){
				ptr1 = ptr2;
				ptr2 = ptr2->n;
				
				free(ptr1);
			}
		}
		arruser[i]->q = NULL;
		arruser[i]->num_data = 0;
	}
	return 1;
	
}

/* this is a fnuction to reset only one user's statements..*/
/* this function returns a 1 if successful*/
int reset_1stmt(struct name * arruser[16], int user){
	struct list *ptr1, *ptr2;
	if(arruser[user] != NULL){
		if(arruser[user]->p != NULL){
			ptr1 = arruser[user]->p;
			ptr2 = arruser[user]->p->next;
			free(ptr1->stmt);
			free(ptr1);
			while(ptr2){
				ptr1 = ptr2;
				ptr2 = ptr2->next;
				free(ptr1->stmt);
				free(ptr1);
			}
		}
		arruser[user]->p = NULL;
		arruser[user]->num_stmt = 0;
	}
	return 1;
}
/* this function return sa 1 when it resets the user stmts*/
int reset_stmt(struct name * arruser[16]){
	int i;
	struct list *ptr1, *ptr2;
	for(i = 0; (i < 16) && (arruser[i] != NULL); i++){
		if(arruser[i]->p != NULL){
			ptr1 = arruser[i]->p;
			ptr2 = arruser[i]->p->next;
			free(ptr1->stmt);
			free(ptr1);
			while(ptr2){
				ptr1 = ptr2;
				ptr2 = ptr2->next;
				free(ptr1->stmt);
				free(ptr1);
			}
		}
		arruser[i]->p = NULL;
		arruser[i]->num_stmt = 0;
	}
	return 1;
}
/* this function prints the expenses done after the last expense split*/
/* this function returns a 0 on failure and a 1 on success which the caller must check*/
int get_statement(int user, struct name * arruser[16]){
	printf(" entered get statement\n");
	
	struct data * curr;
	int sum = 0; /* this variable keeps count of the total expenses*/
	printf("Your expense statement is:\n");
	printf("amount\tdescription\n");
	print_line();
	curr = arruser[user]->q;
	if(curr == NULL){
		printf("You dont have any expenses yet!\n");
	}
	else{
		while(curr){
			printf("%d\t%s\n",curr->amt, curr->des);
			sum = sum + curr->amt;
			curr = curr->n;
		}
		print_line();
		printf("Total: %d\n",sum);
		print_line();
	}
	return 1;
}
	
/* this function adds an expense to a existing file*/
/* this function return 0 if file could not be opened and 1 on success*/

int add_expense(int user, struct name * arruser[16] ){
	struct data d;
	int count=0;
	struct data * curr;
	printf("enter amount and description and pres cntrl + d when you are done!\n");
	print_line();
	if(arruser[user]->q == NULL){
		if(scanf("%d %16[^\n]",&d.amt, d.des)!= EOF){// changed here from %[^\n]s
		arruser[user]->q = (struct data *) malloc(sizeof(struct data));
		arruser[user]->q->amt = d.amt;
		strcpy(arruser[user]->q->des, d.des);
		arruser[user]->q->n = NULL;
		arruser[user]->num_data += 1;
		}
		else{
			return 1;
		}
	}
	curr = arruser[user]->q;
	while(curr->n){
		curr = curr->n;
	}/* advance till the last node*/
	
	while(scanf("%d %16[^\n]",&d.amt, d.des) != EOF){// same thing changed as above
		curr->n= (struct data*)malloc(sizeof(struct data));
		if(curr->n == NULL){
			printf("malloc failed\n");
			return 0;
		}
		curr->n->amt = d.amt;
		strcpy(curr->n->des, d.des);
		curr->n->n = NULL;
		curr = curr->n;
		count++;
	}
	arruser[user]->num_data += count;
	return 1;
}
	
	
int split_expense(char * mem[16], struct name * arruser[16]){
	int array_expense[16];
	int i = 0, min, max;
	int sum, count = 0;
	char arr1[64];
	struct data * curr;
	struct list *curr1;
	while(i != 16){
		sum = 0;
		if(arruser[i] == NULL){
			array_expense[i] = INT_MIN;
			i++;
		}
		else{
			curr = arruser[i]->q;
			while(curr){
				sum = sum + curr->amt;
				curr = curr->n;
			}
			array_expense[i] = sum;
			i++;
		}
	}
	
	for(i = 0; i< 16; i++){
		if(array_expense[i] != INT_MIN){
			count++;
			sum = sum + array_expense[i];
		}
	}
	if(count == 0){
		printf(" all accounts are settled\n");
		print_line();
		return 1;
	}
	sum = sum / count;
	for(i = 0; i< 16; i++){
		if(array_expense[i] != INT_MIN){
			array_expense[i] = array_expense[i] - sum;
		}
	} 
	min = findmin(array_expense);
	max = findmax(array_expense);
	while(min != (-1) && max != (-1) && (max!= min)){
		
		printf("%s pays %s an amount of rs %d\n",mem[min], mem[max], (-1) * array_expense[min]);
		if(arruser[min] == NULL){
			arruser[min] = (struct name*)malloc(sizeof(struct name));
			arruser[min]->num_stmt = 0;
			arruser[min]->num_data = 0;
			arruser[min]->p = NULL;
			arruser[min]->q = NULL;
		}
		if(arruser[min]->p == NULL){
			arruser[min]->p = (struct list*) malloc(sizeof(struct list));
			sprintf(arr1, "you owe %s an amount of rs %d", mem[max], (-1)*array_expense[min]);
			arruser[min]->p->stmt = (char*) malloc(64);
			strcpy(arruser[min]->p->stmt, arr1);
			arruser[min]->p->next = NULL;
		}
		else{
			curr1 = arruser[min]->p;
			while(curr1->next){
				curr1 = curr1->next;
			}
			curr1->next = (struct list*) malloc(sizeof(struct list));
			sprintf(arr1, "you owe %s an amount of rs %d", mem[max], (-1)*array_expense[min]);
			curr1->next->stmt = (char *)malloc(64);
			strcpy(curr1->next->stmt, arr1);
			curr1->next->next = NULL;
		}
		
		
		if(arruser[max] == NULL){
			arruser[max] = (struct name*)malloc(sizeof(struct name));
			arruser[max]->num_stmt = 0;
			arruser[max]->num_data = 0;
			arruser[max]->p = NULL;
			arruser[max]->q = NULL;
		}
		if(arruser[max]->p == NULL){
			arruser[max]->p = (struct list*) malloc(sizeof(struct list));
			sprintf(arr1, "%s owes you an amount of rs %d", mem[min], (-1)*array_expense[min]);
			arruser[max]->p->stmt = (char*) malloc(64);
			strcpy(arruser[max]->p->stmt, arr1);
			arruser[max]->p->next = NULL;
		}
		else{
			curr1 = arruser[max]->p;
			while(curr1->next){
				curr1 = curr1->next;
			}
			curr1->next = (struct list*) malloc(sizeof(struct list));
			sprintf(arr1, "%s owes you an amount of rs %d", mem[min], (-1)*array_expense[min]);
			curr1->next->stmt = (char*)malloc(64);
			strcpy(curr1->next->stmt, arr1);
			curr1->next->next = NULL;
		}
		arruser[min]->num_stmt += 1;
		arruser[max]->num_stmt += 1;
		
		array_expense[max] += array_expense[min];
		array_expense[min] = 0;
		min = findmin(array_expense);
		max = findmax(array_expense);
	}
	reset_data(arruser);
	print_line();
	return 1;
}

/* this function find the minimum in an array of integers and returns the index of the array..*/
/* this function will return a -1 if all numbers are 0 or INT_MIN */
int findmin( int a[16]){
	int min;
	int i;
	int min_index = -1;
	min = INT_MAX;
	for(i = 0; i<16; i++){
		if(a[i] == INT_MIN || a[i] == 0){
		;
		}
		else{
			if(a[i] < min){
				min = a[i];
				min_index = i;
			}
		}
	}
	return min_index;
}

/* this function will find the max of integer in the array and then return the index of that num*/
/* this function return the index if successsful otherwise it returns a -1*/
 int findmax( int a[16]){
 	int max;
	int i;
	int max_index = -1;
	max = INT_MIN;
	for(i = 0; i<16; i++){
		if(a[i] == INT_MIN || a[i] == 0){
		;
		}
		else{
			if(a[i] > max){
				max = a[i];
				max_index = i;
			}
		}
	}
	return max_index;
}

/* this function will load data from the file into the variables.. so that calls to the file will remain less during the running*/
/* this function returns 1 on success and the file contains data(group name and its members), 4 if the file opens contains data(groupname and pass only), 2 if file opens but does not contain data and a 0 on failure*/

int load_file(char* grpname, char* grppass, char* mem[16], int * num_mem, char* pass[16], struct name * arruser[16]){
	FILE * fp;
	char * data = "stddata";
	char array1[10];
	char array2[10];
	int i, num1, num2, j;
	
	struct data data1;
	char arr1[64];
	struct data* curr1;
	struct list * curr2;
	fp = fopen(data, "a+");
	if(fp == NULL){/* this means that the file open failed*/
		grpname[0] = '\0';
		grppass[0] = '\0';
		*num_mem = 0;
		for(i = 0; i< 16; i++){
			mem[i] = NULL;
			pass[i] = NULL;
			arruser[i] = NULL;
		}
		return 0;
	}
	else{
	;
	}
	if(fscanf(fp, "%s", grpname) != EOF){
		if(fscanf(fp, "%s", grppass) != EOF){
			if(fscanf(fp, "%d", num_mem) != EOF){
				;
			}
			else{ 
				*num_mem = 0;
				for(i = 0; i< 16; i++){
					mem[i] = NULL;
					pass[i] = NULL;
					arruser[i] = NULL;
				}
				fclose(fp);
				return 4;
			}
		}
		else{ 
			grpname[0] = '\0';
			grppass[0] = '\0';
			*num_mem = 0;
			for(i = 0; i< 16; i++){
				mem[i] = NULL;
				pass[i] = NULL;
				arruser[i] = NULL;
			}
			fclose(fp);
			return 2;
		}
	}
	else{
		grpname[0] = '\0';
		grppass[0] = '\0';
		*num_mem = 0;
		for(i = 0; i< 16; i++){
			mem[i] = NULL;
			pass[i] = NULL;
			arruser[i] = NULL;
		}
		fclose(fp);
		return 2;
	}
	for(i = 0; i < (*num_mem); i++){
		fscanf(fp, "%s", array1);
		fscanf(fp, "%s", array2);
		mem[i] = (char *) malloc(10);
		pass[i] = (char*)malloc(10);
		strcpy(mem[i], array1);
		strcpy(pass[i], array2);
	}
	for( i = (*num_mem);i < 16; i++){
		mem[i] = NULL;
		pass[i] = NULL;
		arruser[i] = NULL;
	}
	fclose(fp);
	for(i = 0; i < (*num_mem); i++){
		fp = fopen(mem[i],"a+");
		if(fp == NULL){
			arruser[i] = (struct name*)malloc(sizeof(struct name));
			arruser[i]->p = NULL;
			arruser[i]->q = NULL;
			arruser[i]->num_data = 0;
			arruser[i]->num_stmt = 0;
		}
		else{
			arruser[i] = (struct name*)malloc(sizeof(struct name));
			if(fscanf(fp, "%d", &(num1)) != EOF){
				arruser[i]->num_data = num1;
				if (num1 != 0){
				arruser[i]->q = (struct data*) malloc(sizeof(struct data));
				fscanf(fp, "%d",&(data1.amt));
				fscanf(fp, "%[^\n]%*[\n]", arr1);
				arruser[i]->q->amt = data1.amt;
				strcpy(arruser[i]->q->des,&arr1[1]);
				arruser[i]->q->n = NULL;
				curr1 = arruser[i]->q;
				
				for(j = 1; j< num1; j++){
					curr1->n = (struct data*) malloc(sizeof(struct data));
					fscanf(fp, "%d",&(data1.amt));
					fscanf(fp, "%[^\n]%*[\n]", arr1);
					curr1->n->amt = data1.amt;
					strcpy(curr1->n->des,&arr1[1]);
					curr1->n->n = NULL;
					curr1 = curr1->n;
				}
				}
				else{
					arruser[i]->q = NULL;
				}
				fscanf(fp, "%d", &num2);
				arruser[i]->num_stmt = num2;
				if(num2 != 0){
				arruser[i]->p = (struct list*) malloc(sizeof(struct list));
				fscanf(fp, "%[^\n]%*[\n]", arr1);
				arruser[i]->p->stmt = (char *)malloc(64);
				strcpy(arruser[i]->p->stmt, &arr1[1]);
				arruser[i]->p->next = NULL;
				curr2 = arruser[i]->p;
				for(j = 1; j < num2; j++){
					curr2->next = (struct list*) malloc(sizeof(struct list));
					fscanf(fp, "%[^\n]%*[\n]", arr1);
					curr2->next->stmt = (char *)malloc(64);
					strcpy(curr2->next->stmt,&arr1[1]);
					curr2->next->next = NULL;
					curr2 = curr2->next;
				}
				}
				else{
					arruser[i]->p = NULL;
				}
			}
			else{
				arruser[i]->num_data = 0;
				arruser[i]->q = NULL;
				arruser[i]->num_stmt = 0;
				arruser[i]->p= NULL;
			}
			
		}
		fclose(fp);
	}
	return 1;
}

/* this function will check that login is correct password and username*/
/* this function may return 0 if the person wants to exit and always gets the correct username and password and returns a 1*/

int login(char * mem[16], char * pass[16], int * user){
	char * buffer1;
	char buffer[10];
	int i, j = -1, flag = 1;
	while(flag == 1){
		printf(" please enter your username\nor enter 1 for exit\n");
		scanf("%s", buffer);
		if(buffer[0] == '1'){
			flag = 0;
			break;
		}
		for(i = 0; i < 16; i++){
			if(mem[i] != NULL){
				if(strcmp(mem[i], buffer) == 0){
					j = i;
					break;
				}
			}
		}
		if(j != (-1)){
			flag = 2;	
		}
		else{
			printf(" this username does not match any in our list\n please try again\n");
			flag = 1;
		}
	}
	if (flag == 0){
		return 0;
	}
	print_line();
	
	while(flag == 2){
		buffer1 = getpass("please enter your password\n");	
		
		if(strcmp(pass[i],buffer1) == 0){
			flag = 1;
			*user = j;
			break;
		}
		else{
			flag = 2;
			printf(" password does not match\n");
		}
	}
	if (flag == 0){
		return 0;
	}
	return 1;
}
			
/* this function prints choice seen on the home screen*/
void print_home(){
	print_line();
	printf(" HELLO,\n what would you like to do?\n");
	printf("1. login\n\n");
	printf("2. create a group\n\n");
	printf("3. delete a group\n\n");
	printf("4. delete a member\n\n");
	printf("5. exit\n\n");
	print_line();
	return;
}

/* this function printf a horizontal line on the screen*/
void print_line(){
	printf("------------------------------------------------------------------\n");
	return;
}		


/* this function check if the given group name is valid*/
/* this function return s a 1 if the grp namei s valid otherwise return s a 0*/
int check_grp(char * grpname){
	if(grpname[0] == '\0'){
		return 0;
	}	
	return 1;
}

/* this function creates anew group */
/* this function returns 1 if successful or return a 0 if unsuccessful and return s 2 if the user wants to discontinue*/
int create_grp(char *grpname, char *grppass, char *mem[16],char * pass[16], int *num_mem, struct name* arruser[16]){
	char buffer[10];
	char * buffer1;
	int flag = 1;
	(*num_mem) = 0;
	printf(" enter your group name\n");
	scanf("%s", buffer);
	strcpy(grpname, buffer);
	buffer1 = getpass(" enter password for the group: ");
	
	strcpy(grppass, buffer1);
	print_line();
	while(flag == 1){
		print_line();
		printf(" enter name of a member and ctrl+d when you are done, or press one if you want to exit\n");
		if(scanf("%s", buffer) == EOF){
			break;
		}
		if(buffer[0] == '1'){
			flag = 0;
			break;
		}
		mem[*num_mem] = (char*)malloc(10);
		strcpy(mem[*num_mem], buffer);
		print_line();
		buffer1 = getpass("enter password for member: ");
		pass[*num_mem] = (char*)malloc(10);
		strcpy(pass[*num_mem],buffer1);
		arruser[*num_mem] = (struct name*)malloc(sizeof(struct name));
		arruser[*num_mem]->num_stmt = 0;
		arruser[*num_mem]->num_data = 0;
		arruser[*num_mem]->q = NULL;
		arruser[*num_mem]->p = NULL;
		(*num_mem)+=1;
		flag = 1;
	}
	if(flag == 0){
		return 0;
	}
	return 1;

}

/* this function will delete the group which is already present*/
/* this function returns the values 1 on success and 0 on failure and 2 if the user wants to discontinue*/
/*remove files of members too*/
int delete_grp(char *grpname, char * grppass,char * mem[16], char * pass[16], int * num_mem, struct name* arruser[16]){
	int j;
	int i;
	printf(" you have chosen to delete the group %s .\n", grpname);
	printf(" please enter 1 to confirm, 0 to discontinue\n");
	scanf("%d",&j);
	if(j == 0){
		return 2;
	}
	for(i = 0; i< (*num_mem); i++){
		 remove(mem[i]);
	}
	grpname[0] = '\0';
	grppass[0] = '\0';
	for(i = 0; i < (*num_mem); i++){
		free(mem[i]);
		free(pass[i]);
		mem[i] = NULL;
		pass[i] = NULL;
	}
	reset_data(arruser);
	reset_stmt(arruser);
	(*num_mem) = 0;
	return 1;
}

/* this function is to view what contribution the user has to recieve or give to someone*/
/* this function returns 1 onsucess and 0 on failure*/
int view_my_contri(int user, struct name* arruser[16]){
	struct list * curr;
	curr = arruser[user]->p;
	printf(" Your contribution is:\n");
	if(curr == NULL){
		printf("no entries!\n");
		return 1;
	}
	while(curr){
		printf("%s\n",curr->stmt);
		curr = curr->next;
	}
	return 1;
}	

/* this function adds a member to the existing member group*/
/* this function returns 1 if successful and a 0 if unsuccessful and a 2 if the user wants to exit*/
int add_member(char* grpname, char * grppass, char * mem[16], char * pass[16], int * num_mem, struct name* arruser[16]){
	int flag = 0;
	char buffer[10];
	char * buffer1;
	printf("please enter groupname\n");
	while(flag == 0){
		scanf("%s", buffer);
		if(buffer[0] == '1'){
			flag = 1;
			break;
		}
		if(strcmp(grpname, buffer) == 0){
			flag = 2;
			break;
		}
		else{
			printf("please re enter groupname or press 1 to exit\n");
			flag = 0;
		}
	}//while closed
	if(flag == 1){
		return 0;
	}
	buffer1 = getpass(" please enter password for group : ");
	while(flag == 2){
		
		if(strcmp(grppass, buffer1) == 0){
			flag = 3;
			break;
		}
		else{
			printf(" incorrect password\n");
			buffer1 = getpass(" please re-enter password for group : ");
			flag = 2;
		}
	}/* while closed*/
	if(flag == 3){
		printf(" add new member's name\n");
		scanf("%s", buffer);
		mem[*num_mem] = (char *) malloc(10);
		strcpy(mem[*num_mem], buffer);
		buffer1 = getpass("add password");
		pass[*num_mem] = (char *) malloc(10);
		strcpy(pass[*num_mem], buffer1);
		arruser[*num_mem] = (struct name *)malloc(sizeof(struct name));
		arruser[*num_mem]->q = NULL;
		arruser[*num_mem]->p = NULL;
		arruser[*num_mem]->num_stmt = 0;
		arruser[*num_mem]->num_data = 0;
		(*num_mem)++;
		return 1;
	}
	if (flag == 0){
		return 0;
	}
	return 0;
			
}

/* this function prints the menu after you have logged in*/
void print_menu(){
	print_line();
	printf(" please enter you choice\n");
	printf("1. add an expense to your account\n");
	printf("2. get statement\n");
	printf("3. split all your expenses equally\n");
	printf("4. view your contri\n");
	printf("5. add a member to your group\n");
	printf("6. clear my contri\n");
	printf("7. logout\n");
	printf("8. exit\n");
	print_line();
	return;
}

/* this function enables you to delete some member when you are not logged it( so that you cannot delete yourself and then try to acces you rown data*/
/* this function returns 1 on success, 2 if user wants to discontinue and 0 if the function fails*/
int delete_member(char * grpname, char * grppass, char * mem[16], char * pass[16], int * num_mem, struct name * arruser[16]){
	int flag = 0;
	char buffer[10];
	int j = -1, i;
	char * buffer1;
	printf("please enter groupname\n");
	while(flag == 0){
		scanf("%s", buffer);
		if(buffer[0] == '1'){
			flag = 1;
			break;
		}
		if(strcmp(grpname, buffer) == 0){
			flag = 2;
			break;
		}
		else{
			printf("please re enter groupname or press 1 to exit\n");
			flag = 0;
		}
	}/*while closed*/
	if(flag == 1){
		return 0;
	}
	
	while(flag == 2){
		buffer1 = getpass(" enter password for the group ");
		if(strcmp(grppass, buffer1) == 0){
			flag = 3;
			break;
		}
		else{
			printf(" incorrect password\n");
			flag = 2;
		}
	}/* while closed*/
	if(flag == 1){
		return 0;
	}
	printf(" enter the username of the member you want to delete from the group\n");
	while(flag == 3){
		scanf("%s", buffer);
		if(buffer[0] == '1'){
			flag = 1;
			break;
		}
		for(i = 0; i< (*num_mem); i++){
			if(strcmp(buffer, mem[i]) == 0){
				flag = 4;
				remove(mem[i]);
				arruser[i]->num_data = 0;
				arruser[i]->num_stmt = 0;
				j = i;
				break;
			}
		}
		if(j == (-1)){
			printf(" no such member exists\n");
			printf("enter once again or press1 for exit\n");
			flag = 3;
		}
	}
	if(flag == 1){
		return 0;
	}
	free(mem[j]);
	free(pass[j]);
	free(arruser[j]);
	for(i = j+1; i < (*num_mem); i++){
		mem[i-1] = mem[i];
		pass[i-1] = pass[i];
		arruser[i-1] = arruser[i];
	}
	mem[*num_mem] = NULL;
	pass[*num_mem] = NULL;
	arruser[*num_mem] = NULL;
	(*num_mem) -= 1;
	return 1;
}	
	
	
int main(){
/* this function is the actual main function in my program*/
	struct name * arruser[16];
	int num_mem;/*this is the total number of members in the group*/
	char * mem[16];/* this the array which stores addresses of usernames*/
	char * pass[16];/* this array stores addresses of respective passwords*/
	char grpname[10];/* char array containing the group name*/
	char grppass[10]; /* this array contains the password of the group*/
	int user;/* this is the current user that has logged in( users index in the array)*/
	int i, j, k;/* these are some random variables to be used as counters.*/
	int state = 1;/* this acs like flag between two different switch cases which go on in an infinite loop wher only few condition can break out of it depending on the value of this flag.*/
	int choice; /*this variable selects the case in switch*/
	struct data * curr1;
	struct list * curr2;
	char buffer[10];
	char * buffer1;
	/*now to start calling functions one by one:*/
	printf("Loading...\n");
	print_line();
	j = load_file(grpname, grppass, mem, &num_mem, pass, arruser);
	if(j == 0){
	 printf(" the standard file could not be opened\n you can still go ahead with working but previous data is lost now\n");
	 }
	 if(j == 2){
	 printf(" Looks like you are a new user!\n welcome to splitwise!\n");
	 }
	 if(j == 1){
	 printf("All previous user data is now loaded!\n welcome back to splitwise!\n");
	 }
	 if(j == 4){
	 	printf(" welcome back to splitwise!\n you already have a group created\n but there are no members in this group\n please add yourself to the group first\n");
	 	printf(" please enter your username\n");
	 	scanf(" %s", buffer);
	 	mem[0] = (char * ) malloc(10);
	 	if(mem[0] == NULL){
	 		printf(" not enough memory\n");
	 		exit(1);
	 	}
	 	strcpy(mem[0], buffer);
	 	buffer1 = getpass(" hello, please enter you password\n");
	 	pass[0] = (char*)malloc(10);
	 	if(pass[0] == NULL){
	 		printf(" not enough memory\n");
	 		exit(1);
	 	}
	 	strcpy(pass[0], buffer1);
	 	printf(" you are now added to the group %s \n welcome!\n", grpname);
	 	arruser[0] = (struct name*)malloc(sizeof(struct name));
	 	arruser[0]->q = NULL;
	 	arruser[0]->p = NULL;
	 	arruser[0]->num_data = 0;
	 	arruser[0]->num_stmt = 0;
	 }
	 print_line();
	 print_home();
	 scanf("%d", &choice);
	 while(1){
	 	
	 	if(state == 1){
	 		switch(choice){
	 			case 1: 
	 				if(num_mem == 0){
	 					printf(" there aren't any group members\n");
	 					create_grp(grpname, grppass,mem, pass, &num_mem,arruser); 
	 					print_home();
	 					scanf("%d", &choice);
	 					print_line();
	 					state = 1;
	 					/* check the return values*/
	 					break;
	 				}
	 				j = login(mem, pass, &user);
	 				print_line();
	 				if(j == 0){
	 					state = 3;
	 					break;
	 				}	
	 				if(j == 1){
	 					print_menu();
	 					scanf("%d", &choice);
	 					print_line();
	 					state = 2;
	 				}
	 				break;
	 			case 2: 
					j = check_grp(grpname);
					if(j == 0){
						/* no group exists*/
						k = create_grp(grpname, grppass, mem, pass, &num_mem, arruser);
						if(k == 0){
							printf(" creating group failed\n");
							state = 3;
						}
						if(k == 1){
							print_home();
	 						scanf("%d", &choice);
							print_line();
							state = 1;
						}
					}
					else{
						printf("A group alredy exists. you cannot create multiple groups. if youi want to create a new group the current group will have to be deleted.\n press 1 to delete existing group\n press 2 to exit\n");
						scanf("%d", &k);
						print_line();
						if(k == 1){
							delete_grp(grpname, grppass, mem, pass, &num_mem, arruser);
							i = create_grp(grpname, grppass, mem, pass, &num_mem,arruser);
							if(i == 0){
							printf(" creating group failed\n");
							state = 3;
							}
							if(i == 1){
								print_home();
	 							scanf("%d", &choice);
								print_line();
								state = 1;
							}
						}
						if(k == 2){
						print_home();
						scanf("%d", &choice);
							print_line();
							state  = 1;
						}
					}
					break;
				
				case 3:
					j = check_grp(grpname);
					if(j == 0){
					;
					}
					else{
						delete_grp(grpname, grppass, mem, pass, &num_mem, arruser);
					}
					print_home();
					scanf("%d", &choice);
					print_line();
					state = 1;
					break;
				
				case 4: delete_member(grpname, grppass, mem, pass, &num_mem, arruser);
					print_home();
					scanf("%d", &choice);
					print_line();
					state = 1;
					break;
				case 5:
					state = 3;
					break;
					
				default:
						printf(" you have entered an invalid choice\n please enter again\n");
						print_home();
						scanf("%d", &choice);
						print_line();
						state = 1;
					break;
			}/* switch closed*/
		}/* if closed*/
		if( state == 2){
			switch(choice){
				case 1:
					j = add_expense(user, arruser);
					if(j == 0){
						printf(" sorry could not add any more expenses\n");
						print_menu();
	 					scanf("%d", &choice);
						print_line();
						state  = 2;
					}
					if(j == 1){
						print_menu();
	 					scanf("%d", &choice);
						print_line();
						state = 2;
					}
				break;
				 case 2:
				 	j = get_statement(user, arruser);
				 	if(j == 0){
				 		printf(" sorry, the file could no be opened\n");
				 	}
				 	print_menu();
	 				scanf("%d", &choice);
				 	print_line();
				 	state = 2;
				 	break;
				 	
				 case 3:
				 	j = split_expense(mem, arruser);
				 	print_menu();
	 				scanf("%d", &choice);
				 	print_line();
				 	state = 2;
				 	break;
				 
				 case 4:
				 	view_my_contri(user, arruser);
				 	print_menu();
	 				scanf("%d", &choice);
				 	print_line();
				 	state = 2;
				 	break;
				 	
				 case 5:
				 	j = split_expense(mem, arruser);
				 	if( j == 0){
						print_menu();
	 					scanf("%d", &choice);
						print_line();
				 		/* unsucessful*/
				 		state = 2;
				 		break;
				 	}
				 	j = add_member(grpname, grppass, mem, pass, &num_mem, arruser);
				 	if (j == 0){
				 	/* unsuccessful*/
				 		printf(" adding member was unsuccessful\n please try again\n");
				 		print_menu();
	 					scanf("%d", &choice);
				 		print_line();
				 		state = 2;
				 		break;
				 	}
				 	print_menu();
	 				scanf("%d", &choice);
				 	print_line();
				 	state = 2;
				 	break;
				 
				 case 6:
					 reset_1stmt(arruser, user);
					 printf("successful in clearing you statements!\n");
					 print_menu();
			       	 	 scanf("%d",&choice);	
			       	 	print_line();
			       	 	 state = 2;
				 	break;
				 	
				 case 7:
				 	user = -1;
				 	print_home();
				 	scanf("%d", &choice);
				 	print_line();
				 	state = 1;
				 	break;
				 
				 case 8 :
				 	state = 3;
				 	break;
				 	
				 default : printf(" you have entered a wrong choice\n please enter again\n");
				 print_menu();
	 			scanf("%d", &choice);
				print_line();
				 state = 2;
				 break;
			 }/* switch closed*/
		}/* if closed*/
		if(state == 3){
			FILE * fp;
			char data[] = "stddata";
			fp = fopen(data, "w");
			if(fp == NULL){
				printf(" file cannot be opened to store all the data you create in this session.\n sorry\n");
				return 0;
			}
			fprintf(fp, "%s\n", grpname);
			fprintf(fp, "%s\n",grppass);
			fprintf(fp, "%d\n", num_mem);
			for(i = 0; i< num_mem; i++){
				if(mem[i] != NULL){
					fprintf(fp, "%s\n", mem[i]);
					fprintf(fp, "%s\n", pass[i]);
					
				}/* if closed*/
			}
			fclose(fp);
			for(i = 0; i< num_mem; i++){
				fp = fopen(mem[i], "w+");
				fprintf(fp, "%d\n", arruser[i]->num_data);
				curr1 = arruser[i]->q;
				while(curr1){
					fprintf(fp, "%d ", curr1->amt);
					fprintf(fp, "%s\n", curr1->des);
					curr1 = curr1->n;
				}
				fprintf(fp, "%d ", arruser[i]->num_stmt);
				curr2 = arruser[i]->p;
				while(curr2){
					fprintf(fp, "%s\n", curr2->stmt);
					curr2 = curr2->next;
				}
				fclose(fp);
			}
			reset_data(arruser);
			reset_stmt(arruser);	
			for(i = 0; i < num_mem; i++){
				free(mem[i]);
				free(pass[i]);
				
			}
			break;
		}
	}/* while closed*/
	return 0;
}/* main closed*/		

