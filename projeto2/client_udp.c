#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#include <arpa/inet.h>


#define PORT 49152

// Numero maximo de bytes que cada resposta pode conter
#define MAXDATASIZE 5000

#define REG_SEP '\n'
#define FIELD_SEP '|'

#define NUMBER_MOVIES 13

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

void printMenu(int isClientLibrary) {
  printf("\n\n******************************************************\n");
  printf("Catálogo de livros! Entre com uma das opções abaixo e pressione ENTER\n\n");
  printf("p - Imprimir esse menu\n");
  printf("0 - Sair\n");
  printf("1 - Listar ISBN e título de todos os livros\n");
  printf("2 - Exibir descrição de um livro\n");
  printf("3 - Exibir todas informacoes de um livro\n");
  printf("4 - Exibir todas informacoes de todos os livros\n");
  printf("5 - Exibir a quantidade de um livro\n");
  if (isClientLibrary) {
    printf("6 - Alterar a quantidade de um livro\n");
  }
  printf("******************************************************\n");
}

void listAllBooks(char response[]) { 
  char ** temp;
  char ** all_books;
  char ** id_isbn_title;
  int i = 0;

  if (*response != '\0') { 

    temp = split(response, '#');

    int len = atoi(temp[0]); 
    all_books = split(temp[1], '\n');


    for(i = 0; i < len; i++) {
      id_isbn_title = split(all_books[i], '|');
      
      printf("%s | %s\n", id_isbn_title[0], id_isbn_title[1]);

      free(id_isbn_title);
    }
    free(all_books);
    free(temp);

  }
  
}

void listAllBooksInfo (char response[]) { 
  char ** temp;
  char ** all_books;
  char ** all_info;
  int i = 0;

  temp = split(response, '#');

  int len = atoi(temp[0]); 
  all_books = split(temp[1], '\n');

  for(i = 0; i < len; i++) {
    all_info = split(all_books[i], '|');
    
    printf("%s | %s | %s | %s | %s | %s\n\n", all_info[0], all_info[1], all_info[2], all_info[3], all_info[4], all_info[5],all_info[6]);

    free(all_info);
  }
  free(all_books);
  free(temp);
}


void showBookDesc(char response[]){

  printf("%s \n", response);

}

int main(int argc, char* argv[]) {
  // File descriptor do socket
  int sfd;  
  struct addrinfo hints, *result, *rp;
  int rv;
  char s[INET6_ADDRSTRLEN];
  int isClientLibrary = 0;

  char response[MAXDATASIZE]; // Buffer de resposta

  //informacao de endereco dos conectores
  struct sockaddr_storage their_addr;

  size_t addr_len;
  

  clock_t start, end;
  double elapsed, t1, t2;
  struct timeval tv1, tv2;

  // arquivo que armazenara os tempos de processamento de requisicao
  FILE * relatorio;
  
  if (argc <= 2) {
      fprintf(stderr,"usage: client hostname usertype\n");
      exit(1);
  }

  if (argc == 3) {
    if ((strcmp(argv[2],"library") == 0)) {
      isClientLibrary = 1;
    }
  }
  
  /*
  // hints define o tipo de endereço que estamos procurando no getaddrinfo
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  
  // getaddrinfo() retorna uma lista de structs contendo endereços do tipo especificado em "hints". 
  if ((getaddrinfo(argv[1], SERVERPORT, &hints, &result)) != 0) {
      perror("Erro getaddrinfo\n");
      exit(0);
  }
  
  // Percorre todos os endereços encontrados no getaddrinfo
  // Faz o "bind" para o primeiro socket criado com sucesso
  for (rp = result; rp != NULL; rp = rp->ai_next) {

      // socket() retorna um inteiro similar a um descritor de arquivos relacionado ao socket criado, 
      // através do qual ele pode ser referenciado
      sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (sfd == -1){
          perror("client: socket");
          continue;
      }

      if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == -1) {
          close(sfd);
          perror("client: connect");
          continue;    
      }

      break;
  }

  if (rp == NULL) {
       No address succeeded             
      printf("Could not bind\n");
      exit(0);
  }*/

  struct sockaddr_in serv_addr;
  int slen=sizeof(serv_addr);

  if ((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
    perror("socket");
  }
  else {
    printf("Client : Socket() successful\n");
  }

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  if (inet_aton(argv[1], &serv_addr.sin_addr)==0) {
      fprintf(stderr, "inet_aton() failed\n");
      exit(1);
  }

  printMenu(isClientLibrary);
  
  int connected = 1;
  char option[1]; // Armazena opcao escolhida 
  char buffer[20]; // Buffer para envio de requisicao
  char isbn[10];
  int buffersize = strlen(buffer);
  while(connected) {

    scanf("%s", option);

    buffer[0] = (option[0]);
    buffer[1] = '\0';

    printf("buffer: %s \n",buffer);
    
    switch ( buffer[0] ) {
      // Sair
      case '0' :
        // size = strlen(buffer);
        // send(sockfd, buffer, size, 0);
        // ativo = 0;
        // break;

        if (sendto(sfd, buffer, 1, 0, (struct sockaddr*)&serv_addr, slen)==-1)
          perror("sendto()");

        connected = 0;
        break;

      /*
      case '1' :

      gettimeofday(&tv1, NULL);
      t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

      size = strlen(buffer);
      
      // Exibe id e titulo de todos os filmes
      sendto(sockfd, buffer, size, 0, p->ai_addr,  p->ai_addrlen);

      recvfrom(sockfd, response, MAXDATASIZE , 0, (struct sockaddr *)&their_addr, &addr_len);

      listAllBooks(response);

      // Registra tempo apos receber requisicao processada
      gettimeofday(&tv2, NULL);
      t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
      
      // Calcula tempo gasto
      elapsed = t2 - t1;
      
      // relatorio = fopen("relatorio_com_1.txt","a+");
      // fprintf(relatorio, "%f\n", elapsed);
      // fclose(relatorio);

      getchar();
      break;
      
    case '2': 
      // Exibe a sinopse do filme
      printf("Digite o número do filme: ");
      scanf("%s", aux);
      strcat(buffer,aux);
      buffer[3] = '\0';

      //Registra tempo logo apos envio da requisicao, para calcular
      //tempo total da comunicacao
      gettimeofday(&tv1, NULL);
      t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

      size = strlen(buffer);

      sendto(sockfd, buffer, size, 0, p->ai_addr,  p->ai_addrlen);
      recvfrom(sockfd, response, MAXDATASIZE , 0, (struct sockaddr *)&their_addr, &addr_len);
	
      printf("\n=======================================================\n");
      printf("\n\nSINOPSE: %s\n", response);
      printf("\n=======================================================\n");

      // Registra tempo apos receber requisicao processada
      gettimeofday(&tv2, NULL);
      t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
      
      // Calcula tempo gasto
      elapsed = t2 - t1;
      
      // Armazena resultado em arquivo
      relatorio = fopen("relatorio_com_2.txt","a+");
      fprintf(relatorio, "%f\n", elapsed);
      fclose(relatorio);

      getchar();
      break;
      
    case '3': 
      // Exibe todas as informacoes de um filme
      printf("Digite o número do filme: ");
      scanf("%s", aux);
      strcat(buffer,aux);
      buffer[3] = '\0';

      //Registra tempo logo apos envio da requisicao, para calcular
      //tempo total da comunicacao
      gettimeofday(&tv1, NULL);
      t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;
            
      size = strlen(buffer);

      sendto(sockfd, buffer, size, 0, p->ai_addr,  p->ai_addrlen);
      recvfrom(sockfd, response, MAXDATASIZE , 0, (struct sockaddr *)&their_addr, &addr_len);

	 findMovieById(response);

      // Registra tempo apos receber requisicao processada
      gettimeofday(&tv2, NULL);
      t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
      
      // Calcula tempo gasto
      elapsed = t2 - t1;
      
      // Armazena resultado em arquivo
      relatorio = fopen("relatorio_com_3.txt","a+");
      fprintf(relatorio, "%f\n", elapsed);
      fclose(relatorio);
      getchar();
      break;
      
    case '4':
       //Registra tempo logo apos envio da requisicao, para calcular
      //tempo total da comunicacao
      gettimeofday(&tv1, NULL);
      t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

      
      // Exibe todas as informacoes de todos os filmes
      size = strlen(buffer);

     sendto(sockfd, buffer, size, 0, p->ai_addr,  p->ai_addrlen);
      recvfrom(sockfd, response, MAXDATASIZE , 0, (struct sockaddr *)&their_addr, &addr_len);

      findAllMovies(response);

      // Registra tempo apos receber requisicao processada
      gettimeofday(&tv2, NULL);
      t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
      
      // Calcula tempo gasto
      elapsed = t2 - t1;
      
      // Armazena resultado em arquivo
      relatorio = fopen("relatorio_com_4.txt","a+");
      fprintf(relatorio, "%f\n", elapsed);
      fclose(relatorio);
      getchar();
      break;

	case '5':
	 // Exibe todas as informacoes de um filme
      printf("Digite o número do filme: ");
      scanf("%s", aux);
      strcat(buffer,aux);

	printf("Digite a nota:");
	 scanf("%s", aux);
	 strcat(buffer,aux);
      	buffer[5] = '\0';
       //Registra tempo logo apos envio da requisicao, para calcular
      //tempo total da comunicacao
      gettimeofday(&tv1, NULL);
      t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

      
      // Exibe todas as informacoes de todos os filmes
      size = strlen(buffer);

      sendto(sockfd, buffer, size, 0, p->ai_addr,  p->ai_addrlen);
      recvfrom(sockfd, response, MAXDATASIZE , 0, (struct sockaddr *)&their_addr, &addr_len);

	
	if(response[0] == '1') {
		printf("Nota enviada e processada com sucesso!\n");
	}
      
      // Registra tempo apos receber requisicao processada
      gettimeofday(&tv2, NULL);
      t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
      
      // Calcula tempo gasto
      elapsed = t2 - t1;
      
      // Armazena resultado em arquivo
      relatorio = fopen("relatorio_com_4.txt","a+");
      fprintf(relatorio, "%f\n", elapsed);
      fclose(relatorio);
      getchar();
      break;

	case '6':
	 printf("Digite o número do filme: ");
	      scanf("%s", aux);
	      strcat(buffer,aux);

       //Registra tempo logo apos envio da requisicao, para calcular
      //tempo total da comunicacao
      gettimeofday(&tv1, NULL);
      t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

      
      // Exibe todas as informacoes de todos os filmes
      size = strlen(buffer);

     sendto(sockfd, buffer, size, 0, p->ai_addr,  p->ai_addrlen);
      recvfrom(sockfd, response, MAXDATASIZE , 0, (struct sockaddr *)&their_addr, &addr_len);

	printf("%s\n", response);
      

      // Registra tempo apos receber requisicao processada
      gettimeofday(&tv2, NULL);
      t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
      
      // Calcula tempo gasto
      elapsed = t2 - t1;
      
      // Armazena resultado em arquivo
      relatorio = fopen("relatorio_com_4.txt","a+");
      fprintf(relatorio, "%f\n", elapsed);
      fclose(relatorio);
      getchar();
      break;
    */
    default:
      printf("Opção inválida. Digite novamente:");
      getchar();
      break;
    }
    
  }
  freeaddrinfo(result); // all done with this structure
  close(sfd);
  
  return 0;
}
