#include<stdio.h>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <unistd.h>
#define SIZE_ARRAY 20

void populateArray(int *elements);

int main(){
    //seed the random number generator
    srand(time(0));

    int elements[SIZE_ARRAY];
    populateArray(elements);

    int start,end;
    int pipefd[2];
    printf("Array size: %d\n" , SIZE_ARRAY);

    if(pipe(pipefd) == -1){
        printf("Error creating pipe");
        return 1;
    }

    int id = fork();

    if( id == -1){
        return 2;
    }

    //if id == 0 then child will get second half
    if( id == 0){
        start = (SIZE_ARRAY/2);
        end = SIZE_ARRAY-1;
        printf("Child loop from %d to : %d\n" ,start, end);
    }
    else{ // parent will get first half
        start = 0;
        end = start + SIZE_ARRAY/2;
        printf("Parent loop from %d to : %d\n" , start, end);
    }

    int minNUmber = elements[start] ;
    for (int i = start; i < end; i++)
    {
        if (elements[i] <= minNUmber)
        {
            minNUmber = elements[i];
        }
                
    }

    printf("The minimum number is: %d\n", minNUmber);

    if(id == 0){ // Child process
        printf("child's process id is  %d\n",getpid());
        write(pipefd[1],&minNUmber,sizeof(minNUmber));
        close(pipefd[1]);
    }
    else{
        printf("parent's process id is  %d\n",getpid());
        close(pipefd[1]);
        int parentMinNumber;
        read(pipefd[0], &parentMinNumber,sizeof(int));
        close(pipefd[0]);
        int mNumber;
        if (minNUmber <= parentMinNumber)
        {
            mNumber= minNUmber;
        }
        else{
            mNumber = parentMinNumber;
        }
    
        printf("Min number in the array is : %d\n",mNumber);
    }

    return 0;
}


/**
 * populateArray function
 * This function populates the array with random numbers.
 */
void populateArray(int *elements){
    int randomNUmber;
    for (int i = 0; i < SIZE_ARRAY; i++)
    {
        randomNUmber = rand()%50+1;
        elements[i]=randomNUmber;
    }
}