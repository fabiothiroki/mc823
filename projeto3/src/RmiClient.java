//package rmi;

import java.io.Console;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.List;
import java.util.Iterator;

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
		        	else if (option.equals("4")) {
		        		// Exibir todas as informacoes de todos os livros
		        		List<Book> books = rmiServer.getAllBooksInfo();
		        		Iterator iter = books.iterator();
		        		while(iter.hasNext()){
		        			Book book = (Book)iter.next();
	
						String resposta = book.getIsbn()+" "+book.getAuthor()+" "+book.getDescription()+" "+book.getPublisher()+" "+book.getYear()+" "+book.getQuantity();
						
						System.out.println(resposta);

			        	}
			        	System.out.println("\n\n");
		        	} 
					else if(option.equals("5")) {
		        		// Exibir a quantidade de um livro
		        		info = c.readLine("Digite o ISBN do livro:");

		        		String quant = null;
		        		if(info != null && !info.trim().isEmpty()) {
		        			quant = rmiServer.getBookQuant(info);
		        			
		        			if(quant != null) {
		        				System.out.println(quant);
		        			}
		        		}
		        		System.out.println("\n\n");


		        	} 
		        	else if(option.equals("6")) {
		        		//Alterar a quantidade de um livro
		        		if (isClientLibrary) {

			        		String newQtd;
			        		info = c.readLine("Digite o ISBN do livro:");
			        		newQtd = c.readLine("Digite a nova quantidade:");

			        		if( (info != null && !info.trim().isEmpty()) && (newQtd != null && !newQtd.trim().isEmpty()) )	{
			        			rmiServer.setBookQuant(info, newQtd, isClientLibrary);
			        		}
		        		}

		        	}
		        	
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
