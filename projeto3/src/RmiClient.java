//package rmi;

import java.io.Console;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.List;

import sun.nio.cs.ext.ISCII91;

public class RmiClient {
	
	static public void main(String args[]) {
		RMIServerInterface rmiServer;
		Registry registry;
		Boolean isClientLibrary = false;
		
		if (args.length < 2){
			System.out.println("Usage: client hostname usertype");
			System.exit(1);
		}
		
		String serverAddress = args[0];
		String serverPort = args[1];
		
		try {
			// Obtem o "registry" do objeto que fornecera as informacoes do
			// cinema no servidor
			registry = LocateRegistry.getRegistry(serverAddress, (new Integer(
					serverPort)).intValue());
			
			// Look up the remote object
			rmiServer = (RMIServerInterface) (registry.lookup("rmiServer"));
			RmiClient rmiClient = new RmiClient();
			
			// Seta a permissao para o usuario do tipo "livraria"
			if((args.length == 3) && (args[2].equals("library"))){
				isClientLibrary = true;
			}
			
			if(rmiServer != null) {
				// Exibe menu
				System.out.println(rmiClient.printMenu(isClientLibrary));
				
				// Le da entrada padrao
				Console c = System.console();
		        if (c == null) {
		            System.err.println("No console.");
		            System.exit(1);
		        }

		        String option = c.readLine("Entre com uma das opcoes disponiveis: ");
		        String info = null;
		        
		        while(option != null && !option.trim().isEmpty()) {
		        	if(option.equals("p")) {
		        		// Imprime menu
		        		System.out.println(rmiClient.printMenu(isClientLibrary));
		        	}
		        	else if(option.equals("0")){
		        		// Sair
		        		break;
		        	}
		        	else if(option.equals("1")) {
		        		// Listar ISBN e título de todos os livros
		        		System.out.println("\n\n");
		        		List<Book> titulos = rmiServer.listarTodosLivros();
		        		for (Book book : titulos) {
							System.out.println(book.getIsbn()+" "+book.getTitle());
						}
		        		System.out.println("\n\n");
		        	} 
		        	else if (option.equals("2")) {
		        		//Exibir descrição de um livro
		        		info = c.readLine("Digite o ISBN do livro:");
		        		
		        		String desc = null;
		        		if(info != null && !info.trim().isEmpty()) {
		        			desc = rmiServer.getBookDescByIsbn(info);
		        			
		        			if(desc != null) {
		        				System.out.println(desc);
		        			}
		        		}
		        		System.out.println("\n\n");
		        		
		        	} 
		        	else if (option.equals("3")) {
		        		//Exibir todas informacoes de um livro
		        		info = c.readLine("Digite o ISBN do livro:");
		        		
		        		Book book = new Book();
		        		if(info != null && !info.trim().isEmpty()){
		        			book = rmiServer.getAllInfo(info);
		        			
		        			if(book != null) {
		        				String resposta = book.getIsbn()+" "+book.getAuthor()+" "+book.getDescription()+" "+
		        						book.getPublisher()+" "+book.getYear()+" "+book.getQuantity();
								System.out.println(resposta);
							}	
		        		}	
		        		System.out.println("\n\n"); 		
		        	} 
//		        	else if (option.equals("4")) {
//		        		// Mostra todas as informacoes de todos os filmes
//		        		List<Movie> movies = rmiServer.getAllMovies();
//		        		Iterator iter = movies.iterator();
//		        		while(iter.hasNext()){
//		        			Movie movie = (Movie)iter.next();
//			        		System.out.println("--------------------------------------------------------------------------------");
//		        			System.out.println("*** ID: "+movie.getId());
//							System.out.println("*** TITULO: "+movie.getTitulo());
//							System.out.println("*** SINOPSE: "+movie.getSinopse());
//							System.out.println("*** SALA: "+movie.getSala());
//							String horarios = "";
//							for(int i=0; i<movie.getHorarios().length-1; i++)
//								horarios += movie.getHorarios()[i] + " / ";
//							horarios += movie.getHorarios()[movie.getHorarios().length-1];
//							System.out.println("*** HORARIOS: " + horarios);
//							System.out.println("*** MEDIA: "+movie.getMedia());
//							System.out.println("*** QUANTIDADE DE NOTAS: "+movie.getQtdeNotas());
//		        			System.out.println("--------------------------------------------------------------------------------\n");
//			        	}
//			        	System.out.println("\n\n");
//		        	} else if(option.equals("5")) {
//		        		// Atribui nota a um dado filme
//		        		String id, nota;
//		        		id = c.readLine("Digite o ID do filme:");
//		        		nota = c.readLine("Digite a nota:");
//		        		if( (id != null && !id.trim().isEmpty()) &&
//		        				(nota != null && !nota.trim().isEmpty()) )	{
//		        			Double rate = Double.valueOf(nota);
//		        			rmiServer.rateMovieById(id, rate);
//		        		}
//		        		System.out.println("Media atribuida com sucesso!");
//		        		System.out.println("\n\n");
//		        	} else if(option.equals("6")) {
//		        		// Exibe a media atual de um dado filme
//		        		info = c.readLine("Digite o ID do filme:");
//		        		
//		        		Double rating = 0.0;
//		        		Integer voters = 0;
//		        		
//		        		if(info != null && !info.trim().isEmpty()){
//		        			rating = rmiServer.getRatingById(info);
//		        			voters = rmiServer.getVotersById(info);
//		        		}
//		        		System.out.println("***MEDIA: " + rating);
//		        		System.out.println("***NUMERO DE CLIENTES QUE VOTARAM: " + voters);
//		        	}
		        	
		        	// Exibe menu
//					System.out.println(rmiClient.printMenu());
		        	option = c.readLine("Entre com uma das opcoes disponiveis: ");
		        }
			}
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (NotBoundException e) {
			e.printStackTrace();
		}
	}
	
	public String printMenu(Boolean isClientLibrary) throws RemoteException {
		String menu = "\n\n******************************************************\n";
		menu += "Catálogo de livros! Entre com uma das opções abaixo e pressione ENTER\n\n";
		menu += "p - Imprimir esse menu\n";
		menu += "0 - Sair\n";
		menu += "1 - Listar ISBN e título de todos os livros\n";
		menu += "2 - Exibir descrição de um livro\n";
		menu += "3 - Exibir todas informacoes de um livro\n";
		menu += "4 - Exibir todas informacoes de todos os livros\n";
		menu += "5 - Exibir a quantidade de um livro\n";
		
		if (isClientLibrary){
			menu += "6 - Alterar a quantidade de um livro\n";
		}
		
		menu += "******************************************************\n";
		
		return menu;
	}
	
//	void printMenu(int isClientLibrary) {
//		  if (isClientLibrary) {
//		    printf("6 - Alterar a quantidade de um livro\n");
//		  }
//		  printf("******************************************************\n");
//	}

}
