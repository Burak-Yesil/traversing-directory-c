#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


int main(int argc, char* argv[]){

     DIR* dir;
     dir = opendir(argv[1]); //Opening Directory from CLI
     
     
     struct dirent *f; //stores file
     struct stat buff; //stores file info
     

     char maxFileNameW[1024]; //Largest writable 
     char maxFileNameNW[1024]; //Used to keep track of largest file

     
     if (dir == NULL){ //Checking if directory exists
     	printf("Unable to open directory\n");
     	exit(1);
     }
     
     
     while((f=readdir(dir)) != NULL){
     
     	stat(f->d_name, &buff); //Getting info on the current file
     	
     	
     	//printf("Mode: %d\n", buff.st_mode);

     	if(S_ISREG(buff.st_mode) != 0){  //Checking that the file is a regular file
     	printf("regular file");
     	}
     	
  
     	printf("%s: %ld\n", f->d_name, buff.st_size);
     }
     


     
     closedir(dir);
  
     return 0;
}
