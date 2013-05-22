//package rmi;

import java.io.Console;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.List;
import java.util.Iterator;

public class RmiClient {

	static public void main(String args[]) {
		RMIServerInterface rmiServer;
		Registry registry;
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
			
			if(rmiServer != null) {
				// Exibe menu
				System.out.println(rmiClient.showMenu());
				
				// Le da entrada padrao
				Console c = System.console();
		        if (c == null) {
		            System.err.println("No console.");
		            System.exit(1);
		        }

		        String option = c.readLine("Digite a sua opcao: ");
		        String info = null;
		        
		        while(option != null && !option.trim().isEmpty() && !option.equals("7")) {
		        	if(option.equals("1")) {
		        		System.out.println("\n\n");
		        		// Exibe titulo de todos os filmes
		        		List<Movie> titulos = rmiServer.getAllMovieTitles();
		        		System.out.println("********** TITULOS **********");
		        		System.out.println("* ID\t Titulo");
		        		for (Movie movie : titulos) {
							System.out.println("* "+movie.getId()+"\t"+movie.getTitulo());
						}
		        		System.out.println("\n\n");
		        	} else if (option.equals("2")) {
		        		// Exibe a sinopse de um dado filme
		        		info = c.readLine("Digite o ID do filme:");
		        		
		        		String sinopse = null;
		        		if(info != null && !info.trim().isEmpty()) {
		        			System.out.println("Pesquisando..."+info);
		        			sinopse = rmiServer.getMovieSynById(info);
		        			
		        			if(sinopse != null) {
		        				System.out.println("SINOPSE : "+sinopse);
		        			}
		        		}
		        		System.out.println("\n\n");
		        		
		        	} else if (option.equals("3")) {
		        		// Exibe todas as informacoes de um filme
		        		info = c.readLine("Digite o ID do filme:");
		        		
		        		Movie movie = new Movie();
		        		if(info != null && !info.trim().isEmpty()){
		        			System.out.println("Pesquisando..."+info);
		        			movie = rmiServer.getMovieById(info);
		        			
		        			if(movie != null) {
								System.out.println("*** ID: "+movie.getId());
								System.out.println("*** TITULO: "+movie.getTitulo());
								System.out.println("*** SINOPSE: "+movie.getSinopse());
								System.out.println("*** SALA: "+movie.getSala());
								String horarios = "";
								for(int i=0; i<movie.getHorarios().length-1; i++)
									horarios += movie.getHorarios()[i] + " / ";
								horarios += movie.getHorarios()[movie.getHorarios().length-1];
								System.out.println("*** HORARIOS: " + horarios);
								System.out.println("*** MEDIA: "+movie.getMedia());
								System.out.println("*** QUANTIDADE DE NOTAS: "+movie.getQtdeNotas());
							}	
		        		}	
		        		System.out.println("\n\n"); 		
		        	} else if (option.equals("4")) {
		        		// Mostra todas as informacoes de todos os filmes
		        		List<Movie> movies = rmiServer.getAllMovies();
		        		Iterator iter = movies.iterator();
		        		while(iter.hasNext()){
		        			Movie movie = (Movie)iter.next();
			        		System.out.println("--------------------------------------------------------------------------------");
		        			System.out.println("*** ID: "+movie.getId());
							System.out.println("*** TITULO: "+movie.getTitulo());
							System.out.println("*** SINOPSE: "+movie.getSinopse());
							System.out.println("*** SALA: "+movie.getSala());
							String horarios = "";
							for(int i=0; i<movie.getHorarios().length-1; i++)
								horarios += movie.getHorarios()[i] + " / ";
							horarios += movie.getHorarios()[movie.getHorarios().length-1];
							System.out.println("*** HORARIOS: " + horarios);
							System.out.println("*** MEDIA: "+movie.getMedia());
							System.out.println("*** QUANTIDADE DE NOTAS: "+movie.getQtdeNotas());
		        			System.out.println("--------------------------------------------------------------------------------\n");
			        	}
			        	System.out.println("\n\n");
		        	} else if(option.equals("5")) {
		        		// Atribui nota a um dado filme
		        		String id, nota;
		        		id = c.readLine("Digite o ID do filme:");
		        		nota = c.readLine("Digite a nota:");
		        		if( (id != null && !id.trim().isEmpty()) &&
		        				(nota != null && !nota.trim().isEmpty()) )	{
		        			Double rate = Double.valueOf(nota);
		        			rmiServer.rateMovieById(id, rate);
		        		}
		        		System.out.println("Media atribuida com sucesso!");
		        		System.out.println("\n\n");
		        	} else if(option.equals("6")) {
		        		// Exibe a media atual de um dado filme
		        		info = c.readLine("Digite o ID do filme:");
		        		
		        		Double rating = 0.0;
		        		Integer voters = 0;
		        		
		        		if(info != null && !info.trim().isEmpty()){
		        			rating = rmiServer.getRatingById(info);
		        			voters = rmiServer.getVotersById(info);
		        		}
		        		System.out.println("***MEDIA: " + rating);
		        		System.out.println("***NUMERO DE CLIENTES QUE VOTARAM: " + voters);
		        	}
		        	
		        	// Exibe menu
					System.out.println(rmiClient.showMenu());
		        	option = c.readLine("Digite a sua opcao: ");
		        }
			}
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (NotBoundException e) {
			e.printStackTrace();
		}
	}
	
	public String showMenu() throws RemoteException {
		String menu = "********************* MENU **********************\n";
		menu += "*1 - Exibir titulo de todos os filmes           *\n";
		menu += "*2 - Exibir a sinopse de um filme               *\n";
		menu += "*3 - Exibir todas as informacoes de um filme    *\n";
		menu += "*4 - Exibir a informacao de todos os filmes     *\n";
		menu += "*5 - Dar nota para um filme                     *\n";
		menu += "*6 - Exibir a media de um filme                 *\n";
		menu += "*7 - Sair                                       *\n";
		menu += "**************************************************\n\n";
		return menu;
	}

}
