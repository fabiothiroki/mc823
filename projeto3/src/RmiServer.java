
import java.net.InetAddress;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Connection;
import java.sql.DriverManager;
import java.util.ArrayList;
import java.util.List;

public class RmiServer extends UnicastRemoteObject implements RMIServerInterface {

	/* serialVersionUID: used during deserialization to verify that the sender and receiver of a
	 * serialized object have loaded classes for that object that are compatible 
	 * with respect to serialization
	*/
	private static final long serialVersionUID = -3100149800409779504L;
	
	final int thisPort = 49152; // porta efemera escolhida para o servidor
	String thisAddress;
	Registry registry; // rmi registry for lookup the remote objects.
	
//	List<Book> arr_books = null;
	List<Movie> movies = null;

	/*
	 * Construtor da classe RmiServer: tenta criar um servidor local na porta escolhida
	 * previamente
	 */
	public RmiServer() throws RemoteException {
		try {
			// get the address of this host.
			thisAddress = (InetAddress.getLocalHost()).toString();
		} catch (Exception e) {
			throw new RemoteException("can't get inet address.");
		}
		
//		thisPort = 49152; // this port(registrys port)
			
		System.out.println("this address=" + thisAddress + ",port=" + thisPort);
		try {
			// create the registry and bind the name and object.
			registry = LocateRegistry.createRegistry(thisPort);
			registry.rebind("rmiServer", this);
		} catch (RemoteException e) {
			throw e;
		}
	}

	public static void main(String args[]) {
		try {
			RmiServer s = new RmiServer();
			s.loadBooks();
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	/**
	 * Carrega as informações dos livros na memória
	 */
	public void loadBooks() {
		
		try {
			Connection c = null;
			Class.forName("org.sqlite.JDBC");
			c = DriverManager.getConnection("jdbc:sqlite:database.sqlite");
	    } catch ( Exception e ) {
	    	System.err.println( e.getClass().getName() + ": " + e.getMessage() );
	    	System.exit(0);
	    }
	    System.out.println("Opened database successfully");
	  
		
//		arr_books = new ArrayList<Book>();
		
//		try {
//			FileReader fr = new FileReader("movies.txt");
//			BufferedReader br = new BufferedReader(fr);
//			String linha = null;
//			while((linha = br.readLine()) != null) {
//				//System.out.println(linha);
//				String[] movieTokens = linha.split("\\|");
//				Movie filme = new Movie();
//				filme.setId(movieTokens[0]);
//				//System.out.println("id :"+filme.getId());
//				filme.setTitulo(movieTokens[1]);
//				//System.out.println("titulo :"+filme.getTitulo());
//				filme.setSinopse(movieTokens[2]);
//				//System.out.println("sinopse :"+filme.getSinopse());
//				filme.setSala(movieTokens[3]);
//				//System.out.println("sala :"+filme.getSala());
//				String[] horarios = movieTokens[4].split(" ");
//				filme.setHorarios(horarios);
//				//System.out.println("horarios :"+filme.getHorarios());
//				filme.setMedia(0.0);
//				filme.setQtdeNotas(0);
//				movies.add(filme);
//			}
//		} catch (FileNotFoundException e) {
//			e.printStackTrace();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
	}

	@Override
	public List<Movie> getAllMovieTitles() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getMovieSynById(String id) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Movie getMovieById(String id) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public List<Movie> getAllMovies() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void rateMovieById(String id, Double rate) throws RemoteException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Double getRatingById(String id) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Integer getVotersById(String id) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

//	@Override
//	public List<Movie> getAllMovieTitles()
//			throws RemoteException {
//		return movies;
//	}
//
//	@Override
//	public List<Movie> getAllMovies() throws RemoteException {
//		return movies;
//	}
//
//	@Override
//	public Movie getMovieById(String id)
//			throws RemoteException {
//		Movie resposta = null;
//		
//		if(movies != null) {
//			for (Movie filme : movies) {
//				if(filme.getId().equals(id)) {
//					resposta = filme;
//				}
//			}
//			
//		}
//		return resposta;
//	}
//
//	@Override
//	public String getMovieSynById(String id) throws RemoteException {
//		String resposta = null;
//		if(movies != null) {
//			for (Movie filme : movies) {
//				if(filme.getId().equals(id)) {
//					resposta = filme.getSinopse();
//				}
//			}
//		}
//		return resposta;
//	}
//
//	@Override
//	public Double getRatingById(String id) throws RemoteException {
//		Double resposta = null;
//		if(movies != null) {
//			for (Movie filme : movies) {
//				if(filme.getId().equals(id)) {
//					resposta = filme.getMedia();
//				}
//			}
//		}
//		return resposta;
//	}
//	
//	@Override
//	public Integer getVotersById(String id) throws RemoteException {
//		Integer resposta = null;
//		if(movies != null) {
//			for (Movie filme : movies) {
//				if(filme.getId().equals(id)) {
//					resposta = filme.getQtdeNotas();
//				}
//			}
//		}
//		return resposta;
//	}
//
//	@Override
//	public void rateMovieById(String id, Double rate) throws RemoteException {
//		if(movies != null) {
//			for (Movie filme : movies) {
//				if(filme.getId().equals(id)) {
//					Double oldMedia = filme.getMedia();
//					System.out.println("Media antiga: "+oldMedia);
//					Integer qtdeNotas = filme.getQtdeNotas();
//					Double novaMedia = (oldMedia*qtdeNotas+rate)/(qtdeNotas+1);
//					System.out.println("Nova media: "+novaMedia);
//					filme.setMedia(novaMedia);
//					filme.setQtdeNotas(qtdeNotas+1);
//				}
//			}
//		}
//	}
}
