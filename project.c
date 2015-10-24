#include"project.h"
#include<limits.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
/* this function will open all the user files, and erase all the data stored in them*/
int reset(char* a[16]){
	FILE * fd;	
	int i = 0;
	while(a[i] != NULL && i < 16){
		fd = fopen(a[i], "w");
		if(fd != NULL){
			i++;
			fclose(fd);
		}
		else{
			printf("file open nahi hote\n");
			i++;
		}
	}
	return 0;
}
/* this function prints the expenses done after the last expense split*/
/* this function returns a 0 on failure and a 1 on success which the caller must check*/
int get_statement(char * fname){
	printf(" entered get staement\n");
	struct data d;
	int sum = 0; /* this variable keeps count of the total expenses*/
	int fd;
	fd = open(fname, O_RDONLY);
	if(fd == -1){
		printf(" open failed\n");
		return 0;
	}
//here file has been successfully opened
	printf(" expense statement of %s",fname);
	printf("\n");
	printf("description amount\n");
	while(read(fd, &d, sizeof(struct data)) != 0){
		sum = sum + d.amt;
		printf("%d\t%s\n",d.amt, d.des);
	}
	printf("\n your total expenses are %d",sum);
	close(fd);
	return 1;
}

/* this function adds an expense to a existing file*/
/* this function return 0 if file could not be opened and 1 on success*/

int add_expense(char * fname){
	struct data d;
	int fd = open(fname, O_APPEND|O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1){
		printf("file colud not be opened\n");
		return 0;
	}
	/* now the file has been opened*/
	printf("enter amount and description");
	while(scanf("%d%s",&d.amt, d.des) != EOF){
	write(fd, &d, sizeof(struct data));
	}
	close(fd);
	return 1;
}


int split_expense(char * a[16]){
	int array_expense[16];
	int i = 0, min, max;
	int sum, count = 0;
	//int total_expenses = 0; unused variable
	struct data d;
	char * name[16];
	//int contribution; unused variable
	while(i != 16){
		name[i] = a[i];
		sum = 0;
		if(a[i] != NULL) {
			int fd = open(a[i], O_RDONLY| O_CREAT);/* check if it really needs to be created*/
			while(read(fd, &d, sizeof(struct data)) != 0){
				sum  = sum + d.amt;
			}
			close(fd);
			array_expense[i] = sum;
			i++;
		}
		else{
			array_expense[i] = INT_MIN;
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
		
		printf("%s pays %s an amount of rs %d\n",name[min], name[max], (-1) * array_expense[min]);
		array_expense[max] += array_expense[min];
		array_expense[min] = 0;
		min = findmin(array_expense);
		max = findmax(array_expense);
	}
	reset(a);
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
/* this function returns 1 on success and the file contains data, 2 if file opens but does not contain data and a 0 on failure*/

int load_file(char* grpname, char* grppass, char* mem[16], int * num_mem, char* pass[16]){
	FILE * fp;
	char * data = "stddata";
	char array1[10];
	char array2[10];
	int i;
	
	fp = fopen(data, "r+");
	if(fp == NULL){
		printf("file open failed\n");
		return 0;
	}
	if(fscanf(fp, "%s", grpname) != EOF){
		if(fscanf(fp, "%s", grppass) != EOF){
			if(fscanf(fp, "%d", num_mem) != EOF){
				;
			}
			else{ 
				return 2;
			}
		}
		else{ 
			return 2;
		}
	}
	else{
		return 0;
	}
	for(i = 0; i< (*num_mem); i++){
		fscanf(fp, "%s", array1);
		fscanf(fp, "%s", array2);
		mem[i] = (char *) malloc(10);
		pass[i] = (char*)malloc(10);
		strcpy(mem[i], array1);
		strcpy(pass[i], array2);
	}
	return 1;
}

/* this function will check that login is correct password and username*/
/* this function may return 0 if the person wants to exit and always gets the correct username and password and returns a 1*/

int login(char * grpname, char * mem[16], char * pass[16], int * user){
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
			printf(" this username does not match any in our list\n please try again\n");
			flag = 1;
		}
		else{
			flag = 2;
		}
	}
	printf("hello %s please enter your password\n", mem[j]);
	while(flag == 2){
		
		scanf("%s", buffer);
		if(buffer[0] == '1'){
			flag = 0;
			break;
		}
		if(strcmp(pass[i],buffer) == 0){
			flag = 0;
			*user = j;
			break;
		}
		else{
			flag = 2;
			printf(" password does not match\n please enter again\n");
		}
	}
	if (flag == 0){
		return 0;
	}
	return 1;
}
			
/* this function prints choice seen on the home screen*/
void print_home(){
	printf(" HELLO,\n what would you like to do?\n");
	printf("1. login\n\n");
	printf("2. create a group\n\n");
	printf("3. delete a group\n\n");
	printf("4. exit\n\n");
	return;
}			
	
int main(){
	int num_mem;// this is the total number of members in the group
	char * mem[16];// this the array which stores addresses of usernames
	char * pass[16];// this array stores addresses of respective passwords
	char grpname[10];// char array containing the group name
	char grppass[10]; // this array contains the password of the group
	int user;// this is he current user that has logged in( users index in the array)
	int i, j, k;// these are some random variables to be used as counters.
	int state;// this acs like flag between two different switch cases which go on in an infinite loop wher only few condition can break out of it depending on the value of this flag.
	int choice; // this variable selects the case in switch
	
	/*now to start calling functions one by one:*/
	j = load_file(grpname, grppass, mem, &num_mem, pass);
	if(j == 0){
	 printf(" the standard file could not be opened\n you can still go ahead with working but previous data is lost now\n");
	 }
	 if(j == 1){
	 printf(" file succesfully opened\n looks like you are a new user\n welcome to splitwise\n");
	 }
	 if(j == 2){
	 printf("file successfully loaded\n all previous user data recovered\n welcome back to splitwise\n");
	 }
	 print_line();
	 print_home();
	 scanf("%d", &choice);
	 while(1){
	 	
	 	if(state == 1){
	 		switch(choice){
	 			case 1: 
	 				j = login(grpname, mem, pass, &user);
	 				print_line();
	 				if(j == 0){
	 					state = 3;
	 					break;
	 				}	
	 				if(j == 1){
	 					print_home();
	 					scanf("%d", &choice);
	 					state = 2;
	 				}
	 				break;
	 			case 2: 
					j = check_grp(grpname);
					if(j == 0){
						// no group exists
						k = create_grp(grpname, grppass, mem, pass, &num_mem);
						if(k == 0){
							printf(" creating group failed\n");
							state = 3;
						}
						if(k == 1){
							print_line();
							print_home();
	 						scanf("%d", &choice);
							state = 2;
						}
					}
					else{
						printf("A group alredy exists. you cannot create multiple groups. if youi want to create a new group the current group will have to be deleted.\n press 1 to delete existing group\n press 2 to exit\n");
						scanf("%d", &k);
						if(k == 1){
							delete_grp(grpname, grppass, mem, pass, &num_mem);
							i = create_grp(grpname, grppass, mem, pass, &num_mem);
							if(i == 0){
							printf(" creating group failed\n");
							state = 3;
							}
							if(i == 1){
								print_line();
								print_home();
	 							scanf("%d", &choice);
								state = 2;
							}
						}
						if(k == 2){
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
						delete_grp(grpname, grppass, mem, pass, &num_mem);
					}
					state = 1;
					break;
				
				case 4:
					state = 3;
					break;
					
				case default:
						printf(" you have entered an invalid choice\n please enter again\n");
						print_home();
					break;
			}// switch closed
		}// if closed
		if( state == 2){
			switch(choice){
				case 1:
					j = add_expense(a[user]);
					if(j == 0){
						printf(" sorry could not add any more expenses\n");
						print_home();
	 					scanf("%d", &choice);
						state  = 2;
					}
					if(j = 1){
						print_home();
	 					scanf("%d", &choice);
						state = 2;
					}
				break;
				 case 2:
				 	j = get_statement(user);
				 	if(j == 0){
				 		printf(" sorry, the file could no be opened\n");
				 	}
				 	print_home();
	 				scanf("%d", &choice);
				 	state = 2;
				 	break;
				 	
				 case 3:
				 	j = spilt_expense(mem);
				 	print_home();
	 				scanf("%d", &choice);
				 	state = 2;
				 	break;
				 
				 case 4:
				 	view_my_contri(user);
				 	print_home();
	 				scanf("%d", &choice);
				 	state = 2;
				 	break;
				 	
				 case 5:
				 	j = split_expenses(mem);
				 	if( j == 0){
				 		// unsucessful
				 		state = 2;
				 		break;
				 	}
				 	j = add_member(mem, pass, &num_mem);
				 	if (j == 0){
				 	// unsuccessful
				 		printf(" adding member was unsuccessful\n please try again\n");
				 		print_home();
	 					scanf("%d", &choice);
				 		state = 2;
				 		break;
				 	}
				 	print_home();
	 				scanf("%d", &choice);
				 	state = 2;
				 	break;
				 	
				 case 6:
				 	free(user);
				 	user = NULL;
				 	state = 1;
				 	break;
				 
				 case 7 :
				 	state = 3;
				 	break;
				 	
				 default : printf(" you have netered a wrong choice\n please enter again\n");
				 print_home();
	 			scanf("%d", &choice);
				 state = 2;
				 break;
			 }// switch closed
		}// if closed
		if(state == 3){
			FILE * fp;
			char data[] = "stddata";
			fp = fopen(data, "w");
			if(fp == NULL){
				printf(" file cannot be opened to store all the data you create in this session.\n sorry\n");
				return 0;
			}
			fprintf(fp, "%s\n", grpnme);
			fprintf(fp, "%s\n",grppass);
			fprintf(fp, "%d\n", num_mem);
			for(i = 0; i< num_mem; i++){
				if(mem[i] != NULL){
					fprintf(fp, "%s\n", mem[i]);
					fprintf(fp, "%s\n", pass[i]);
					free(mem[i]);
					free(pass[i]);
					mem[i] = NULL;
					pass[i] = NULL;
				}// if closed
			}
			// for closed
			break;
		}
	}// while closed
	return 0;
}// main closed						
