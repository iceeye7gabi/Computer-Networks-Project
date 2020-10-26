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
    #include <stdio.h>
#include <dirent.h>
#include<stdlib.h>
#include<string.h>
#include<linux/limits.h>
#include<sys/stat.h>
#include<time.h>
    //using namespace std;
   
   //program andrei
   
   void parcurgere(char* path,char* sir)
{
struct dirent *drum;
  DIR *fis;
  char next_path[PATH_MAX];
  strcpy(next_path,path);
  fis=opendir(path);
//printf("%s\n\n",path);
char*p=strrchr(path,'/');
char cuv[100];
if(fis==NULL){
strcpy(cuv,p+1);
if(strcmp(cuv,sir)==0)
  {
     printf("%s\n",path);
     struct stat stare;
     stat(path,&stare);
 
  }

}
else
  while((drum=readdir(fis)))
    {
    if(drum->d_name[0]!='.')
        {
         if(strlen(next_path)!=1)
         strcat(next_path,"/");
         strcat(next_path,drum->d_name);
         if((strcmp(next_path,"/proc")!=0)&&(strcmp(next_path,"/dev")!=0)&&(strcmp(next_path,"/sys")!=0))
         parcurgere(next_path,sir);
         strcpy(next_path,path);
        }
    }
closedir(fis);
}
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   void printfFileProp(struct stat stats) {
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
    printf("\nModified on: %d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon+1, dt.tm_year + 1900, 
                                              dt.tm_hour, dt.tm_min, dt.tm_sec);

    // File modification time
    dt = *(gmtime(&stats.st_mtime));
    printf("\nCreated on: %d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon+1, dt.tm_year + 1900, 
                                              dt.tm_hour, dt.tm_min, dt.tm_sec);


   	// sursa : https://codeforwin.org/2018/03/c-program-find-file-properties-using-stat-function.html 	
   	
   		printf("\n\n\n");
   }
   
   
   
   int main()
    {	struct stat stats;	
    	char command[50];
    	char command2[50];
    	int fd[2];
    	int fd2[2];
    	int socket[2];
        pid_t pid;
        int rv;
        char username[50];
        char answer[50];
        char path[50];
        char i[50];
        char xx[50];
        int sockp[2];
        socketpair(AF_UNIX, SOCK_STREAM, 0, sockp);
        int sockp2[2];
        int fifo;
        char fisier[50];
        socketpair(AF_UNIX, SOCK_STREAM, 0, sockp2);
        mkfifo("myfifo",0666);
        
	FILE * file1;
        if(pipe(fd)==-1) {perror("Eroare la compunere pipe"); exit(0);}
        if(pipe(fd2)==-1) {perror("Eroare la compunere pipe"); exit(0);}
        while(1){
        pid=fork();            
        if(pid==0){
                //sleep(1);
               read(fd2[0],xx,sizeof(xx));
               if(strcmp(xx,"exit")==0){                    //daca se alege comanda quit
               		strcpy(answer,"Ati iesit din aplicatie.");
               		//write(sockp2[1],answer,sizeof(answer));close(sockp2[1]); 
               		fifo=open("myfifo",O_WRONLY); write(fifo,answer,sizeof(answer)); close(fifo);
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
                	char q[50];                                            //PROCES COPIL
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
               		if(stat(path, &stats)==0)  printfFileProp(stats);
               		strcpy(answer,"Rezultate obtinute");
               		write(sockp[1],answer,sizeof(answer));
               		exit(0);
               	
               	}
               	
               	
               	
               	else if(strcmp(command,"myfind")==0) {
               		
      					printf("Introduceti numele fisierului : \n");
     					 scanf("%s",fisier);
               		char* p = strchr(fisier, '\n');
    				//if (p==0)  *p = 0;
   				parcurgere("/",fisier);
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
                	//read(sockp2[0],sir,sizeof(sir));
                	fifo=open("myfifo",O_RDONLY);
                	read(fifo,sir,sizeof(sir));
                	wait(NULL);
                	if(strcmp(sir,"Ati iesit din aplicatie.")==0){
                	printf("%s\n",sir);
                	return 0;
                	}
               	    
                }
                else{
                strcpy(xx,"abcd");
                write(fd2[1],xx,sizeof(xx));
                write(fd[1],command,sizeof(command));
                                                                  //PROCES TATA
                wait(NULL);
                char sir[50];
                read(sockp[0],sir,sizeof(sir));
               	printf("%s\n",sir);
               	   } 
		       
       }
   }
 }	
