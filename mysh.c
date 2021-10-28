/*
Author: Ricardo Mangandi
COP4600 Fall 2021
Project 2 Create a Shell Program
*/



#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<ctype.h>
#include <signal.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCLETTERSUPPORTED 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

#define clear() printf("\033[H\033[J")

struct commands
{
	char *command_name;
	char *parameter;
	int current;
	int size;
	int command_number;
	struct commands *next;
};


int size_of_stack = 0;
struct commands *top = NULL;

char *current_directory_global_variable = NULL;
struct commands *push_to_stack(char *entry_command, int index_stack, char *parameter_passed,int command_number);


void start_shell()
{
	clear();
	 printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t--");
    printf("\n\n\n\n*******************"
        "***********************");
	char*username = getenv("USER");
	printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}




void history_command()
{

		struct commands *temp = top;
		int count = 0;
		while(temp != NULL)
		{
		printf("%d: %s %d\n", count, temp -> command_name, temp -> command_number);
		temp = temp -> next;
		count++;
		}	 
	

}



char *whereami_command()
{	
	
	char cwd[1024];
    getcwd(cwd, sizeof(cwd));

	if(current_directory_global_variable == NULL)
	{

    current_directory_global_variable = malloc(sizeof(char)*(strlen(cwd) + 1));

	}

	else
	{

	free(current_directory_global_variable);
	current_directory_global_variable = malloc(sizeof(char)*(strlen(cwd) + 1));
	
	}
	
    strcpy(current_directory_global_variable,cwd);
    return current_directory_global_variable;
	
}

struct commands *find_node(int index)
{
	struct commands *temp = top;
	int counter = 0;
	while(counter != index)
	{	

		temp = temp -> next;
		counter++;
	}

	return temp;

}


char **init_valid_commands()
{

	char **list_of_valid_commands = (char **) malloc(sizeof(char*)*7);

	list_of_valid_commands[0] = (char *) malloc(sizeof(char)*strlen("movetodir") + 1);
	strcpy(list_of_valid_commands[0],"movetodir");

	//DONE
	list_of_valid_commands[1] = (char *) malloc(sizeof(char)*strlen("whereami") + 1);
	strcpy(list_of_valid_commands[1],"whereami");

	//DONE
	list_of_valid_commands[2] = (char *) malloc(sizeof(char)*strlen("history") + 1);
	strcpy(list_of_valid_commands[2],"history");

	//DONE
	list_of_valid_commands[3] = (char *) malloc(sizeof(char)*strlen("byebye") + 1);
	strcpy(list_of_valid_commands[3],"byebye");

	//DONE
	list_of_valid_commands[4] = (char *) malloc(sizeof(char)*strlen("replay") + 1);
	strcpy(list_of_valid_commands[4],"replay");


	list_of_valid_commands[5] = (char *) malloc(sizeof(char)*strlen("start") + 1);
	strcpy(list_of_valid_commands[5],"start");


	list_of_valid_commands[6] = (char *) malloc(sizeof(char)*strlen("background") + 1);
	strcpy(list_of_valid_commands[6],"background");


	list_of_valid_commands[7] = (char *) malloc(sizeof(char)*strlen("dalek") + 1);
	strcpy(list_of_valid_commands[7],"dalek");

	return list_of_valid_commands;
}

int returns_integer_value(char *parameter)
{

	if(parameter[0] == 48 || parameter[1] == 48)
		return 0;

	char *builds_integer = malloc(sizeof(char)*10);
	int counter = 0;
	for(int i = 0; i < strlen(parameter); i++)
	{

		if(isdigit(parameter[i]) == 1)
		{
			builds_integer[counter] = parameter[i];
			counter++;
		}

	}

	if(strlen(builds_integer) > 0)
	{
		int val = atoi(builds_integer);
		return val;
	}

	else
	{
		return -999999;
	}


}

char **parse_parameter_command_start(char *parameter)
{

	//if paraemeter[0] != 47 then we need to concat to current directory
	if(parameter[0] != 47)
	{	


		int current_path_length = strlen(whereami_command());
		int size = strlen(parameter) + current_path_length;
		


		
		char *new_path_specified = malloc(sizeof(char)*size+1);

		

		//parameter = 'test' param param param
		// /users/ricardomangandi/desktop/c-programs

		for(int i = 0; i < strlen(current_directory_global_variable); i++)
		{	
			new_path_specified[i] = current_directory_global_variable[i];
		}


		new_path_specified[current_path_length] = 47;

		int j = 0;
		int temp_counter = current_path_length + 1;
		while(j < strlen(parameter))
		{

			char temp = parameter[j];

			if(temp == 32)
				break;

			new_path_specified[temp_counter] = temp;
			temp_counter++;
			j++;
		}

		new_path_specified[temp_counter] = '\0';

		printf("\nnew path: %s\n", new_path_specified);
	

		//get parameter if there are any
		char **arg = malloc(sizeof(char*) * strlen(parameter) + 1);


		if(j == strlen(parameter))
		{	
			//just return the 2d array with the specified path
			printf("There are no parameter");

			arg[0] = malloc(sizeof(char)*strlen(new_path_specified)+1);

			strcpy(arg[0],new_path_specified);

			return arg;

		}

		else
		{

		printf("Look for parameters and store them in 2d array and return");

		int new_counter_i = j + 1;
		int new_counter_j = 0;
		int two_d_counter = 1;
		
		char *temp_collector = malloc(sizeof(char)*strlen(parameter) + 1);

		arg[0] = malloc(sizeof(char)*strlen(new_path_specified)+1);

		strcpy(arg[0],new_path_specified);

		while(new_counter_i <= strlen(parameter))
		{
			

			char temp_holder = parameter[new_counter_i];

			new_counter_i++;

			if(temp_holder == 32 || temp_holder == '\n' || temp_holder == '\0')
			{	

				temp_collector[new_counter_j] = '\0'; 

				new_counter_j = 0;

				arg[two_d_counter] = malloc(sizeof(char) *strlen(temp_collector)+1);

				strcpy(arg[two_d_counter],temp_collector);

				two_d_counter++; 
				
				free(temp_collector);
				
				temp_collector = malloc(sizeof(char)*strlen(parameter) + 1);
			
			}

			else
			{
				temp_collector[new_counter_j] = temp_holder;
				new_counter_j++;
			}


			}

			/*
			for(int k = 0; k < two_d_counter;k++ )
			{
				printf("\nIndex %d and val:%s",k, arg[k]);
			}
			*/

			return arg;

		}
	}


	else
	{

	int i = 0;
	char **arg = malloc(sizeof(char*) * strlen(parameter) + 1);
	char *full_path = malloc(sizeof(char) *strlen(parameter) + 1);
	int j = 0;

	while(i < strlen(parameter))
	{
		char temp = parameter[i];

		if(temp == 32 || temp == '\n' || temp == '\0')
		{
			break;
		}

		else
		{
			full_path[j] = temp;
			j++;
		}

		i++;
	}

	full_path[i] = '\0';

	arg[0] = malloc(sizeof(char) * strlen(full_path) + 1);

	strcpy(arg[0],full_path);

	int two_d_counter = 1; //this means there is only one argument passed which is the path
	
	if(i != strlen(parameter))
	{
		
		int new_counter_i = i + 1;
		int new_counter_j = 0;
		
		char *temp_collector = malloc(sizeof(char)*strlen(parameter) + 1);

		while(new_counter_i <= strlen(parameter))
		{
			

			char temp_holder = parameter[new_counter_i];

			new_counter_i++;

			if(temp_holder == 32 || temp_holder == '\n' || temp_holder == '\0')
			{	

				temp_collector[new_counter_j] = '\0'; 

				new_counter_j = 0;

				arg[two_d_counter] = malloc(sizeof(char) *strlen(temp_collector)+1);

				strcpy(arg[two_d_counter],temp_collector);

				two_d_counter++; 
				
				free(temp_collector);
				
				temp_collector = malloc(sizeof(char)*strlen(parameter) + 1);
			
			}

			else
			{
				temp_collector[new_counter_j] = temp_holder;
				new_counter_j++;
			}
		}
	}


	/*
	for(int k = 0; k < two_d_counter;k++ )
	{
		printf("\nIndex %d and val:%s",k, arg[k]);
	}
	*/
	return arg;

}
}



int command_start(char *parameter)
{


	char ** arg = parse_parameter_command_start(parameter);

	

	pid_t pid;
	int status_of;
	pid = fork();

	//printf("\nThe pid is: %d\n", (int) pid);

	if(pid < 0)
	{
		
		printf("\nCould not use fork()\n");

		kill(pid,SIGKILL);

		return -1;

	}

	else if(pid == 0)
	{	
		
		int res = execv(arg[0],arg);

      	if( res == -1)
      	{
      		printf("\nExecv error could not find path or file\n");
      		return -1;
      	}

	}

	else
	{	
		
		if (waitpid(pid, &status_of, 0) > 0) 
		{
              
            if (WIFEXITED(status_of) && !WEXITSTATUS(status_of))
            {
              printf("program execution successful\n");
              return 5;
            }
              
            else if (WIFEXITED(status_of) && WEXITSTATUS(status_of)) 
            {
                if (WEXITSTATUS(status_of) == 127)
                 	{
                    	// execv failed
                    	printf("execv failed\n");
                    	return -1;
                	}
               
                else
                	{

                	   printf("Program terminated normally, but returned a non-zero status\n");                
                	   return -1;
                	} 
            }
            		
            else
            {
				printf("program didn't terminate normally\n");  
				return -1;
            } 
               	          
        } 
        
        else 
        {
           printf("waitpid() failed\n");
           return -1;
        }
	}


}


pid_t command_background(char *parameter)
{


	pid_t pid;

	// fork the process
	pid = fork();
	
	// check for failure. main will handle the error
	if (pid < 0)
	{
		kill(pid, SIGKILL);
		return -1;
	}

	// for the child
	else if (pid == 0)
	{
		// if the execution call doesn't return an error code
		if (execv("ls", "-l") < 0)
		{
			printf("\nExecution call was not able to return\n");
			return -1;
		}
	}


	return pid;

}


int dalek_command(pid_t target_val)
{


	if(kill(target_val,SIGKILL) == 0)
	{
		printf("Dalek is done process is dead: %d",(int) target_val);
		return 7;
	}

	else
	{
		printf("Dalek is NOT DEAD: %d",(int) target_val);
		return -1;
	}

}


int execute_command(int command_number, char *parameter)
{

	//needs parameter
	if(command_number == 0)
	{
		printf("\nmovetodir executed\n");
		return 0;
	}


	//needs to print directory
	else if(command_number == 1)
	{
		printf("\nwhereami executed\n");
		current_directory_global_variable = whereami_command();
		printf("\nDir: %s\n",current_directory_global_variable);
		return 1;
	}

	//optional parameter
	else if(command_number == 2)
	{	
		history_command();
		return 2;
	}

	//no parameter needed
	else if(command_number == 3)
	{
		printf("\nbyebye\n");
		return 3;
	}

	//parameter needed
	else if(command_number == 4)
	{
		printf("\nreplay executed\n");


		int val = returns_integer_value(parameter);

		//printf("\nThe size of stack is %d Testing:%d\n",top->size ,val);

		

		if(val == -999999)
		{
			printf("\nOnly integers are allowed to be passed in\n");
			return -2;
		}

		else if(val > size_of_stack || val < 0)
		{
			printf("\nIndex %d is out of bound\n", val);
			return -1;

		}


		else
		{
			printf("\ncommand at index %d is executed\n",val);
			struct commands *temp = find_node(val);
			execute_command(temp ->command_number,temp->parameter);
			top = push_to_stack(temp->command_name, temp->current, temp->parameter, temp->command_number);
			return 4;
		}


	}

	else if(command_number == 5)
	{
		printf("\nstart executed\n");


		int res = command_start(parameter);


		if(res == 5)
		{	
			printf("\nProgram executed!\n");
			return 5;
		}

		else
		{
			printf("\nError: The start command could not be executed\n");
			return -1;
		}



		
	}

	//parameter needed
	else if(command_number == 6)
	{
		printf("\nbackground executed\n");
		int res = command_background(parameter);

		dalek_command(res);


		if(res == -1)
		{	
			printf("\nError: Could not run background\n");
			return -1;
		}

		else
		{	
			printf("\nThe PID of program: %d\n",res);
			return 6;
		}
		
	}

	//no parameter needed
	else if(command_number == 7)
	{
		printf("\ndalek PID executed\n");


		//dalek_command(parameter);

		return 7;

	}

	return -1;

}

//checks if it a valid command if yes executes it
int command_valid(char *command, char *parameter)
{

	char **list_of_valid_commands = init_valid_commands();


		for(int i = 0; i < 8; i++)
		{

			if(strcmp(list_of_valid_commands[i],command) == 0)
			{	
				int res = execute_command(i,parameter);
				
				if( res > -1)
					return res;

				//parameter
				else if(res == -2)
				{	
					printf("\nCommand %s could not be executed invalid parameter %s\n", command,parameter);
					return -2;
				}	

				else
				{	
					printf("\nCommand %s could not be executed\n",command);
					return -1;
				}

			}	

		}

	
	
	return -1;

}

struct commands *push_to_stack(char *entry_command, int index_stack, char *parameter_passed,int command_number)
{
	struct commands *node = malloc(sizeof(struct commands));
	node -> command_name = malloc(sizeof(char)*strlen(entry_command)+1);

	if(parameter_passed != NULL)
	node -> parameter = malloc(sizeof(char)*strlen(parameter_passed)+1);
	

	if(top == NULL)
	{	
			strcpy(node ->command_name,entry_command);

			if(parameter_passed != NULL)
				strcpy(node -> parameter,parameter_passed);

			else
				node -> parameter = NULL;

			node -> current = index_stack;
			node -> next = NULL;
			node -> size = 1;
			node -> command_number = command_number;
			size_of_stack = 1;
			return node;

	}

	else
	{
			strcpy(node ->command_name,entry_command);
			
			if(parameter_passed != NULL)
				strcpy(node -> parameter,parameter_passed);

			else
				node -> parameter = NULL;

			node -> current = index_stack;
			node -> next = top;
			node -> command_number = command_number;
			node -> size = node -> size + 1;
			size_of_stack ++;
			return node;
	}


}


struct commands *analyze_command_by_user_function(char *entry_command, int index_stack)
{	



	//struct commands *refer = malloc(sizeof(struct commands));
	int len = strlen(entry_command);
	int counter = 0;
	char traversal_char = entry_command[0];
	char *temp = malloc(sizeof(char)*MAXCLETTERSUPPORTED);

	//if we hit a white space or end or the of the string break
	while(counter < len)
	{
		traversal_char = entry_command[counter];

		if(traversal_char == 32 || traversal_char == '\n')
		{
			break;
		}

		else
		{
			temp[counter] = traversal_char;
		}
		
		counter++;
	}

	temp[counter] = '\0';
	//there is no parameter in the history so we do not have to clear anything
	
	int tester = strcmp("history",temp);


	if(strcmp("history",temp) == 0 && counter == len)
	{	
		//printf("\ntester: %d\n", tester);

		top = push_to_stack(temp,index_stack,NULL,2);
		history_command();
		return top;
	}


	//there is only one command with zero parameter
	else if(counter == len)
	{	
		//verify the command before placing it in a node
		int res = command_valid(temp,NULL);
		if(res < 0)
		{
			free(temp);	
			printf("\n Error not a valid command!\n");
			return top;
		}

		
		else
		{	
			return push_to_stack(temp,index_stack,NULL,res);
		}
		
	}
	


	//there is a command with parameters
	else
	{
			
		
		char *other_temp = malloc(sizeof(char)*MAXCLETTERSUPPORTED); //gets parameter

		int other_counter = 0;

		/*
		for(int i = counter; i < len;i++)
		{
			printf("\nindex:%d and val: %c\n",i,entry_command[i]);
		}
		*/

		int j_counter = counter + 1;
		while(j_counter < len)
		{	

			traversal_char = entry_command[j_counter];
			other_temp[other_counter] = traversal_char;
			j_counter++;
			other_counter++;
		}

		other_temp[other_counter] = '\0';


		//VERY IMPORTANT HERE IS WHERE THE COMMAND GETS EXECUTED 
		//ALONG WITH IT PARAMETER PASSED IN

		int res = -999999;
		if(strcmp("history",temp) != 0)
			res = command_valid(temp, other_temp);



		//THE STACK GETS CLEARED HERE VERY IMPORTANT !!!!
		if(strcmp("history",temp) == 0)
		{
			if(strcmp("-c",other_temp ) == 0)
			{	
				printf("\nThe history has been cleared\n");
				size_of_stack = 0;
				return NULL;
			}
			

			else 
			{
				printf("\nError %s is not a valid parameter for history\n",other_temp);
				return top;
			}
		}

		
		else if(res < 0)
			{	
				printf("\nhas parameter\n");
				int length = strlen(temp);
				printf("\n Error %s is not a valid command it length is: %d!\n", temp,length);
				free(temp);
				return top;
			}


		else
			{
				return push_to_stack(temp,index_stack,other_temp,res);
			}

	}

	return NULL;
}





int main()
{

	char inputString[MAXCLETTERSUPPORTED];
	char *parsedArgs[MAXLIST];

	int continueFlag = 0;

	start_shell();

	char *command_by_user = malloc(sizeof(char)*MAXCLETTERSUPPORTED);
	
	int index_stack_main = 0;

	while(1)
	{
		printf("\n# ");

		//get command and possible parameter
		scanf(" %[^\n]s",command_by_user);


		if(strcmp(command_by_user,"byebye") == 0)
		{	
		
			break;
		}

		else
		{
			top = analyze_command_by_user_function(command_by_user,index_stack_main);
			index_stack_main++;
		}


	}
	
}