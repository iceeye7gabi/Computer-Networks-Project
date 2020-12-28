#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <errno.h>

#include <unistd.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <signal.h>

#include <pthread.h>

#include <sqlite3.h>

sqlite3 * database;
#define red_color "\x1b[31m"

/* portul folosit */
#define PORT 2323

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData { //structura pt thread
  int idThread; //id-ul thread-ului tinut in evidenta de acest program
  int cl; //descriptorul intors de accept
}
thData;

static int callback(void * str, int argc, char ** argv, char ** azColName) {
  int i;
  char * data = (char * ) str;
  for (i = 0; i < argc; i++) {
    strcat(data, azColName[i]);
    strcat(data, " = ");
    if (argv[i]) strcat(data, argv[i]);
    else strcat(data, "NULL");
    strcat(data, "\n");
  }

  strcat(data, "\n");
  return 0;

}

void raspunde(void * arg) {
  char message[100];
  char nickname[100];
  char password[100];
  char email[100];
  char sql[100];
  char str[100];
	char type1[]="0";
	char type2[]="1";
  char * error_message;
  int database_descriptor = sqlite3_open("database", & database);;
  struct thData tdL;
  tdL = * ((struct thData * ) arg);
  while (1) {
    if (read(tdL.cl, & message, sizeof(message)) <= 0) {
      // printf("[Thread %d]\n",tdL.idThread);
      //  perror ("Eroare la read() de la client.\n");
      break;
    }

    if (strcmp(message, "quit") == 0) {
      //printf("salut");
      write(tdL.cl, "quit", sizeof("quit"));
      break;
    }

    else if (strcmp(message, "login") == 0) {
      write(tdL.cl, "login", sizeof("login"));
      read(tdL.cl, & nickname, sizeof(nickname));
      read(tdL.cl, & password, sizeof(password));
      if((strcmp(nickname,"Esti deja logat1!")==0) && (strcmp(password,"Esti deja logat2!")==0)) {
          write(tdL.cl, "Esti deja logat!", sizeof("Esti deja logat!"));
       }
      else{
      sql[0] = 0;
      str[0] = 0;
      sprintf(sql, "SELECT username FROM users WHERE username='%s';", nickname);
      database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
      if (strstr(str, nickname)) {
        sql[0] = 0;
        str[0] = 0;
        sprintf(sql, "SELECT password FROM users WHERE password='%s' AND username='%s';", password, nickname);
        database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
        if (strstr(str, password)){
					sql[0] = 0;
	        str[0] = 0;
	        sprintf(sql, "SELECT type FROM users WHERE password='%s' AND username='%s';", password, nickname);
					database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
					if(strstr(str, type1)) write(tdL.cl, "V-ati logat! Bine ai venit jucatorule!", sizeof("V-ati logat! Bine ai venit jucatorule!"));
					else if(strstr(str, type2)) write(tdL.cl, "V-ati logat! Bine ai venit administratorule!", sizeof("V-ati logat! Bine ai venit administratorule!"));
				}
        else write(tdL.cl, "Parola gresita!", sizeof("Parola gresita!"));
      }
			else write(tdL.cl, "Username gresit!", sizeof("Username gresit!"));
    }
    }

    else if (strcmp(message, "register") == 0) {
      int tip = 0;
      write(tdL.cl, "register", sizeof("register"));
      read(tdL.cl, & nickname, sizeof(nickname));
      read(tdL.cl, & password, sizeof(password));
      read(tdL.cl, & email, sizeof(email));
      if(strcmp(nickname,"Esti deja logat1!")==0 && strcmp(nickname,"Esti deja logat2!")==0 && strcmp(email,"Esti deja logat3!")==0) {
          continue;
       }
      else{
      sql[0] = 0;
      str[0] = 0;
      sprintf(sql, "INSERT INTO users (username,password,type,email) VALUES ('%s','%s',0,'%s');", nickname, password,email);
      database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
      write(tdL.cl, "V-ati inregistrat cu succes!", sizeof("V-ati inregistrat cu succes!"));
    }
    }

    else if(strcmp(message, "turneu") == 0){
        write(tdL.cl, "turneu", sizeof("turneu"));

    }

		else {
      write(tdL.cl, "Command not found!", sizeof("Command not found!"));
    }

  }
  sqlite3_close(database);
}

static void * treat(void * arg) {
  struct thData tdL;
  tdL = * ((struct thData * ) arg);
  //	printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
  fflush(stdout);
  pthread_detach(pthread_self());
  raspunde((struct thData * ) arg);
  /* am terminat cu acest client, inchidem conexiunea */
  close((intptr_t) arg);
  return (NULL);

};

int main() {

  struct sockaddr_in server; // structura folosita de server
  struct sockaddr_in from;
  int nr; //mesajul primit de trimis la client
  int socket_descriptor; //descriptorul de socket
  pthread_t th[100]; //Identificatorii thread-urilor care se vor crea
  int i = 0;

  /* crearea unui socket */
  if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("[server]Eroare la socket().\n");
    return errno;
  }

  /* utilizarea optiunii SO_REUSEADDR */
  int on = 1;
  setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, & on, sizeof(on));
  /* pregatirea structurilor de date */
  bzero( & server, sizeof(server));
  bzero( & from, sizeof(from));

  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
  server.sin_family = AF_INET;
  /* acceptam orice adresa */
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  /* utilizam un port utilizator */
  server.sin_port = htons(PORT);

  /* atasam socketul */
  if (bind(socket_descriptor, (struct sockaddr * ) & server, sizeof(struct sockaddr)) == -1) {
    perror("[server]Eroare la bind().\n");
    return errno;
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen(socket_descriptor, 2) == -1) {
    perror("[server]Eroare la listen().\n");
    return errno;
  }

  while (1) {
    int client;
    thData * td; //parametru functia executata de thread
    int length = sizeof(from);

    //printf ("[server]Asteptam la portul %d...\n",PORT);
    fflush(stdout);

    // client= malloc(sizeof(int));
    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    if ((client = accept(socket_descriptor, (struct sockaddr * ) & from, & length)) < 0) {
      perror("[server]Eroare la accept().\n");
      continue;
    }

    /* s-a realizat conexiunea, se astepta mesajul */

    // int idThread; //id-ul threadului
    // int cl; //descriptorul intors de accept

    td = (struct thData * ) malloc(sizeof(struct thData));
    td -> idThread = i++;
    td -> cl = client;

    pthread_create( & th[i], NULL, & treat, td);

  } //while

}
