#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

// porta (efemera) de entrada dos clientes
#define PORT "49152"  
#define BACKLOG 10

#define MAXDATASIZE 5000

char ** split(char * string, char delim){

  char * pch;
  char *temp = &delim;
  char ** vector;

  vector = (char **) malloc (sizeof(char *) * MAXDATASIZE);

  pch = strtok (string,temp);
  int i = 0;
  while (pch != NULL) {
    vector[i] = pch;
    pch = strtok (NULL, temp);
    i++;
  }

  return vector;

}

typedef struct bookStruct {
  char isbn[11];        
  char title[50];
  char author[50];
  char description[300];
  char publisher[50];
  char year[4]; 
  char quantity[50];    
} Book;

Book arr_books[10];
int numberBooks;

void loadBooks(){

    sqlite3 *conn;
    sqlite3_stmt *res;
    int error = 0;
    int rec_count = 0;
    const char *errMSG;
    const char *tail;

    error = sqlite3_open("database.sqlite", &conn);

    if (error) {
        perror("Can not open database");
    }

     error = sqlite3_prepare_v2(conn, "select title,author,description,publisher,year,quantity,isbn from books order by title",1000, &res, &tail);

     int i = 0;
     while (sqlite3_step(res) == SQLITE_ROW) {

        strcpy(arr_books[i].title,sqlite3_column_text(res, 0));
        strcpy(arr_books[i].author,sqlite3_column_text(res, 1));
        strcpy(arr_books[i].description,sqlite3_column_text(res, 2));
        strcpy(arr_books[i].publisher,sqlite3_column_text(res, 3));
        strcpy(arr_books[i].year,sqlite3_column_text(res, 4));
        strcpy(arr_books[i].quantity,sqlite3_column_text(res, 5));
        strcpy(arr_books[i].isbn,sqlite3_column_text(res, 6));

        // printf("banco: %s \n",arr_books[i].isbn);

        i++;

     }

    numberBooks = i;
    sqlite3_close(conn);

}


void listarTodosLivros(int new_fd) {

    int i;

    char buffer[MAXDATASIZE];

    buffer[0] = '\0';

    char str[15];
    sprintf(str, "%d", numberBooks);
    strcat(buffer,str);
    strcat(buffer, "#");

    for(i=0; i < numberBooks; i++) {
      strcat(buffer,arr_books[i].isbn);
      strcat(buffer, "|");
      strcat(buffer,arr_books[i].title);
      strcat(buffer, "\n");
    }

    printf("buffer: %s \n",buffer);

    send(new_fd, buffer, MAXDATASIZE, 0);
}

void getBookDescByIsbn(int new_fd, char opt[]) {


    char buffer[MAXDATASIZE];
    char opt2[11];

    int i;

    for (i=0;i<10;i++) {
        opt2[i] = opt[i+1];
    }

    buffer[0] = '\0';
    for (i=0;i<numberBooks;i++) {
        if (atoi(opt2) == atoi(arr_books[i].isbn)) {
            strcat(buffer,arr_books[i].description);
        }
    }
  
    send(new_fd, buffer, MAXDATASIZE, 0);
}

void getAllInfo(int new_fd, char opt[]) {


    char buffer[MAXDATASIZE];
    char opt2[11];

    int i;

    for (i=0;i<10;i++) {
        opt2[i] = opt[i+1];
    }

    buffer[0] = '\0';
    for (i=0;i<numberBooks;i++) {
        if (atoi(opt2) == atoi(arr_books[i].isbn)) {
            strcat(buffer,arr_books[i].title);
            strcat(buffer, "|");
            strcat(buffer,arr_books[i].author);
            strcat(buffer, "|");
            strcat(buffer,arr_books[i].description);
            strcat(buffer, "|");
            strcat(buffer,arr_books[i].publisher);
            strcat(buffer, "|");
            strcat(buffer,arr_books[i].year);
            strcat(buffer, "|");
            strcat(buffer,arr_books[i].quantity);
            strcat(buffer, "\n");
        }
    }

    send(new_fd, buffer, MAXDATASIZE, 0);
}

void getAllBooksInfo (int new_fd, char opt[]) {
    char buffer[MAXDATASIZE];

    buffer[0] = '\0';
    int i;

    char str[15];
    sprintf(str, "%d", numberBooks);
    strcat(buffer,str);
    strcat(buffer, "#");

    for (i=0;i<numberBooks;i++) {
        strcat(buffer,arr_books[i].isbn);
        strcat(buffer, "|");
        strcat(buffer,arr_books[i].title);
        strcat(buffer, "|");
        strcat(buffer,arr_books[i].author);
        strcat(buffer, "|");
        strcat(buffer,arr_books[i].description);
        strcat(buffer, "|");
        strcat(buffer,arr_books[i].publisher);
        strcat(buffer, "|");
        strcat(buffer,arr_books[i].year);
        strcat(buffer, "|");
        strcat(buffer,arr_books[i].quantity);
        strcat(buffer, "\n");
    }

    send(new_fd, buffer, MAXDATASIZE, 0);
}

void getBookQuant(int new_fd, char opt[]) {


    char buffer[MAXDATASIZE];
    char opt2[11];

    int i;

    for (i=0;i<10;i++) {
        opt2[i] = opt[i+1];
    }

    buffer[0] = '\0';
    for (i=0;i<numberBooks;i++) {
        if (atoi(opt2) == atoi(arr_books[i].isbn)) {
            strcat(buffer,arr_books[i].quantity);
        }
    }
  
    send(new_fd, buffer, MAXDATASIZE, 0);
}

void setBookQuant(int new_fd, char opt[]) {
    char ** temp;
    int i;

    temp = split(opt, '|');

    
    if (atoi(temp[3]) == 1){  

        sqlite3 *conn;
        sqlite3_stmt *res;
        int error = 0;
        int rec_count = 0;
        const char *errMSG;
        const char *tail;  

        error = sqlite3_open("database.sqlite", &conn); 

        if (error) {
            perror("Can not open database");
        }

        char query[1000] = "UPDATE books SET quantity = \'";

        strcat(query, temp[2]);
        strcat(query, "\'");

        strcat(query, " WHERE isbn = \'");
        strcat(query, temp[1]);
        strcat(query, "\'");

        printf("%s\n",query);

        error = sqlite3_prepare_v2(conn, query,1000, &res, &tail);

        char* errmsg;
        sqlite3_exec(conn, "COMMIT", NULL, NULL, &errmsg);

        if(SQLITE_DONE != sqlite3_step(res))
            perror("step");
        
        sqlite3_finalize(res);
        

        // error = sqlite3_prepare_v2(conn, query,1000, &res, &tail);

        // int i = 0;
        // while (sqlite3_step(res) == SQLITE_ROW) {

        // }

        sqlite3_close(conn);                

    }
}

// Mata todos os processos zumbis
void sigchld_handler(int s) {
    pid_t pid;
    int stat; 
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        // printf("child %d terminated\n", pid); 
    }
}

int main(void) {

    // File descriptors dos sockets
    int sfd, new_fd;

    struct sigaction sa;

    struct addrinfo hints, *result  , *rp;

    socklen_t sin_size;

    int yes = 1;

    //informacao de endereco dos conectores
    struct sockaddr_storage their_addr;   

    // status da conexao com um cliente
    int connected;

    // Carrega todas as informações do livro do banco de dados
    loadBooks();

    // hints define o tipo de endereço que estamos procurando no getaddrinfo
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = AI_PASSIVE;

    /* getaddrinfo() retorna uma lista de structs contendo endereços do tipo especificado em "hints". */
    if ((getaddrinfo(NULL, PORT, &hints, &result)) != 0) {
        printf("Erro getaddrinfo\n");
        exit(0);
    }

    // Percorre todos os endereços encontrados no getaddrinfo
    // Faz o "bind" para o primeiro socket criado com sucesso
    for (rp = result; rp != NULL; rp = rp->ai_next) {

        // socket() retorna um inteiro similar a um descritor de arquivos relacionado ao socket criado, 
        // através do qual ele pode ser referenciado
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    // bind() associa uma porta para o socket
    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == -1) {
      close(sfd);
      perror("server: bind");
      continue;
    }

    break;
    }

    if (rp == NULL) {
        /* No address succeeded */              
    printf("Could not bind\n");
    exit(0);
  }

    freeaddrinfo(result);
    
    // Seta o número de conexões permitidas para o socket criado
    if (listen(sfd, BACKLOG) == -1) {
        printf("ERROR: listen\n");
        exit(0);
    }

    // trata o sinal recebido pelos processos filhos
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      printf("ERROR: sigaction");
      exit(0);
    }


    printf("SERVER -> waiting connections from clients...\n");

    // mensagem contendo o request do cliente
    // contem a opcao do cliente, id do livro e o id do usuario
    char msg[15];   

    while(1) {
        // accept(): responsável por aceitar uma conexão em socket
        sin_size = sizeof their_addr;
        new_fd = accept(sfd, (struct sockaddr *)&their_addr, &sin_size);

        if (new_fd == -1) {
          perror("ERROR: accept");
          continue;
        }

        printf("SERVER: conexao aceita\n");
        if (!fork()) {

            close(sfd); 

            // char welcome[] = "Seja bem vindo ao catalogo de livros!\n";
            // if (send(new_fd, welcome, sizeof(welcome), 0) == -1) {
            //     perror("send");
            //     exit(0);
            // }

            connected = 1;
            while(connected){

                //recv(): recebe dados em um socket
                if(recv(new_fd,msg, 20, 0) == -1) {
                    perror("ERROR: recv");
                }


                else if (*msg != '\0') {
                    printf("msg: %s \n",msg);

                    switch(msg[0]){

                        case '0':
                          // Encerra conexao
                          connected = 0;
                          break;

                        case '1':
                            // Listar ISBN e título de todos os livros
                            listarTodosLivros(new_fd);
                            break;

                        case '2':
                            //Exibir descrição de um livro
                            getBookDescByIsbn(new_fd,msg);
                            break;

                        case '3':
                            //Exibir todas informacoes de um livro
                            getAllInfo(new_fd,msg);
                            break;

                        case '4':
                            //Exibir todas informacoes de um livro
                            getAllBooksInfo(new_fd,msg);
                            break;

                        case '5':
                            //Exibir todas informacoes de um livro
                            getBookQuant(new_fd,msg);
                            break;
                        case '6':
                            //Exibir todas informacoes de um livro
                            setBookQuant(new_fd,msg);
                            break;
                        default:

                          // printf("opt: %c",msg[0]);
                          // printf("Opcao do servidor inválida\n");
                          break;
                    }

                } 

            }

            close(new_fd);
            exit(0);

        } 

        close(new_fd);

    }

    return 0;
}