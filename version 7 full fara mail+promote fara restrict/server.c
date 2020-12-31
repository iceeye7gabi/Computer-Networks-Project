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



#include <string.h>

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
  char message[100];char nickname[100];char password[100];char email[100];
  char nameTournament[100];char nameGame[100]; char nbPlayers[100]; char nameRule[100]; char nameExtragere[100];
  char date[100];
  char sql[10000];
  char str[10000];
  char string[100];
	char type1[]="0";
	char type2[]="1";
  char type3[]="3";
  char tabel[10000];
  int tip_utilizator=0;
  char * error_message;
  int database_descriptor = sqlite3_open("database", & database);;
  struct thData tdL;
  tdL = * ((struct thData * ) arg);
  while (1) {
    fflush (stdout);
    memset(message,0,sizeof(message));
    if (read(tdL.cl,  message, sizeof(message)) <= 0) {
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
      memset(nickname,0,sizeof(nickname));memset(password,0,sizeof(password));
      read(tdL.cl,string,sizeof(string));
      if((strcmp(string,"nicetry")==0)) {
          write(tdL.cl, "Esti deja logat!", sizeof("Esti deja logat!"));
       }
       else{
      read(tdL.cl,  nickname, sizeof(nickname));
      read(tdL.cl,  password, sizeof(password));
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
					else if(strstr(str, type2)){
             write(tdL.cl, "V-ati logat! Bine ai venit administratorule!", sizeof("V-ati logat! Bine ai venit administratorule!"));
             tip_utilizator=1;
            }

        }
        else write(tdL.cl, "Parola gresita!", sizeof("Parola gresita!"));
      }
			else write(tdL.cl, "Username gresit!", sizeof("Username gresit!"));
    }

    fflush (stdout);
    memset(message,0,sizeof(message));
    }

    else if (strcmp(message, "register") == 0) {
      int tip = 0;
      write(tdL.cl, "register", sizeof("register"));
      memset(nickname,0,sizeof(nickname));memset(password,0,sizeof(password));memset(email,0,sizeof(email));
      read(tdL.cl,string,sizeof(string));
      if((strcmp(string,"nicetry")==0)) {
          write(tdL.cl, "Esti deja logat!", sizeof("Esti deja logat!"));
       }
       else{
      read(tdL.cl,  nickname, sizeof(nickname));
      read(tdL.cl,  password, sizeof(password));
      read(tdL.cl,  email, sizeof(email));
      sql[0] = 0;
      str[0] = 0;
      sprintf(sql, "INSERT INTO users (username,password,type,email) VALUES ('%s','%s',0,'%s');", nickname, password,email);
      database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
      write(tdL.cl, "V-ati inregistrat cu succes!", sizeof("V-ati inregistrat cu succes!"));
      fflush (stdout);
      memset(message,0,sizeof(message));
    }
    }



    else if( (strcmp(message, "register_tournament") == 0) && (tip_utilizator==1) ) {
        write(tdL.cl, "register_tournament", sizeof("register_tournament"));
        memset(nameTournament,0,sizeof(nameTournament));memset(nameGame,0,sizeof(nameGame));memset(nbPlayers,0,sizeof(nbPlayers));
        memset(nameRule,0,sizeof(nameRule));memset(nameExtragere,0,sizeof(nameExtragere));
        read(tdL.cl,string,sizeof(string));
        if((strcmp(string,"nicetry")==0) ) {
          read(tdL.cl,  nameTournament, sizeof(nameTournament));
          read(tdL.cl,  nameGame, sizeof(nameGame));
          read(tdL.cl,  nbPlayers, sizeof(nbPlayers));
          read(tdL.cl,  nameRule, sizeof(nameRule));
          read(tdL.cl,  nameExtragere, sizeof(nameExtragere));
          sql[0] = 0;
          str[0] = 0;
          sprintf(sql, "INSERT INTO tournaments (Nume_turneu,Game,Players,Rule,Extragere) VALUES ('%s','%s','%s','%s','%s');", nameTournament, nameGame,nbPlayers,nameRule,nameExtragere);
          database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
          write(tdL.cl, "Ati creat turneul cu succes!", sizeof("Ati creat turneul cu succes!"));
          fflush (stdout);
          memset(message,0,sizeof(message));
         }
         else if((strcmp(string,"trynice")==0)){
        write(tdL.cl, "Trebuie sa fie logat!", sizeof("Trebuie sa fie logat!"));
      }

    } else if( (strcmp(message, "register_tournament") == 0) && (tip_utilizator==0) ) {  write(tdL.cl, "register_tournament2", sizeof("register_tournament2"));}


    else if( (strcmp(message, "history") == 0) && (tip_utilizator==1) ) {
          write(tdL.cl, "history", sizeof("history"));
          read(tdL.cl,string,sizeof(string));
          if(strcmp(string,"nicetry")==0){
            sql[0] = 0;
            str[0] = 0;
            sprintf(sql,"select * from istoricpartide;");
            database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
            char string2[10000];
            sprintf(string2,"%s",str);
          //  printf("%s",string2);
            write(tdL.cl, string2, sizeof(string2));
          } else if(strcmp(string,"trynice")==0) {
            write(tdL.cl, "Trebuie sa fii logat!", sizeof("Trebuie sa fii logat!"));
          }
        }

        else if( (strcmp(message, "promote") == 0) && (tip_utilizator==1) ) {
            write(tdL.cl, "promote", sizeof("promote"));
            read(tdL.cl,string,sizeof(string));
            if(strcmp(string,"nicetry")==0){
              read(tdL.cl,  nickname, sizeof(nickname));
              sql[0] = 0;
              str[0] = 0;
              sprintf(sql,"UPDATE users SET type='1' WHERE username='%s';",nickname);
              database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
              write(tdL.cl, "Ati promovat cu succes utilizatorul!", sizeof("Ati promovat cu succes utilizatorul!"));
              fflush (stdout);
              memset(message,0,sizeof(message));
        }else if(strcmp(string,"trynice")==0) {
          write(tdL.cl, "Trebuie sa fii logat!", sizeof("Trebuie sa fii logat!"));
        }
      }
     else if( (strcmp(message, "promote") == 0) && (tip_utilizator==0) ) {  write(tdL.cl, "promote2", sizeof("promote2"));}





    else if( (strcmp(message, "show_tournaments") == 0) && ((tip_utilizator==1) || (tip_utilizator==0) ) ) {
          write(tdL.cl, "show_tournaments", sizeof("show_tournaments"));
          read(tdL.cl,string,sizeof(string));
          memset(tabel,0,sizeof(tabel));
          if((strcmp(string,"nicetry")==0) ) {
          sql[0] = 0;
          str[0] = 0;
          sprintf(sql, "select * from tournaments;");
          database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
          char string2[10000];
          sprintf(string2,"%s",str);
        //  printf("%s",string2);
          write(tdL.cl,string2,sizeof(string2));
      }
      else if((strcmp(string,"trynice")==0)){
     write(tdL.cl, "Trebuie sa va logati pentru a folosi aceasta comanda!", sizeof("Trebuie sa va logati pentru a folosi aceasta comanda!"));
   }
    }



    else if( (strcmp(message, "participate_tournament") == 0) && (tip_utilizator==0)   ) {
        write(tdL.cl, "participate_tournament", sizeof("participate_tournament"));
        read(tdL.cl,string,sizeof(string));
        memset(nameTournament,0,sizeof(nameTournament));
        if((strcmp(string,"nicetry")==0) ) {
          read(tdL.cl,  nameTournament, sizeof(nameTournament));
          sql[0] = 0;
          str[0] = 0;
          sprintf(sql, "SELECT * FROM tournaments WHERE Nume_turneu='%s';", nameTournament);
          database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
          str[strlen (str)-1]=0;
        //  printf("%s",str);
          if(strlen(str)>2){
            sql[0] = 0;
            str[0] = 0;
            sprintf(sql, "INSERT INTO usersregistered (username,numeTournament) VALUES ('%s','%s');", nickname, nameTournament);
            database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
            write(tdL.cl, "Sunteti inscris!", sizeof("Sunteti inscris!"));
        }
          else write(tdL.cl, "Ne pare rau dar campionatul la care v-ati inscris nu exista!", sizeof("Ne pare rau dar campionatul la care v-ati inscris nu exista!"));
          fflush (stdout);
          memset(message,0,sizeof(message));
         }
         else if((strcmp(string,"trynice")==0)){
        write(tdL.cl, "Trebuie sa va logati pentru a folosi aceasta comanda!", sizeof("Trebuie sa va logati pentru a folosi aceasta comanda!"));
      }

    } else if( (strcmp(message, "participate_tournament") == 0) && (tip_utilizator==1) ) {  write(tdL.cl, "participate_tournament2", sizeof("participate_tournament2"));}










    else if( (strcmp(message, "postpone") == 0) && (tip_utilizator==0)   ) {
        write(tdL.cl, "postpone", sizeof("postpone"));
        read(tdL.cl,string,sizeof(string));
        memset(nameTournament,0,sizeof(nameTournament));
        if((strcmp(string,"nicetry")==0) ) {
          read(tdL.cl,  nameTournament, sizeof(nameTournament));
          sql[0] = 0;
          str[0] = 0;
          sprintf(sql, "SELECT * FROM tournaments WHERE Nume_turneu='%s';", nameTournament);
          database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
          str[strlen (str)-1]=0;
        //  printf("%s",str);
          if(strlen(str)>2){
            read(tdL.cl,  date, sizeof(date));
            sql[0] = 0;
            str[0] = 0;
            sprintf(sql, "UPDATE partidecurente SET Data='%s' WHERE Turneu='%s' AND ( Player1='%s' OR Player2='%s' );", date,nameTournament,nickname,nickname);
            database_descriptor = sqlite3_exec(database, sql, callback, str, & error_message);
            write(tdL.cl, "Ati modificat cu succes data partidei!", sizeof("Ati modificat cu succes data partidei!"));
        }
          else write(tdL.cl, "Ne pare rau dar campionatul la care v-ati inscris nu exista!", sizeof("Ne pare rau dar campionatul la care v-ati inscris nu exista!"));
          fflush (stdout);
          memset(message,0,sizeof(message));
         }
         else if((strcmp(string,"trynice")==0)){
        write(tdL.cl, "Trebuie sa va logati pentru a folosi aceasta comanda!", sizeof("Trebuie sa va logati pentru a folosi aceasta comanda!"));
      }

    } else if( (strcmp(message, "postpone") == 0) && (tip_utilizator==1) ) {  write(tdL.cl, "postpone2", sizeof("postpone"));}





    else if (strcmp(message, "help") == 0) {
          write(tdL.cl, "help", sizeof("help"));
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
