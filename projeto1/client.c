//=========================
// MC823 - Laboratorio de Redes de Computadores - Projeto 1
//
// Nome: Davi   RA: 097464
// Nome: Fabio  RA: 073048
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

#include <arpa/inet.h>

#include "aux_functions.h"

// Porta efêmera que o cliente usará para se conectar
#define PORT "49152" 

// Numero maximo de bytes que cada resposta pode conter
#define MAXDATASIZE 4486

#define REG_SEP '\n'
#define FIELD_SEP '|'

#define NUMBER_MOVIES 13

void printMenu(bool isClientLibrary) {
  printf("\n\n******************************************************\n");
  printf("Catálogo de livros! Entre com uma das opções abaixo e pressione ENTER\n\n");
  printf("0 - Sair\n");
  printf("1 - Listar ISBN e título de todos os livros\n");
  printf("2 - Exibir descrição de um livro\n");
  printf("3 - Exibir todas informacoes de um livro\n");
  printf("4 - Exibir todas informacoes de todos os livros\n");
  printf("5 - Exibir a quantidade de um livro");
  if (isClientLibrary) {
    printf("6 - Alterar a quantidade de um livro");
  }
  printf("******************************************************\n");
}

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

int main(int argc, char* argv[]) {
    // File descriptor do socket
    int sfd;  
    struct addrinfo hints, *result, *rp;
    int rv;
    char s[INET6_ADDRSTRLEN];

    char option = '0';          // Armazena opcao escolhida 
    char buffer[6];             // Buffer para envio de requisicao
    char response[MAXDATASIZE]; // Buffer de resposta
    char aux[3];

    int ativo;

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }
  
    // hints define o tipo de endereço que estamos procurando no getaddrinfo
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */

    /* getaddrinfo() retorna uma lista de structs contendo endereços do tipo especificado em "hints". */
    if ((getaddrinfo(argv[1], PORT, &hints, &result)) != 0) {
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

        // if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            // break;                  /* Success */

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == -1) {
            close(sfd);
            perror("client: connect");
            continue;    
        }

        break;
    }

    if (rp == NULL) {
        /* No address succeeded */              
        printf("Could not bind\n");
        exit(0);
    }

    freeaddrinfo(result); // all done with this structure
    
    char buf[100]; int numbytes;

    if ((numbytes=recv(sfd, buf, 100, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    // welcome message
    printf("%s \n", buf);

    int connected = 1;
    while (connected) {
        
        printMenu(0);

        scanf("%c", &option);
        buffer[0] = option;
        buffer[1] = '\0';

        switch ( option ) {

          // Sair
          case '0' :
            send(sfd, buffer, 6, 0);
            connected = 0;
            break;

          //Listar ISBN e título de todos os livros
          case '1' :

          break;

          //Exibir descrição de um livro
          case '2' :

          break;

          //Exibir todas informacoes de um livro
          case '3' :

          break;

          //Exibir todas informacoes de todos os livros
          case '4' :

          break;

          //Exibir a quantidade de um livro
          case '5' :

          break;

          //Alterar a quantidade de um livro
          case '6' :

          break;

          default:
            printf("Opção inválida");
            getchar();
            break;
          }
        }
        close(sfd);

        return 0;
    }

      
  ativo = 0;
  while(ativo) {

    scanf("%c", &option);
    buffer[0] = option;
    buffer[1] = '\0';

    switch ( option ) {
    case '1' :
      // Exibe id e titulo de todos os filmes
      send(sfd, buffer, 6, 0);
      recv(sfd, response, MAXDATASIZE, 0);
      listAllMovies(response);
      getchar();
      break;
      
    case '2': 
      // Exibe a sinopse do filme
      printf("Digite o número do filme: ");
      scanf("%s", aux);
      strcat(buffer,aux);
      send(sfd, buffer, 6, 0);
      recv(sfd, response, MAXDATASIZE, 0);
      printf("\n=======================================================\n");
      printf("\n\nSINOPSE: %s\n", response);
      printf("\n=======================================================\n");
      getchar();
      break;
      
    case '3': 
      // Exibe todas as informacoes de um filme
      printf("Digite o número do filme: ");
      scanf("%s", aux);
      strcat(buffer,aux);
      send(sfd, buffer, 6, 0);
      recv(sfd, response, MAXDATASIZE, 0);
      findMovieById(response);
      getchar();
      break;
      
    case '4':
      // Exibe todas as informacoes de todos os filmes
      send(sfd, buffer, 6, 0);
      recv(sfd, response, MAXDATASIZE, 0);
      findAllMovies(response);
      getchar();
      break;
      
    case '5':
      // Envia solicitacao de encerramento de conexao
      send(sfd, buffer, 6, 0);
      ativo = 0;
      break;

    default:
      printf("Opção inválida. Digite novamente:");
      getchar();
      break;
    }
    
  }
  
  close(sfd);
  
  return 0;
}
