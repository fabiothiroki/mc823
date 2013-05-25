//package rmi;

import java.io.Console;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.List;
import java.util.Iterator;
import java.util.Date;
import sun.nio.cs.ext.ISCII91;
import java.io.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

public class RmiClient {
	
	static public void main(String args[]) {
		RMIServerInterface rmiServer;
		Registry registry;
		Boolean isClientLibrary = false;

		long lStartTime; // start time
		long lEndTime; // end time
		
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
				Scanner c = new Scanner(System.in);
				// Console c = System.console();
		        if (c == null) {
		            System.err.println("No console.");
		            // System.exit(1);
		        }


		        System.out.println("Entre com uma das opcoes disponiveis: ");
		        String option = c.nextLine();
		        // String option = c.readLine("Entre com uma das opcoes disponiveis: ");
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

		        		lStartTime = new Date().getTime();
		        		List<Book> titulos = rmiServer.listarTodosLivros();
		        		lEndTime = new Date().getTime();
		        		for (Book book : titulos) {
							System.out.println(book.getIsbn()+" "+book.getTitle());
						}
						System.out.println("Elapsed milliseconds: " +  (lEndTime - lStartTime));
		        		System.out.println("\n\n");

		        		relatorio("relatorio_com_1.txt",""+(lEndTime - lStartTime));
		        	} 
		        	else if (option.equals("2")) {
		        		//Exibir descrição de um livro
		        		System.out.println("Digite o ISBN do livro:");
		        		info = c.nextLine();
		        		// info = c.readLine("Digite o ISBN do livro:");
		        		
		        		String desc = null;
		        		if(info != null && !info.trim().isEmpty()) {
		        			lStartTime = new Date().getTime();
		        			desc = rmiServer.getBookDescByIsbn(info);
		        			
		        			if(desc != null) {
		        				System.out.println(desc);
		        			}
		        		}
		        		System.out.println("\n\n");
		        		
		        	} 
		        	else if (option.equals("3")) {
		        		//Exibir todas informacoes de um livro
		        		System.out.println("Digite o ISBN do livro:");
		        		info = c.nextLine();
		        		// info = c.readLine("Digite o ISBN do livro:");
		        		
		        		Book book = new Book();
		        		if(info != null && !info.trim().isEmpty()){
		        			lStartTime = new Date().getTime();
		        			book = rmiServer.getAllInfo(info);
		        			lEndTime = new Date().getTime();
		        			
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
		        		lStartTime = new Date().getTime();
		        		List<Book> books = rmiServer.getAllBooksInfo();
		        		lEndTime = new Date().getTime();
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
		        		System.out.println("Digite o ISBN do livro:");
		        		info = c.nextLine();

		        		String quant = null;
		        		if(info != null && !info.trim().isEmpty()) {
		        			lStartTime = new Date().getTime();
		        			quant = rmiServer.getBookQuant(info);
		        			lEndTime = new Date().getTime();
		        			
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
			        		System.out.println("Digite o ISBN do livro:");
		        			info = c.nextLine();
		        			System.out.println("Digite a nova quantidade:");
			        		newQtd = c.nextLine();

			        		if( (info != null && !info.trim().isEmpty()) && (newQtd != null && !newQtd.trim().isEmpty()) )	{
			        			lStartTime = new Date().getTime();
			        			rmiServer.setBookQuant(info, newQtd, isClientLibrary);
			        			lEndTime = new Date().getTime();
			        		}
		        		}

		        	}
		        	
		        	System.out.println("Entre com uma das opcoes disponiveis: ");
		        	option = c.nextLine();
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

	public static void relatorio(String fileName, String time) {
		try  
		{
		    FileWriter fstream = new FileWriter(fileName, true); //true tells to append data.
		    BufferedWriter out = new BufferedWriter(fstream);
		    out.write(time+"\n");
		    out.close();
		}
		catch (Exception e)
		{
		    System.err.println("Error: " + e.getMessage());
		}
	}

}
