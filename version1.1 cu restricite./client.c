#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <errno.h>

#include <unistd.h>

#include <stdio.h>

#include <stdlib.h>

#include <netdb.h>

#include <string.h>

#define red_color "\x1b[31m"
#define yellow_color "\x1b[33m"
#define blue_color "\x1b[36m"
#define white_color "\x1b[0m"

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char * argv[]) {
  int socket_descriptor; // descriptorul de socket
  struct sockaddr_in server; // structura folosita pentru conectare
  // mesajul trimis
  int nr = 0;
  char message[100];
  char logare[100];
  char nickname[100];
  char password[100];
  char email[100];
  int type;
  int logged=0;

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3) {
    printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }

  /* stabilim portul */
  port = atoi(argv[2]);

  /* cream socketul */
  if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Eroare la socket().\n");
    return errno;
  }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons(port);

  /* ne conectam la server */
  if (connect(socket_descriptor, (struct sockaddr * ) & server, sizeof(struct sockaddr)) == -1) {
    perror("[client]Eroare la connect().\n");
    return errno;
  }


 printf (red_color "\n\n\n\n\n                          BINE AI VENIT!                        \n" white_color);
 printf (red_color "                 Hello! :) Welcome to BrailaChampionship! \n" white_color);
 printf (yellow_color "                  Here are some tips before you start:\n" white_color);
 printf (yellow_color "                        This color is for tips.\n" white_color);
 printf (yellow_color "                     This color is for good output.\n" white_color);
 printf (yellow_color  "                       This color is for errors.\n" white_color);
 printf (blue_color"        This color is for database errors or for a wrong command.\n" white_color);
 printf ("         This color is for client errors, input or for quitting.\n\n\n");
 printf (blue_color "                       Now you are good to start.\n" white_color);
 printf (blue_color "           Type in 'commands' to see the available commands.\n\n\n\n\n\n" white_color);


  while (1) {
    /* citirea mesajului */
    printf("[client]Introduceti o comanda: ");
    fflush(stdout);
    scanf("%s", & message);
    fflush(stdin);

    //printf("[client] Am citit %d\n",nr);

    /* trimiterea mesajului la server */
    if (write(socket_descriptor, & message, sizeof(message)) <= 0) {
      perror("[client]Eroare la write() spre server.\n");
      return errno;
    }

    /* citirea raspunsului dat de server
       (apel blocant pina cind serverul raspunde) */
    if (read(socket_descriptor, & message, sizeof(message)) < 0) {
      perror("[client]Eroare la read() de la server.\n");
      return errno;
    }

    /* afisam mesajul primit */

    if (strcmp(message, "quit") == 0) {
      printf(blue_color "Ati iesit din aplicatie! O zi buna!\n"
        white_color);
      break;
    }
    else if (strcmp(message, "login") == 0 )  {
      if(logged==0){
      printf("Introduceti nickname-ul: ");
      fflush(stdout);
      scanf("%s", & nickname);
      fflush(stdin);
      write(socket_descriptor, & nickname, sizeof(nickname));
      printf("Introduceti parola: ");
      fflush(stdout);
      scanf("%s", & password);
      fflush(stdin);
      write(socket_descriptor, & password, sizeof(password));
      read(socket_descriptor, & logare, sizeof(logare));
      printf("%s\n", logare);
      if(strcmp(logare,"V-ati logat! Bine ai venit jucatorule!")==0 || strcmp(logare,"V-ati logat! Bine ai venit administratorule!")==0)
        logged=1;
      }
      else {
        write(socket_descriptor, "Esti deja logat1!", sizeof("Esti deja logat1!"));
        write(socket_descriptor, "Esti deja logat2!", sizeof("Esti deja logat2!"));
        read(socket_descriptor, & logare, sizeof(logare));
        printf("%s\n",logare);

      }
    }
    else if (strcmp(message, "register") == 0) {
      if(logged==0){
      printf("Introduceti nickname-ul: ");
      fflush(stdout);
      scanf("%s", & nickname);
      fflush(stdin);
      write(socket_descriptor, & nickname, sizeof(nickname));

      printf("Introduceti parola: ");
      fflush(stdout);
      scanf("%s", & password);
      fflush(stdin);
      write(socket_descriptor, & password, sizeof(password));

      printf("Introduceti email-ul: ");
      fflush(stdout);
      scanf("%s", & email);
      fflush(stdin);
      write(socket_descriptor, & email, sizeof(email));

      read(socket_descriptor, & logare, sizeof(logare));
      printf("%s\n", logare);
    }
    else {write(socket_descriptor, "Esti deja logat1!", sizeof("Esti deja logat1!"));
    write(socket_descriptor, "Esti deja logat2!", sizeof("Esti deja logat2!"));
    write(socket_descriptor, "Esti deja logat3!", sizeof("Esti deja logat3!"));
    read(socket_descriptor, & logare, sizeof(logare));
    printf("%s\n",logare);
    }
    }

    else if(strcmp(message,"turneu")==0) {
        if(logged==1) {}
        else printf("Logheazate!");
    }

    else if (strcmp(message, "Command not found!") == 0) {
      printf(red_color "Comanda nu exista!\n"
        white_color);
      continue;
    }

  }
  /* inchidem conexiunea, am terminat */
  close(socket_descriptor);
  return 0;
}
