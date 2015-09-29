#include"project.h"
#include<limits.h>
/* this function prints the expenses done after the last expense split*/
/* this function returns a 0 on failure and a 1 on success which the caller must check*/
int get_statement(char * fname){
	struct data d;
	int sum = 0; /* this variable keeps count of the total expenses*/
	int fd;
	fd = open(fname, O_RDONLY);
	if(fd == -1){
		return 0;
	}
//here file has been successfully opened
	printf(" expense statement of %s",fname);
	printf("\n");
	printf("description amount\n");
	while(read(fd, &d, sizeof(struct data)) != 0){
		sum = sum + d.amt;
		printf("%d\t%s",d.amt, d.des);
	}
	printf("\n your total expenses are %d",sum);
	close(fd);
	return 1;
}

/* this function adds an expense to a existing file*/
/* this function return 0 if file could not be opened an d 1 on success*/

int add_expense(char * fname){
	struct data d;
	int fd = open(fname, O_WRONLY);
	if(fd == -1){
		return 0;
	}
	/* now the file has been opened*/
	printf("enter amount");
	scanf("%d",&d.amt);
	printf("enter a description");
	scanf("%s",d.des);
	write(fd, &d, sizeof(struct data));
	close(fd);
	return 1;
}


int split_expense(char * a[16]){
	int array_expense[16];
	int i = 0;
	int sum;
	int total_expenses = 0;
	struct data d;
	char * name[16];
	int contribution;
	while(i != 16){
		name[i] = a[i];
		sum = 0;
		if(a[i] != NULL) {
			int fd = open(a[i], O_RDONLY);
			while(read(fd, &d, sizeof(struct data)) != 0){
				sum  = sum + d.amt;
			}
			array_expense[i] = sum;
			i++;
		}
