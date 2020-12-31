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
#define pink_color "\x1b[35m"

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
  char haha[100];
  int type;
  int logged=0;
  char numeTurneu[100];char joc[100];char jucatori[100];char regula[100];char extragerea[100];
  char tabel[10000];
  char data[100];

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
 printf (yellow_color "       Pentru a avea acces la toate comenzile disponibile din aplicatie trebuie sa te loghezi!\n" white_color);
 printf (yellow_color "                In caz ca nu ai un cont, te poti inregistra cu usurinta.\n" white_color);
 printf (yellow_color "     Utilizand comanda help, o sa poti avea o lista cu toate comenzile pe care le poti accesa.\n" white_color);
 printf (yellow_color  "              In caz ca vei introduce o comanda gresita, vei fi atentionat.\n" white_color);
 printf (blue_color"       In caz ca introduci o comanda pentru care nu ai permisiune, vei fi atentionat .\n" white_color);
 printf (blue_color"            In caz ca doresti sa iesi din aplicatie, poti folosi comanda quit.\n");
 printf (blue_color "                       Spor la treaba.Enjoy your games!\n\n\n\n" white_color);

  while (1) {
    bzero(message,sizeof(message));
    /* citirea mesajului */
    printf("[client]Introduceti o comanda: ");
    fflush(stdout);
    scanf("%s",  message);
    fflush(stdin);

    //printf("[client] Am citit %d\n",nr);

    /* trimiterea mesajului la server */
    if (write(socket_descriptor, & message, sizeof(message)) <= 0) {
      perror("[client]Eroare la write() spre server.\n");
      return errno;
    }

    /* citirea raspunsului dat de server
       (apel blocant pina cind serverul raspunde) */
    bzero(message,sizeof(message));
    if (read(socket_descriptor, & message, sizeof(message)) < 0) {
      perror("[client]Eroare la read() de la server.\n");
      return errno;
    }

    /* afisam mesajul primit */
    bzero(nickname,sizeof(nickname));bzero(password,sizeof(password));bzero(logare,sizeof(logare));


    if (strcmp(message, "quit") == 0) {                   //comanda quit
      printf(blue_color "Ati iesit din aplicatie! O zi buna!\n"
        white_color);
      break;
    }
    else if ((strcmp(message, "login") == 0) && (logged==0) )  {              //comanda login
      write(socket_descriptor,"normal",sizeof("normal"));
      printf("Introduceti nickname-ul: ");
      fflush(stdout);
      scanf("%s",  nickname);
      fflush(stdin);

      write(socket_descriptor,  nickname, sizeof(nickname));
      printf("Introduceti parola: ");
      fflush(stdout);
      scanf("%s",  password);
      fflush(stdin);

      write(socket_descriptor,  password, sizeof(password));

      read(socket_descriptor,  logare, sizeof(logare));
      printf("%s\n", logare);

      if(strcmp(logare,"V-ati logat! Bine ai venit jucatorule!")==0 || strcmp(logare,"V-ati logat! Bine ai venit administratorule!")==0)
            logged=1;
          }

    else if ((strcmp(message, "login") == 0 || (strcmp(message, "register") == 0) ) && (logged==1) ){
      write(socket_descriptor, "nicetry", sizeof("nicetry"));
      read(socket_descriptor,  haha, sizeof(haha));
      printf("%s\n",haha);
      }


    else if ((strcmp(message, "register") == 0) && (logged==0) )  {  //comanda register
      write(socket_descriptor,"normal",sizeof("normal"));
      printf("Introduceti nickname-ul: ");
      fflush(stdout);
      scanf("%s",  nickname);
      fflush(stdin);
      write(socket_descriptor,  nickname, sizeof(nickname));

      printf("Introduceti parola: ");
      fflush(stdout);
      scanf("%s",  password);
      fflush(stdin);
      write(socket_descriptor,  password, sizeof(password));

      printf("Introduceti email-ul: ");
      fflush(stdout);
      scanf("%s",  email);
      fflush(stdin);
      write(socket_descriptor, email, sizeof(email));

      read(socket_descriptor,  logare, sizeof(logare));
      printf("%s\n", logare);


    }

    else if ((strcmp(message, "register_tournament") == 0) && (logged==1) ) {   //comanda inregistrea turneu
        write(socket_descriptor,"nicetry",sizeof("nicetry"));
        printf("Introduceti numele turneului: ");
        fflush(stdout);
        scanf("%s",  numeTurneu);
        fflush(stdin);
        write(socket_descriptor,  numeTurneu, sizeof(numeTurneu));

        printf("Introduceti numele jocului: ");
        fflush(stdout);
        scanf("%s",  joc);
        fflush(stdin);
        write(socket_descriptor,  joc, sizeof(joc));

        printf("Introduceti numarul de jucatori: ");
        fflush(stdout);
        scanf("%s",  jucatori);
        fflush(stdin);
        write(socket_descriptor, jucatori, sizeof(jucatori));

        printf("Introduceti regula: ");
        fflush(stdout);
        scanf("%s",  regula);
        fflush(stdin);
        write(socket_descriptor, regula, sizeof(regula));

        printf("Introduceti extragerea: ");
        fflush(stdout);
        scanf("%s",  extragerea);
        fflush(stdin);
        write(socket_descriptor, extragerea, sizeof(extragerea));

        read(socket_descriptor,  logare, sizeof(logare));
        printf("%s\n", logare);
        fflush(stdout);
    } else if ((strcmp(message, "register_tournament2") == 0) && (logged==1) ){
      printf("Trebuie sa fiti administrator!\n");
    } else if ((strcmp(message, "register_tournament2") == 0) && (logged==0) ){
      printf("Trebuie sa va logati pentru a folosi aceasta comanda!\n");
    }



    else if ((strcmp(message, "history") == 0) && (logged==1) ){
          write(socket_descriptor,"nicetry",sizeof("nicetry"));
          char table[10000];                                                         //comanda istoric partide
          read(socket_descriptor,&table,sizeof(table));
          printf("%s\n",table);
          fflush(stdout);

    } else if( (strcmp(message,"history2")==0) && (logged==0) ){
        printf("Trebuie sa va logati pentru a folosi aceasta comanda!\n");

    } else if( (strcmp(message,"history2")==0) && (logged==1) ){
        printf("Trebuie sa fiti administrator!\n");

    }


    else if ((strcmp(message, "promote") == 0) && (logged==1) ){
          write(socket_descriptor,"nicetry",sizeof("nicetry"));
          printf("Introduceti numele jucatorului pe care doriti sa il promovati: ");
          fflush(stdout);
          scanf("%s",  nickname);
          fflush(stdin);
          write(socket_descriptor,  nickname, sizeof(nickname));

          read(socket_descriptor,  logare, sizeof(logare));
          printf("%s\n", logare);
          fflush(stdout);
    }else if( (strcmp(message,"promote2")==0) && (logged==0) ){
        printf("Trebuie sa va logati pentru a folosi aceasta comanda!\n");

    } else if( (strcmp(message,"promote2")==0) && (logged==1) ){
        printf("Trebuie sa fiti administrator!\n");

    }



    else if ((strcmp(message, "show_tournaments") == 0) && (logged==1) ){
          //printf("salut!");
          write(socket_descriptor,"nicetry",sizeof("nicetry"));                     //comanda afisare turnee
          read(socket_descriptor,  tabel, sizeof(tabel));
          printf("%s",tabel);
          fflush(stdout);

    }  else if ((strcmp(message, "show_tournaments") == 0) && (logged==0) ){
      write(socket_descriptor,"trynice",sizeof("trynice"));
      read(socket_descriptor,  logare, sizeof(logare));
      printf("%s\n", logare);
      fflush(stdout);
    }


    else if (strcmp(message, "help") == 0){
        printf("\n\n                       Mai jos vei gasi o lista cu comenzile pe care le poti folosi:\n");
        printf("1)Comanda register:\n   -Iti permite sa iti creezi un cont in aplicatie.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, o sa fii nevoit sa introduci pe rand un username,o parola si o adesa de mail.");
        printf("\n   -Dupa ce ai introdus totul, contul a fost creat cu succes si te poti loga in siguranta.\n\n");

        printf("2)Comanda login:\n   -Iti permite sa te loghezi cu un cont existent la aplicatie.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, o sa fii nevoit sa introduci pe rand username-ul si parola de la contul tau.");
        printf("\n   -Dupa ce ai introdus totul, in caz ca datele introduse sunt corecte, te vei conecta la aplicatie si vei avea acces la anumite comenzi.");
        printf("\n   -In caz ca datele tale de logare sunt gresite, vei fi atentionat. In caz ca introduci username-ul corect dar parola gresita, vei fi atentionat. Si viceversa.\n\n");

        printf("3)Comanda register_tournament:\n   -Iti permite sa creezi un turneu la care pot participa mai multi jucatori.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, o sa fii nevoit sa introduci pe rand numele turneului, jocul, numarul de jucatori, reguli aferenta jocului si tipul de extragere.");
        printf("\n   -Dupa ce ai introdus totul, vei primi un mesaj de succes si astfel ai creat un nou turneu.");
        printf(red_color" \n   -REMINDER: Aceasta comanda este specifica doar administratorilor!\n\n"white_color);

        printf("4)Comanda history:\n   -Iti permite sa vizualizezi toate partidele care sunt deja terminate din baza de date a aplicatiei.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, se va afisa pe ecran o lista cu toate partidele deja termina, delimitate printr-un rand liber.");
        printf(red_color" \n   -REMINDER: Aceasta comanda este specifica doar administratorilor!\n\n"white_color);

        printf("5)Comanda participate_tournament:\n   -Iti permite sa te inregistrezi ca jucator la un campionat deja existent.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, o sa fii nevoit sa introduci numele campionatului la care vrei sa participi.");
        printf("\n   -Dupa ce ai introdus numele turneului, vei primi un mesaj de succes. In caz ca turneul nu exista, vei primi un mesaj de eroare care specifica ca turneul nu exista.");
        printf(red_color" \n   -REMINDER: Aceasta comanda este specifica doar jucatorilor!\n\n"white_color);

        printf("6)Comanda postpone:\n   -Iti permite sa reprogramezi o partida de joc la un campionat la care esti inscris.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, o sa fii nevoit sa introduci numele campionatului la care stii ca esti inscris si ai un meci iar apoi noua data la care vrei sa se dispute meciul.");
        printf("\n   -Dupa ce ai introdus datele, vei primi un mesaj de succes. In caz ca turneul nu exista, vei primi un mesaj de eroare care specifica ca turneul nu exista.");
        printf(red_color" \n   -REMINDER: Aceasta comanda este specifica doar jucatorilor!\n\n"white_color);

        printf("7)Comanda show_tournaments:\n   -Iti permite sa vizualizezi toate campionatele care sunt disponibile la acest moment.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, se va afisa pe ecran o lista cu toate turneele disponibile in acest moment, delimitate printr-un rand liber.");
        printf(red_color" \n   -REMINDER: Aceasta comanda este specifica atat jucatorilor cat si administratorilor!\n\n"white_color);

        printf("8)Comanda quit:\n   -Iti permite sa te deconectezi de la aplicatie.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, conexiunea dintre tine si aplicatie va fi incheiata.");
        printf(red_color" \n   -REMINDER: Aceasta comanda este specifica atat jucatorilor cat si administratorilor!\n\n"white_color);

        printf("9)Comanda help:\n   -Iti permite sa vizualizezi un meniu de ajutor.\n");// de terminat aici !!                      //comanda meniu de ajutor
        printf("   -Odata ce ai introdus comanda de la tastatura, se va afisa pe ecran o lista cu toate comenzile disponibile si informatii despre ele.");
        printf("\n   -Probabil stii despre aceasta comanda deja din meniul de inceput :D . ");
        printf(red_color" \n   -REMINDER: Aceasta comanda este specifica atat jucatorilor cat si administratorilor!\n\n"white_color);

        fflush(stdout);

    }

    else if ((strcmp(message, "participate_tournament") == 0) && (logged==1) ) {
        write(socket_descriptor,"nicetry",sizeof("nicetry"));

        printf("Introduceti numele turneului la care doriti sa participati: ");       //comanda inscriere jucator la campionat
        fflush(stdout);
        scanf("%s",  numeTurneu);
        fflush(stdin);
        write(socket_descriptor,  numeTurneu, sizeof(numeTurneu));

        read(socket_descriptor,  logare, sizeof(logare));
        printf("%s\n", logare);
        fflush(stdout);
    } else if ((strcmp(message, "participate_tournament2") == 0) && (logged==1) ){
      printf("Trebuie sa fiti jucator!\n");
    } else if ((strcmp(message, "participate_tournament") == 0) && (logged==0) ){
      write(socket_descriptor,"trynice",sizeof("trynice"));
      read(socket_descriptor,  logare, sizeof(logare));
      printf("%s\n", logare);
      fflush(stdout);
    }



    else if ((strcmp(message, "postpone") == 0) && (logged==1) ) {
        write(socket_descriptor,"nicetry",sizeof("nicetry"));

        printf("Introduceti numele turneului la care doriti sa participati: ");           //comanda amanare partida
        fflush(stdout);
        scanf("%s",  numeTurneu);
        fflush(stdin);
        write(socket_descriptor,  numeTurneu, sizeof(numeTurneu));

        printf("Introduceti noua data in care puteti sa participati: ");
        fflush(stdout);
        scanf("%s",  data);
        fflush(stdin);
        write(socket_descriptor,  data, sizeof(data));

        read(socket_descriptor,  logare, sizeof(logare));
        printf("%s\n", logare);
        fflush(stdout);
    } else if ((strcmp(message, "postpone2") == 0) && (logged==1) ){
      printf("Trebuie sa fiti jucator!\n");
    } else if ((strcmp(message, "postpone") == 0) && (logged==0) ){
      write(socket_descriptor,"trynice",sizeof("trynice"));
      read(socket_descriptor,  logare, sizeof(logare));
      printf("%s\n", logare);
      fflush(stdout);
    }







    else if (strcmp(message, "Command not found!") == 0) {
      printf(red_color "Comanda nu exista!\n"
        white_color);                                                    //cand nu se gaseste o comanda care nu gaseste
      continue;                                                             //neinregistrata
    }

  }
  /* inchidem conexiunea, am terminat */
  close(socket_descriptor);
  return 0;
}

