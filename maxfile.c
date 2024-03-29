#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int function1(char * directoryName, long int * maxWritableFileSize, long int * maxNonWritableFileSize, char * maxWritableName, char * maxNonWritableName){

     DIR *dir;
     dir = opendir(directoryName); //Opening directory

     if (dir == NULL){ //checking if directory successfully opened
          printf("Unable to open directory\n");
          return 1;
     }
     

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



int function2(char * directoryName, long int * maxWritableFileSize, long int * maxNonWritableFileSize, char * maxWritableName, char * maxNonWritableName){

     DIR *dir;
     dir = opendir(directoryName); //Opening directory

     if (dir == NULL){ //checking if directory successfully opened
          printf("Unable to open directory\n");
          return 1;
     }
     
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
               }else if(S_ISDIR(buff.st_mode)){
                    if (strcmp("..", f->d_name)!=0){
                         if (strcmp(".", f->d_name)!=0){
                              function2(path, maxWritableFileSize, maxNonWritableFileSize, maxWritableName, maxNonWritableName);
                         } 
                    } 
               }
               
          }
     }
     closedir(dir);
}




int function3(char * directoryName){

     DIR *dir;
     dir = opendir(directoryName); //Opening directory

     if (dir == NULL){ //checking if directory successfully opened
          printf("du: cannot read directory '%s': Permission denied\n", directoryName);
          printf("4096\t%s\n", directoryName);
          return 1;
     }
     

     struct dirent *f; //stores file
     struct stat buff; //stores file info 
     char path[4096]; //stores path
     size_t totalUsage = 0;
     
     while((f=readdir(dir)) != NULL){     
          strcpy(path, directoryName); //Creating the actual path
          strcat(path,"/");            //I was originally running into issues because
          strcat(path, f->d_name);     //I was putting in the file name into stat


          if(stat(path, &buff) != -1){ //Making sure stat works                
               if(S_ISREG(buff.st_mode)){ //Checking if the file is a regular file 
                    //printf("adding file %s -->  %ld\n", path, buff.st_size);
                    totalUsage += buff.st_size;          
                    continue; //Move on to next file in dir
               }else if(S_ISDIR(buff.st_mode)){
                    //printf("adding directory %s -->  %ld\n", path, buff.st_size);

                    if (buff.st_mode & S_IRUSR){
                         if (strcmp("..", f->d_name)!=0){
                              if (strcmp(".", f->d_name)!=0){
                                   totalUsage += buff.st_size;
                              }
                         }
                    }
               }
               
          }
     }

     totalUsage+=4096;

     printf("%ld\t%s\n", totalUsage, directoryName);

     closedir(dir);
}


int function4(char * directoryName){

     DIR *dir;
     dir = opendir(directoryName); //Opening directory

     if (dir == NULL){ //checking if directory successfully opened
          printf("du: cannot read directory '%s': Permission denied\n", directoryName);
          printf("4096\t%s\n", directoryName);
          return 1;
     }
     
     struct dirent *f; //stores file
     struct stat buff; //stores file info 
     char path[4096]; //stores path

     size_t totalUsage = 0;
     
     while((f=readdir(dir)) != NULL){     
          strcpy(path, directoryName); //Creating the actual path
          strcat(path,"/");            //I was originally running into issues because
          strcat(path, f->d_name);     //I was putting in the file name into stat


          if(stat(path, &buff) != -1){ //Making sure stat works                
               if(S_ISREG(buff.st_mode)){ //Checking if the file is a regular file 
                    if (buff.st_mode & S_IWUSR){ //Writable file case:
                         totalUsage += buff.st_size;          
                         continue; //Move on to next file in dir
                    }else{ //Non-Writable file case:
                         totalUsage += buff.st_size;
                         continue; //move onto next file in dir    
                    }    
               }else if(S_ISDIR(buff.st_mode)){

                    if (strcmp("..", f->d_name)!=0){
                         if (strcmp(".", f->d_name)!=0){
                         totalUsage += buff.st_size;
                         function4(path); 
                         }
                    }
               }             
          }
     }

     totalUsage += 4096;

     printf("%ld\t%s\n", totalUsage, directoryName);

     closedir(dir);
}






int main(int argc, char* argv[]){
     
     long int maxWritableFileSize = 0;
     long int maxNonWritableFileSize = 0;
     
     char maxWritableName[256];
     char maxNonWritableName[256];
     

     //Function 1
     printf("Function 1:\n");
     function1(argv[1], &maxWritableFileSize, &maxNonWritableFileSize, maxWritableName, maxNonWritableName);
     printf("Writable File:       Name: %s, Size: %ld\n",maxWritableName, maxWritableFileSize);
     printf("Non-Writable File:   Name: %s, Size: %ld\n", maxNonWritableName, maxNonWritableFileSize);
     
     //Function 2
     printf("\n\nFunction 2:\n");
     function2(argv[1], &maxWritableFileSize, &maxNonWritableFileSize, maxWritableName, maxNonWritableName);
     printf("Writable File:        Name: %s, Size: %ld\n",maxWritableName, maxWritableFileSize);
     printf("Non-Writable File:    Name: %s, Size: %ld\n", maxNonWritableName, maxNonWritableFileSize);
     
     printf("\n\nFunction 3:\n");
     function3(argv[1]);

     printf("\n\nFunction 4:\n");
     function4(argv[1]);

     return 0;
}
