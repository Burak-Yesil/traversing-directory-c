#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){

     DIR* dir;
     dir = opendir(argv[1]);
     
     char fileName[1024];
     struct dirent *f;
     struct stat st;
     

     char maxFileName[1024];
     
     
     if (dir == NULL){
     	printf("Unable to open directory");
     	exit(1);
     }
     
     
     while((f=readdir(dir)) != NULL){
     
     	stat(f->d_name, &st); //Getting info on the current file
     	
     	if(S_ISREG(st.st_mode)){  //Checking that the file is a regular file
     	printf("regular file");
     	}
     	
  
     	printf("%s: %ld\n", f->d_name, st.st_size);
     }
     
     
     closedir(dir);
  
     return 0;
}
