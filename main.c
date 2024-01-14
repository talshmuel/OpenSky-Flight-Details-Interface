#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>

#include "airportsIcoa.h"
#include "allFilghtsAircrafts.h"
#include "fullSchedule.h"
#include "updateDB.h"
#include "zipFiles.h"


#define PIPE_LIMIT 1024

void gracefull_exit();
void handleSignals (int sigNum);
pid_t childId;
pid_t parentId;

int main() 
{
    if(!directoryExists())
    {
        unZipAllFiles();
    }

    pid_t pid;
    int parent_to_child[2];
    int child_to_parent[2];
   

    // create pipe for commun3ication between parent and child processes
    if (pipe(parent_to_child) == -1) {
        perror("pipe");
        exit(1);
    }

    if (pipe(child_to_parent) == -1) {
        perror("pipe");
        exit(1);
    }

    // fork child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    
    else if (pid == 0)// CHILD
    {

        close(parent_to_child[1]);
        close(child_to_parent[0]);

        signal(SIGUSR1, handleSignals);
        signal(SIGINT, handleSignals);
        
        
        while(1){
           
            int choice;
            read(parent_to_child[0], &choice, sizeof(choice));
            char* buffer;
            int bufferSize=0;
           
            if(choice>=1 && choice<=3){
                char **parameters = NULL;
                int inputSize = 0;
                int inputsize1=read(parent_to_child[0], &inputSize, sizeof(int));
                
                parameters = (char**)malloc(inputSize * sizeof(char *));

                for (int i = 0; i < inputSize; i++) {
                    int length = 0;
                    int length1= read(parent_to_child[0], &length, sizeof(length));
                        

                    parameters[i] = (char *)malloc((length + 1) * sizeof(char));
                    int parameter1=read(parent_to_child[0], parameters[i], length * sizeof(char));
                    parameters[i][length] = '\0';
                }

                
                switch (choice)
                {
                case 1:
                    buffer = allArrivalFlightsDetails(inputSize,parameters,&bufferSize);
                    break;

                case 2:
                        
                    buffer = fullAirportSchedule(inputSize,parameters,&bufferSize);
                    break;

                case 3:
                    
                    buffer = allFlightsAircrafts(inputSize,parameters,&bufferSize);
                    break;
                }

                 for (int i = 0; i < inputSize; i++)
                {
                    free(parameters[i]);
                }
                free(parameters);
                

            }
            else{
                
                switch (choice)
                {
                
                    case 4:
                        updateDB();
                        buffer = (char*)malloc(2*sizeof(char));
                        bufferSize=1;
                        strcpy(buffer,"0");
                        break;
                
                    case 5:
                        //zip;
                        zipAllFiles();
                        buffer = (char*)malloc(2*sizeof(char));
                        bufferSize=1;
                        strcpy(buffer,"0");
                        break;
                    
                    case 6:
                        printf("This is the child process. PID: %d\n", getpid());
                        buffer = (char*)malloc(2*sizeof(char));
                        bufferSize=1;
                        strcpy(buffer,"0");
                        break;
                    case 7:
                        buffer = (char*)malloc(2*sizeof(char));
                        bufferSize=1;
                        strcpy(buffer,"0");
                        break;
                        
                    
                    default:
                        printf("Invalid program number.\n");
                        buffer = (char*)malloc(2*sizeof(char));
                        bufferSize=1;
                        strcpy(buffer,"0");
                        break;
                    }

            }
           

            
            write(child_to_parent[1],&bufferSize,sizeof(int));
           
            int index=0;
            while(bufferSize>0)
            {
                if (bufferSize>=PIPE_LIMIT){
                        write(child_to_parent[1],buffer+(index*PIPE_LIMIT),PIPE_LIMIT*sizeof(char));
                        index++;
                        bufferSize=bufferSize-PIPE_LIMIT;
                }
                else{
                        int num2=write(child_to_parent[1],buffer+(index*PIPE_LIMIT),bufferSize*sizeof(char));
                        bufferSize=0;
                }
            }

            free(buffer);
        }
         close(child_to_parent[1]);
         close(parent_to_child[0]);
    }

    else { // PARENT
        childId=pid;
        parentId = getpid();
        int choice;
        signal(SIGINT, handleSignals);
        close(child_to_parent[1]);
        close(parent_to_child[0]);

        while (1)
        {

            printf("*****************************\n");
            printf("1 - Fetch airports incoming flights.\n");
            printf("2 - Fetch airports full flights schedule.\n");
            printf("3 - Fetch aircraf full flights schedule.\n");
            printf("4 - Update DB.\n");
            printf("5 - Zip DB files.\n");
            printf("6 - Get child process PID.\n");
            printf("7 - Exit.\n");
            printf("Please make your choice <1,2,3,4,5,6,7>:\n");

            scanf("%d", &choice);

            write(parent_to_child[1],&choice,sizeof(int));

            if (choice >= 1 && choice <= 3) {
                
                char **parameters = (char**)malloc(sizeof(char*));
                char input[7];
                int inputSize = 0;
               
                printf("Enter your input\n");
                char c = '0';
            
                while (1) {
                    
                    scanf("%s", input);
                    parameters = (char **)realloc(parameters, (inputSize+1) * sizeof(char *));
                    parameters[inputSize] = (char *)malloc((strlen(input)+1) * sizeof(char));
                    strcpy(parameters[inputSize], input);
                    inputSize++;
                    scanf("%c",&c);
                    if(c=='\n')
                        break;
                }
                

                int inputsize1=write(parent_to_child[1],&inputSize,sizeof(int));
               
                int size;
                for (int i = 0; i < inputSize; i++)
                {
                    size=strlen(parameters[i]);

                    int lenght1= write(parent_to_child[1],&size,sizeof(int));
                   
                    int parameter1=write(parent_to_child[1],parameters[i],sizeof(char)*size);
                    
                }

                   for (int i = 0; i < inputSize; i++)
                    {
                        free(parameters[i]);
                    }
                free(parameters);

            }
            else if (choice == 7) {
                gracefull_exit(pid);
            }
        

                char* output;
                int sizeOut;
                int num1=read(child_to_parent[0],&sizeOut,sizeof(int));

                int sizeOut1 = sizeOut;

                int index=0;
                output = (char*)malloc((sizeOut+1)*sizeof(char));
                while(sizeOut>0)
                {
                    if (sizeOut>=PIPE_LIMIT){
                         read(child_to_parent[0],output+(index*PIPE_LIMIT),PIPE_LIMIT*sizeof(char));
                         index++;
                         sizeOut=sizeOut-PIPE_LIMIT;
                    }
                    else{
                         read(child_to_parent[0],output+(index*PIPE_LIMIT),sizeOut*sizeof(char));
                         sizeOut=0;
                    }
                }
                
               
                output[sizeOut1] = '\0';

                if(strcmp(output,"0")!=0)
                    printf("%s",output);

               
        }
       

        close(child_to_parent[0]);
        close(parent_to_child[1]);
    }


}

void gracefull_exit()
{
  
    int status;
    kill(childId, SIGUSR1);
    waitpid(childId, &status, 0);
    if(WIFEXITED(status))
    {
        printf("The child process is closed\n");
        exit(0);
    }
    else
        printf("An error occurred when trying to close the child process using a signal from the parent process\n");
    
}

void handleSignals (int sigNum)
{
    switch (sigNum)
    {
    case SIGUSR1:
        
        printf("Gracefull exit\n");
        zipAllFiles();
        delete_directory("./flightDB");
        exit(0);
        
    break;

    case SIGINT:
        if(getpid()==parentId)
           {
                printf("parent got SIGINT signal\n");
                gracefull_exit(childId);
                exit(0);
            }
            else{}//parent handling SIGINT signal
        
    break;
    }
}

