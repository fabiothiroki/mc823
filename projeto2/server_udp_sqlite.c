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

#define PORT "49152"  // porta (efemera) de entrada dos clientes
#define BACKLOG 10

#define MAXDATASIZE 5000

#define FIELD_SEP "|"
#define REG_SEP "\n"
#define END_STRING '\0'

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

void listarTodosLivros(int new_fd, struct sockaddr_storage their_addr, int addr_len) {
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

void getAllBooksInfo (int new_fd, struct sockaddr_storage their_addr, int addr_len) {
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

    sendto(new_fd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
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

    sendto(sockfd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
}

void getMovieById(int sockfd, char opt[], struct sockaddr_storage their_addr, int addr_len) {
  char buffer[MAXDATASIZE];
  char opt2[3];
  int index_movie;
    
  opt2[0] = opt[1];
  opt2[1] = opt[2];
  opt2[2] = END_STRING;

  index_movie = atoi(opt2) - 1;

  buffer[0] = END_STRING;

  strcat(buffer, filmes[index_movie].id);
  strcat(buffer, FIELD_SEP);
  strcat(buffer, filmes[index_movie].titulo);
  strcat(buffer, FIELD_SEP);
  strcat(buffer, filmes[index_movie].sinopse);
  strcat(buffer, FIELD_SEP);
  strcat(buffer, filmes[index_movie].sala);
  strcat(buffer, FIELD_SEP);
  strcat(buffer, filmes[index_movie].horarios);
  
  sendto(sockfd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
}

void getBookDescByIsbn(int new_fd, char opt[], struct sockaddr_storage their_addr, int addr_len) {
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
  
  sendto(sockfd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
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
  
    sendto(sockfd, buffer, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, addr_len);
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


        sqlite3_close(conn);                

    }
}


/**
 * Define se o sockaddr é IPv4 ou IPv6
 *
 * @param sa socket
 */
void *get_in_addr(struct sockaddr *sa) {

    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * Principal
 */
int main(int argc, char * argv[]) {
  int sockfd;	
  struct addrinfo hints, *servinfo, *p;
  int rv, numbytes;
  //informacao de endereco dos conectores
  struct sockaddr_storage their_addr;
  char buf[MAXBUFLEN];
  size_t addr_len;
  char s[INET6_ADDRSTRLEN];
  int i=0;
  int ativo; // Booleano que indica se a conexao deve continuar ativa

  clock_t start, end;
  double elapsed, t1, t2;
  struct timeval tv1, tv2;

  // arquivo que armazenara os tempos de processamento de requisicao
  FILE * relatorio;
  
  // Vetor que contera a opcao do cliente (mais o id do filme e a nota dada ao mesmo, se for o
  // caso)
  char opt[6];		                

  // Array que armazenara cada linha do arquivo de filmes, para tratar
  // cada campos posteriormente
  char * lineRead;

  // Matriz onde cada posicao equivale a um campo de um registro do
  // arquivo de filmes
  char ** matrixConfig;
  
  // Carrega todos os filmes contidos no arquivo 'movies.txt' para a
  // memoria (total de 13 filmes)
  FILE *arqFilmes = NULL;

  arqFilmes = fopen("movies.txt","r");
  
  // Le a primeira linha do arquivo de filmes
  lineRead = readFileBySeparator(arqFilmes, '\n');
 
  // Para cada linha do arquivo (que corresponde a um filme) separa os
  // campos e usa cada um deles para preencher a estrutura de filmes
  // adequadamente
  while(lineRead != NULL) {
    
    matrixConfig = split(lineRead, '|');

    strcpy(filmes[i].id, matrixConfig[0]);
    strcpy(filmes[i].titulo, matrixConfig[1]);
    strcpy(filmes[i].sinopse, matrixConfig[2]);
    strcpy(filmes[i].sala, matrixConfig[3]);
    strcpy(filmes[i].horarios, matrixConfig[4]);
    i++;

    free(matrixConfig);

    lineRead = readFileBySeparator(arqFilmes, '\n');
   }

  free(lineRead);

  fclose(arqFilmes);
  //fim do carregamento de filmes
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }
  
  // Percorre a lista ligada e realiza 'bind' ao primeiro que for
  // possivel
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
			 p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("listener: bind");
      continue;
    }
    
    break;
  }
  
  if (p == NULL)  {
    fprintf(stderr, "listener: falha ao realizar 'bind'\n");
    return 2;
  }
  
  // Necessario devido à chamada 'getaddrinfo' acima
  freeaddrinfo(servinfo); 
  
  printf("listener: esperando chamada 'recvfrom'...\n");

  

  /*
  printf("listener: got packet from %s\n",
      inet_ntop(their_addr.ss_family,
          get_in_addr((struct sockaddr *)&their_addr),
          s, sizeof s));
  printf("listener: packet is %d bytes long\n", numbytes);
  buf[numbytes] = '\0';
  printf("listener: packet contains \"%s\"\n", buf);
  */
  
  

  return 0;
  
  ativo = 1;
  while(ativo){
	
	// Recebe a opcao do client
	addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, opt, MAXBUFLEN , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    } 
    else {
	  
	  switch(opt[0]){
	  case '1':			 
	    // Registra tempo antes de processar a requisicao	    
	    gettimeofday(&tv1, NULL);
	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

	    // Listar todos os Ids dos filmes com seus respectivos
	    // titulos
	    getAllMovieTitles(sockfd, their_addr, addr_len); 
	    
	    // Registra tempo apos processar requisicao
	    gettimeofday(&tv2, NULL);
	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;

	    // Calcula tempo gasto
	    elapsed = t2 - t1;
	    
	    // Armazena resultado em arquivo
	    relatorio = fopen("relatorio_1.txt","a+");
	    fprintf(relatorio, "%f\n", elapsed);
	    fclose(relatorio);

	    break;
	    
	  case '2': 
	    // Registra tempo antes de processar a requisicao	    
	    gettimeofday(&tv1, NULL);
	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

	    // Dado o Id de um filme, retornar a sinopse
	    getMovieSynById(sockfd, opt, their_addr, addr_len);  

	    // Registra tempo apos processar requisicao
	    gettimeofday(&tv2, NULL);
	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;

	    // Calcula tempo gasto
	    elapsed = t2 - t1;
	    
	    // Armazena resultado em arquivo
	    relatorio = fopen("relatorio_2.txt","a+");
	    fprintf(relatorio, "%f\n", elapsed);
	    fclose(relatorio);

	    break;
	    
	  case '3':
	    // Registra tempo antes de processar a requisicao	    
	    gettimeofday(&tv1, NULL);
	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

	    // Dado o Id de um filme, retornar todas as informações
	    // desse filme
	    getMovieById(sockfd, opt, their_addr, addr_len);

	    // Registra tempo apos processar requisicao
	    gettimeofday(&tv2, NULL);
	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;

	    // Calcula tempo gasto
	    elapsed = t2 - t1;
	    
	    // Armazena resultado em arquivo
	    relatorio = fopen("relatorio_3.txt","a+");
	    fprintf(relatorio, "%f\n", elapsed);
	    fclose(relatorio);
	    break;
	    
	  case '4':
	     // Registra tempo antes de processar a requisicao	    
	    gettimeofday(&tv1, NULL);
	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

	    // Listar todas as informações de todos os filmes;			  
	    getAllMovies(sockfd, their_addr, addr_len);

	    // Registra tempo apos processar requisicao
	    gettimeofday(&tv2, NULL);
	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;

	    // Calcula tempo gasto
	    elapsed = t2 - t1;
	    
	    // Armazena resultado em arquivo
	    relatorio = fopen("relatorio_4.txt","a+");
	    fprintf(relatorio, "%f\n", elapsed);
	    fclose(relatorio);

	    break;
	    
	  case '5':
	     // Registra tempo antes de processar a requisicao	    
	    gettimeofday(&tv1, NULL);
	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

	    // Dar nota a um filme e atualizar sua media		  
	    rateMovieById(sockfd, opt, their_addr, addr_len);

	    // Registra tempo apos processar requisicao
	    gettimeofday(&tv2, NULL);
	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;

	    // Calcula tempo gasto
	    elapsed = t2 - t1;
	    
	    // Armazena resultado em arquivo
	    relatorio = fopen("relatorio_5.txt","a+");
	    fprintf(relatorio, "%f\n", elapsed);
	    fclose(relatorio);

	    break;
	    
	  case '6':
	     // Registra tempo antes de processar a requisicao	    
	    gettimeofday(&tv1, NULL);
	    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

	    // Retorna a nota de um filme e quantos clientes votaram		  
	    getRatingById(sockfd, opt, their_addr, addr_len);

	    // Registra tempo apos processar requisicao
	    gettimeofday(&tv2, NULL);
	    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;

	    // Calcula tempo gasto
	    elapsed = t2 - t1;
	    
	    // Armazena resultado em arquivo
	    relatorio = fopen("relatorio_6.txt","a+");
	    fprintf(relatorio, "%f\n", elapsed);
	    fclose(relatorio);

	    break;
	    
	  case '7':
	    // Finaliza conexao
	    ativo = 0;
	    break;
	  default:
	    printf("Opcao nao valida %c. Tente novamente\n", opt[0]);
	    break;
	  }
	  
	}
  }
      close(sockfd);
      return 0;
}
