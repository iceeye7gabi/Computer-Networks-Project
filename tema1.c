#include <stdlib.h>
    #include <errno.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <stdio.h> 
    #include <string.h>
    #include <sys/socket.h>
    #include <signal.h>
    #include <time.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include<linux/limits.h>
    #include <dirent.h>  
    
    
   
void myfind(char* string,char* road)
{ DIR *F;F=opendir(road);
  char *p=strrchr(road,'/'),w1[1000];
  char PATH_SEC[PATH_MAX];
  struct dirent *PATH_FIRST;
  strcpy(PATH_SEC,road);
  if(F==NULL){
  strcpy(w1,p+1);
  if(strcmp(w1,string)==0){
     printf("%s",road);
     printf("\n");
     }
}
else
  while((PATH_FIRST=readdir(F))){
    if(PATH_FIRST->d_name[0]!='.'){
         if(strlen(PATH_SEC)!=1)
         	strcat(PATH_SEC,"/");
         strcat(PATH_SEC,PATH_FIRST->d_name);
         if((strcmp(PATH_SEC,"/dev")!=0)&&(strcmp(PATH_SEC,"/sys")!=0)&&(strcmp(PATH_SEC,"/proc")!=0))
         	myfind(string,PATH_SEC);
         strcpy(PATH_SEC,road);
        }
    }
  closedir(F);
}
 
void mystat(struct stat stats) {
   struct tm dt;
   
   printf("Accese fisier: ");
   if (stats.st_mode & R_OK) 
   	printf("read ");
   if (stats.st_mode & W_OK) 
   	printf("write ");	
   if (stats.st_mode & X_OK) 
   	printf("execute ");
   printf("\n");
   		
   printf("Dimensiunea fisierului : %ld", stats.st_size);
   		
    dt = *(gmtime(&stats.st_ctime));
    printf("\nModified on: %d-%d-%d %d:%d:%d",
     dt.tm_mday, dt.tm_mon+1, dt.tm_year , 
     dt.tm_hour+2, dt.tm_min, dt.tm_sec);

    dt = *(gmtime(&stats.st_mtime));
    printf("\nCreated on: %d-%d-%d %d:%d:%d", 
    dt.tm_mday, dt.tm_mon+1, dt.tm_year , 
    dt.tm_hour+2, dt.tm_min, dt.tm_sec);

	// am pus la hour +2 pentru ca cumva imi da ceasul cu 2 ore mai putin
	// am pus la month +1 pentru ca cumva imi da luna anterioare(in loc de oct este sep)
   	
   	// sursa : https://codeforwin.org/2018/03/c-program-find-file-properties-using-stat-function.html 	
   	
   printf("\n\n\n");
   }
   
   
   
   int main()
    {	struct stat stats;	
    	char command[50],command2[50];;
    	int fd[2],fd2[2],socket[2];
        pid_t pid;
        char username[50],answer[50],path[50],i[50],xx[50];
        int sockp[2]; socketpair(AF_UNIX, SOCK_STREAM, 0, sockp);
        int sockp2[2]; socketpair(AF_UNIX, SOCK_STREAM, 0, sockp2);
        int fifo;
        char fisier[50];
        mkfifo("myfifo",0666);
        
	FILE * file1;
        if(pipe(fd)==-1) {perror("Eroare la compunere pipe"); exit(0);}
        if(pipe(fd2)==-1) {perror("Eroare la compunere pipe"); exit(0);}
        
        while(1){
        	pid=fork();            
        	if(pid==0){              
               	read(fd2[0],xx,sizeof(xx));
               	if(strcmp(xx,"exit")==0){                    
               		strcpy(answer,"Ati iesit din aplicatie."); 
               		fifo=open("myfifo",O_WRONLY);
               		write(fifo,answer,sizeof(answer)); close(fifo);
               		exit(0);
               }
                
               else{
                                    //daca se alege orice alta comanda inafara de quit
                	read(fd[0],command,sizeof(command));
                	close(fd[0]);
                
                
               	 if(strcmp(command,"login")==0){
                	 	printf("Introduceti numele si parola dupa urmatoarea sintaxa\n");
                	 	printf("USER:Password : ");
                		scanf("%s",username);
                		char q[50];                                            
                		char aux[50];
                		file1 = fopen ("config.txt" , "r");
                		if (file1 == NULL) perror ("Error opening file");
                  		while(!feof(file1))
                   			if(fgets(aux,50,file1)!=NULL){
                    				if(strstr(aux,username)!=NULL){
                      					strcpy(q,"V-ati conectat!");
                      					break;
                   				}
                    			else{
                  				strcpy(q,"Ne pare rau, username-ul sau parola sunt gresite!");
              				}
              			}	
               		write(sockp[1],q,sizeof(q));
               		exit(0);
               		close(fd[1]);	
                	}			
               	else if(strcmp(command,"mystat")==0) {
               		printf("Introduceti path-ul: ");
               		scanf("%s",path);
               		if(stat(path, &stats)==0)  mystat(stats);
               		strcpy(answer,"Rezultate obtinute");
               		write(sockp[1],answer,sizeof(answer));
               		exit(0);
               	
               	}
               	else if(strcmp(command,"myfind")==0) {
				printf("Introduceti numele fisierului : \n");
     				scanf("%s",fisier);
               		char* p = strchr(fisier, '\n');
    				//if (p==0)  *p = 0;
   				myfind(fisier,"/");
               		strcpy(answer,"\n");
               		write(sockp[1],answer,sizeof(answer));
               		exit(0);
               	
               	}
               //////////////////////////// 
               	else {
               		strcpy(answer,"Comanda executata nu exista!");
               		write(sockp[1],answer,sizeof(answer));
               		exit(0);
  
              	        }		
               	
              //////////////////////////////
           }	
         }
            else {
                printf("\n");
                printf("Introduceti comanda : "); scanf("%s",command);
                if(strcmp(command,"exit")==0){
                	strcpy(xx,"exit");
                	write(fd2[1],xx,sizeof(xx));
                	char sir[50];
                	fifo=open("myfifo",O_RDONLY);
                	read(fifo,sir,sizeof(sir));
                	wait(NULL);
                	if(strcmp(sir,"Ati iesit din aplicatie.")==0){
                		printf("%s\n",sir);
                               printf("[INFO]Aplicatie realizata de Constantinescu George-Gabriel,Grupa E3,Anul 2,Anul universitar 2020-2021, Disciplina Retele de Calculatoare \n");
                               printf("Profesor de laborator: Panu Andrei\n");
                		return 0;
                	}
               	    
                }
                if(strcmp(command,"meniu")==0){
                	printf("Acestea sunt comenzile pe care le poti introduce \n");
                	printf("login \n");printf("myfind \n");printf("mystat \n");
                	printf("meniu \n");printf("exit \n");
                	
                
                }
        	else{
            		strcpy(xx,"other");
             		write(fd2[1],xx,sizeof(xx));
             		write(fd[1],command,sizeof(command));
                                                                                                        
       		wait(NULL);
              		char sir[50];
             		read(sockp[0],sir,sizeof(sir));
             		printf("%s\n",sir);
               }  
		       
       }	
   }
 }
 
 
 
 					//RESOURCES
 //https://www.youtube.com/watch?v=Mqb2dVRe0uo
 //https://www.youtube.com/watch?v=8Q9CPWuRC6o
 //https://www.youtube.com/watch?v=2hba3etpoJg
 //https://profs.info.uaic.ro/~andrei.panu/ (link-urile puse pentru fiecare saptamana)
 //https://codeforwin.org/2018/03/c-program-find-file-properties-using-stat-function.html 	 (folosit pt functia mystat)
 //https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/dirent.h(folosit pt functia myfind)
// i hope it's ok :)
