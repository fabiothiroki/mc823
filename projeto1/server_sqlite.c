#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>

// porta (efemera) de entrada dos clientes
#define PORT "49152"  
#define BACKLOG 10

// mensagem contendo o request do cliente
// contem a opcao do cliente, id do livro e o id do usuario
char msg[4];	

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

void loadBooks(){

	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	int rec_count = 0;
	const char *errMSG;
	const char *tail;

	error = sqlite3_open("database.sqlite", &conn);

	if (error) {
		printf("Can not open database");
	}

	 error = sqlite3_prepare_v2(conn, "select title,author,description from books order by title",1000, &res, &tail);

	 int i = 0;
	 while (sqlite3_step(res) == SQLITE_ROW) {

	 	strcpy(arr_books[i].title,sqlite3_column_text(res, 0));
	 	strcpy(arr_books[i].author,sqlite3_column_text(res, 1));
	 	strcpy(arr_books[i].description,sqlite3_column_text(res, 2));

	 	// printf("\n");
	 	// printf("%s \n",arr_books[i].title);
	 	// printf("%s \n",arr_books[i].author);
	 }

}

// Mata todos os processos zumbis
void sigchld_handler(int s) {
	pid_t pid;
	int stat; 
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
    	printf("child %d terminated\n", pid); 
    }
}

int main(void) {

	// File descriptors dos sockets
	int sfd, new_fd;

	struct sigaction sa;

	struct addrinfo hints, *result	, *rp;

	socklen_t sin_size;

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
		return 0;
	}

	// Percorre todos os endereços encontrados no getaddrinfo
	// Faz o "bind" para o primeiro socket criado com sucesso
	for (rp = result; rp != NULL; rp = rp->ai_next) {

		// socket() retorna um inteiro similar a um descritor de arquivos relacionado ao socket criado, 
		// através do qual ele pode ser referenciado
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
            continue;

        // bind() associa uma porta para o socket
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
        	break; 

        close(sfd);
	}

	if (rp == NULL) {
		/* No address succeeded */              
        printf("Could not bind\n");
        return 1;
    }

    freeaddrinfo(result);
    
    // Seta o número de conexões permitidas para o socket criado
    if (listen(sfd, BACKLOG) == -1) {
    	printf("ERROR: listen\n");
    	return 1;
    }

    // trata o sinal recebido pelos processos filhos
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      printf("ERROR: sigaction");
      return 1;
    }


	printf("SERVER -> waiting connections from clients...\n");

    while(1) {
    	// accept(): responsável por aceitar uma conexão em socket
    	sin_size = sizeof their_addr;
    	new_fd = accept(sfd, (struct sockaddr *)&their_addr, &sin_size);
    	if (new_fd == -1) {
    	  printf("ERROR: accept");
    	  continue;
    	}

    	printf("SERVER: conexao aceita\n");

    	if (!fork()) {

    		close(sfd); 

    		connected = 1;
    		while(connected){

    			//recv(): recebe dados em um socket
    			if(recv(new_fd,msg, 4, 0) == -1) {
    				printf("ERROR: recv");
    			}

    			printf("opt: %s",msg[0]);

    		}

    	}

    }

	return 0;
}