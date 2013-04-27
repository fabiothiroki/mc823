//=========================
// MC823 - Laboratorio de Redes de Computadores - Projeto 1
//
// Nome: Frank  RA: 070934
// Nome: Suelen RA: 072399
//=========================

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

#include "aux_functions.h"

// Identificador da porta a qual o cliente ira se conectar
#define SERVERPORT "4950"

// Numero maximo de bytes que cada resposta pode conter
#define MAXDATASIZE 4486

#define REG_SEP '\n'
#define FIELD_SEP '|'

#define NUMBER_MOVIES 13

/**
 * Lista o id e o titulo de todos os filmes
 *
 * @param response buffer preenchido com a resposta enviada pelo servidor
 */
void listAllMovies(char response[]) {
  char ** all_movies;
  char ** id_title_movie;
  int i = 0;

  printf("ID   |TITULO                   \n");

  all_movies = split(response, REG_SEP);


  for(i = 0; i < NUMBER_MOVIES; i++) {
    id_title_movie = split(all_movies[i], FIELD_SEP);
    
    printf("%5s| %25s\n", id_title_movie[0], id_title_movie[1]);

    free(id_title_movie);
  }
  free(all_movies);
  
}

/**
 * Exibe todas as informacoes de um filme que possui o numero dado
 *
 * @param response buffer preenchido com a resposta enviada pelo servidor
 */
void findMovieById(char response[]) {
  char ** movie_info;

  movie_info = split(response, FIELD_SEP);

  printf("\n\n");
  printf("DADOS DO FILME: \n\n");
  printf("NÚMERO  : %s\n", movie_info[0]);
  printf("TÍTULO  : %s\n", movie_info[1]);
  printf("SINOPSE : %s\n", movie_info[2]);
  printf("SALA    : %s\n", movie_info[3]);
  printf("HORÁRIOS: %s\n", movie_info[4]);
  printf("\n\n");

}
/**
 * Exibe todas as informacoes de todos os filmes
 *
 * @param response buffer preenchido com a resposta enviada pelo servidor
*/
void findAllMovies(char response[]) {
  char ** all_movies;
  char ** info_movie;
  int i = 0;

  all_movies = split(response, REG_SEP);

  printf("\n\n");
  printf("=====FILMES=====\n\n\n");

  for(i = 0; i < NUMBER_MOVIES; i++) {
    info_movie = split(all_movies[i], FIELD_SEP);
    printf("NÚMERO  : %s\n", info_movie[0]);
    printf("TÍTULO  : %s\n", info_movie[1]);
    printf("SINOPSE : %s\n", info_movie[2]);
    printf("SALA    : %s\n", info_movie[3]);
    printf("HORÁRIOS: %s\n", info_movie[4]);
    printf("===========================================================================\n");

    free(info_movie);
  }
  free(all_movies);
  
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
int main(int argc, char* argv[]) {
  int sockfd;  
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  int rv;
  char s[INET6_ADDRSTRLEN];
  size_t addr_len;
  
  char option = '0';          // Armazena opcao escolhida 
  char buffer[6];             // Buffer para envio de requisicao
  char response[MAXDATASIZE]; // Buffer de resposta
  char aux[3];

  int size; // armazenara o tamanho do buffer a cada requisicao
  int ativo; // boolean q indica se o loop de menu deve ficar ativo

  clock_t start, end;
  double elapsed, t1, t2;
  struct timeval tv1, tv2;

  // arquivo que armazenara os tempos de processamento de requisicao
  FILE * relatorio;
  
  if (argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }
  
  // loop through all the results and connect to the first we can
for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
        perror("talker: socket");
        continue;
    }
    break;
}

   if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }
  
  /*inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
  printf("client: connecting to %s\n", s);*/
  
  
  
  // MENU
  ativo = 1;
  while(ativo) {

    	printf("\n\n******************************************************************\n");
	printf("SERVIDOR DE FILMES:\n\n");
    	printf("1 - Exibir número e título de todos os filmes.\n");
    	printf("2 - Exibir sinopse de um filme.\n");
    	printf("3 - Exibir todas informacoes de um filme\n");
    	printf("4 - Exibir todas informacoes de todos os filmes\n");
	printf("5 - Dar nota para um filme\n");
	printf("6 - Exibir media de um filme e o numero de clientes que avaliaram\n");
    	printf("7 - Sair\n");
    	printf("**********************************************************************\n");

    scanf("%c", &option);
    buffer[0] = option;
    buffer[1] = '\0';
    
    switch ( option ) {
    case '1' :
       //Registra tempo logo apos envio da requisicao, para calcular
      //tempo total da comunicacao
      gettimeofday(&tv1, NULL);
      t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;

      size = strlen(buffer);
      
      // Exibe id e titulo de todos os filmes
      sendto(sockfd, buffer, size, 0, p->ai_addr,  p->ai_addrlen);

      recvfrom(sockfd, response, MAXDATASIZE , 0, (struct sockaddr *)&their_addr, &addr_len);

      listAllMovies(response);

      // Registra tempo apos receber requisicao processada
      gettimeofday(&tv2, NULL);
      t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
      
      // Calcula tempo gasto
      elapsed = t2 - t1;
      
      // Armazena resultado em arquivo
      relatorio = fopen("relatorio_com_1.txt","a+");
      fprintf(relatorio, "%f\n", elapsed);
      fclose(relatorio);

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
      
    case '7':
      // Envia solicitacao de encerramento de conexao
      size = strlen(buffer);
      send(sockfd, buffer, size, 0);
      ativo = 0;
      break;

    default:
      printf("Opção inválida. Digite novamente:");
      getchar();
      break;
    }
    
  }
  freeaddrinfo(servinfo); // all done with this structure
  close(sockfd);
  
  return 0;
}
