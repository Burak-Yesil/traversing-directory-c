#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

void function1(char * directoryName, long int * maxWritableFileSize, long int * maxNonWritableFileSize, char * maxWritableName, char * maxNonWritableName){

     DIR *dir;
     dir = opendir(directoryName); //Opening directory

     if (dir == NULL){ //checking if directory successfully opened
          printf("Unable to open directory\n");
          exit(1);
     }
     
     printf("Function 1 Working:\n");

     struct dirent *f; //stores file
     struct stat buff; //stores file info 
     
     char path[4096]; //stores path


     
     while((f=readdir(dir)) != NULL){     
          strcpy(path, directoryName); //Creating the actual path
          strcat(path,"/");            //I was originally running into issues because
          strcat(path, f->d_name);     //I was putting in the file name into stat


          if(stat(path, &buff) != -1){ //Making sure stat works                
               if(S_ISREG(buff.st_mode)){ //Checking if the file is a regular file 
                    if (buff.st_mode & S_IWUSR){ //Writable file case:
                         if (*maxWritableFileSize < buff.st_size){ //Checking if new file has max size
                              *maxWritableFileSize = buff.st_size; 
                              strcpy(maxWritableName, f->d_name); 
                              continue; //Move on to next file in dir
                         }
                    }else{ //Non-Writable file case:
                         if(*maxNonWritableFileSize < buff.st_size){ //Checking if new file has max size
                              *maxNonWritableFileSize = buff.st_size;
                              strcpy(maxNonWritableName, f->d_name);
                              continue; //move onto next file in dir
                         }
                    }    
               }
          }
     }

     closedir(dir);
}






int main(int argc, char* argv[]){

     long int maxWritableFileSize = 0;
     long int maxNonWritableFileSize = 0;
     
     char maxWritableName[256];
     char maxNonWritableName[256];

     function1(argv[1], &maxWritableFileSize, &maxNonWritableFileSize, maxWritableName, maxNonWritableName);
  
     printf("Writable File Name: %s, Size: %ld\n",maxWritableName, maxWritableFileSize);
     printf("Non-Writable File Name: %s, Size: %ld\n", maxNonWritableName, maxNonWritableFileSize);



     return 0;
}
