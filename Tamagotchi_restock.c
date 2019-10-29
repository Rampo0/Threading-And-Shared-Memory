#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/ioctl.h>

void changemode(int);
int  kbhit(void);

int *shopFoodStock;

void *display(void *ptr){

    while(1){
        
        printf("Shop\n");
        printf("Food stock : %d\n",*shopFoodStock);
        printf("Choices\n");
        printf("1. Restock\n");
        printf("2. Exit\n");
      
        system("clear");
    }

}

void input(){

    char input;
    changemode(1);
    while (1) {
        
       if(!kbhit()){
            
            input = getchar();
            if(input == '1'){
                *shopFoodStock=*shopFoodStock+1;
            }else if(input == '2'){
                changemode(0);
                exit(1);
            }
       
       }

    }

}


int main(){

    pthread_t thread;
    
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    shopFoodStock = shmat(shmid, NULL, 0);

    if(pthread_create(&thread,NULL,display,NULL)){
        printf("failed\n");
    }

    /*for(int i=0; i<1; i++){
       pthread_join(thread[0],NULL);
    }*/
    input();

    shmdt(shopFoodStock);
    shmctl(shmid, IPC_RMID, NULL);

    changemode(0);
        
    exit(1);
    return 0;
}

void changemode(int dir){
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 
int kbhit (void){
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

