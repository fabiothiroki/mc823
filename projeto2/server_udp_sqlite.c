#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>

#define PORT 49152  // porta (efemera) de entrada dos clientes

#define MAXDATASIZE 5000


char ** split(char * string, char delim){
  // Função auxiliar que recebe uma string e divide ela em arrays, dado um char separador
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
  char year[3]; 
  char horarios[18];  
  char quantity[2]; 
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

    if(error){
      printf("query");
    }

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

void listarTodosLivros(int new_fd, struct sockaddr_in their_addr, int addr_len) {
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

    sendto(new_fd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
}

void getAllBooksInfo (int new_fd, char opt[], struct sockaddr_in their_addr, int addr_len) {
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

    printf("buffer: %s \n",buffer);

    sendto(new_fd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
}


void getAllInfo(int new_fd, char opt[], struct sockaddr_in their_addr, int addr_len) {
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

    printf("%s \n",buffer);

    sendto(new_fd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
}

void getBookDescByIsbn(int new_fd, char opt[], struct sockaddr_in their_addr, int addr_len) {
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
  
  sendto(new_fd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
}


void getBookQuant(int new_fd, char opt[], struct sockaddr_in their_addr, int addr_len) {

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

    printf("%s \n",buffer);
  
    sendto(new_fd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
}

void setBookQuant(int new_fd, char opt[], struct sockaddr_in their_addr, int addr_len) {
    char ** temp;
    int i;

    printf("%s\n",opt);
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

        sqlite3_close(conn); 

        //atualiza quantidade de livros
        loadBooks();               

    }
}

int main(int argc, char * argv[]) {

  // File descriptors do socket
  int sfd;

  int numbytes;

  int yes = 1;

  char buf[MAXDATASIZE];
  size_t addr_len;
  char s[INET6_ADDRSTRLEN];
  int i=0;

  clock_t start, end;
  double elapsed, t1, t2;
  struct timeval tv1, tv2;

  // Carrega todas as informações do livro do banco de dados
  loadBooks();		               

  struct sockaddr_in si_me, si_other;
  int slen=sizeof(si_other);

  // Cria um socket do tipo UDP
  if ((sfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
    perror("socket");
  }
  else {
    printf("Server : Socket() successful\n");
  }

  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind() associa uma porta para o socket
  if (bind(sfd, (struct sockaddr* ) &si_me, sizeof(si_me))==-1)
    perror("bind");
  
  printf("Server: ligado...\n");

  // mensagem contendo o request do cliente
  // contem a opcao do cliente, id do livro e o id do usuario
  char msg[15];  

  // Arquivo onde será salvo os tempos de cada request
  FILE * relatorio;

  // status da conexao com um cliente
  int connected = 1;  
  
  while(connected){

    // Recebe a opcao do client
    if ((numbytes = recvfrom(sfd, msg, 20 , 0, (struct sockaddr*)&si_other, &slen)) == -1) {
        perror("recvfrom");
        exit(1);
    } 
    else {
	    printf("msg: %s \n",msg);
      printf("numbytes: %d \n",numbytes);

  	  switch(msg[0]){

    	  case '1':			 
          // Listar ISBN e título de todos os livros
    	    gettimeofday(&tv1, NULL);
    	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

          listarTodosLivros(sfd, si_other, slen);
    	    
    	    gettimeofday(&tv2, NULL);
    	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;

    	    elapsed = t2 - t1;
    	    
    	    relatorio = fopen("relatorio_1.txt","a+");
    	    fprintf(relatorio, "%f\n", elapsed);
    	    fclose(relatorio);

    	    break;
    	    
    	  case '2': 
          //Exibir descrição de um livro
    	    gettimeofday(&tv1, NULL);
    	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

    	    getBookDescByIsbn(sfd,msg,si_other, slen);

    	    gettimeofday(&tv2, NULL);
    	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
    	    elapsed = t2 - t1;
    	    relatorio = fopen("relatorio_2.txt","a+");
    	    fprintf(relatorio, "%f\n", elapsed);
    	    fclose(relatorio);

    	    break;
    	    
    	  case '3':
          //Exibir todas informacoes de um livro
    	    gettimeofday(&tv1, NULL);
    	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

          getAllInfo(sfd,msg,si_other, slen);

    	    gettimeofday(&tv2, NULL);
    	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
    	    elapsed = t2 - t1;
    	    relatorio = fopen("relatorio_3.txt","a+");
    	    fprintf(relatorio, "%f\n", elapsed);
    	    fclose(relatorio);
    	    break;
    	    
    	  case '4':
          //Exibir todas informacoes de um livro
    	    gettimeofday(&tv1, NULL);
    	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

    	    getAllBooksInfo(sfd,msg,si_other, slen);

    	    gettimeofday(&tv2, NULL);
    	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
    	    elapsed = t2 - t1;
    	    relatorio = fopen("relatorio_4.txt","a+");
    	    fprintf(relatorio, "%f\n", elapsed);
    	    fclose(relatorio);

    	    break;
    	    
    	  case '5':
          //Exibir a quantidade de um livro
    	    gettimeofday(&tv1, NULL);
    	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

    	    getBookQuant(sfd,msg,si_other, slen);

    	    gettimeofday(&tv2, NULL);
    	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
    	    elapsed = t2 - t1;
    	    relatorio = fopen("relatorio_5.txt","a+");
    	    fprintf(relatorio, "%f\n", elapsed);
    	    fclose(relatorio);

    	    break;
    	    
    	  case '6':
          //Alterar a quantidade de um livro
    	    gettimeofday(&tv1, NULL);
    	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

    	    setBookQuant(sfd,msg,si_other, slen);

    	    gettimeofday(&tv2, NULL);
    	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
    	    elapsed = t2 - t1;
    	    relatorio = fopen("relatorio_6.txt","a+");
    	    fprintf(relatorio, "%f\n", elapsed);
    	    fclose(relatorio);
    	    break;

    	  default:
    	    printf("opt: %c",msg[0]);
          printf("Opcao do servidor inválida\n");
    	    break;
  	  }
	  }
  }
      close(sfd);
      return 0;
}
