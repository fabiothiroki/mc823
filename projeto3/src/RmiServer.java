
import java.net.InetAddress;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
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
	
	List<Book> arrBooks = null; // lista onde sera armazenado os livros na memoria
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
			//cria uma nova instancia do servidor com a configuracoes
			//definidas no construtor
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
		
		arrBooks = new ArrayList<Book>();
		
		Connection c = null;
	    Statement stmt = null;
		try {
			Class.forName("org.sqlite.JDBC");
			c = DriverManager.getConnection("jdbc:sqlite:database.sqlite");
//			System.out.println("Opened database successfully");
			stmt = c.createStatement();
		    ResultSet rs = stmt.executeQuery( "select title,author,description,publisher,year,quantity,isbn from books order by title;" );
		    while ( rs.next() ) {
		    	Book book = new Book();
		    	
		    	book.setIsbn(rs.getString("isbn"));
		    	book.setTitle(rs.getString("title"));
		    	book.setAuthor(rs.getString("author"));
		    	book.setDescription(rs.getString("description"));
		    	book.setPublisher(rs.getString("publisher"));
		    	book.setYear(rs.getString("publisher"));
		    	book.setQuantity(rs.getString("quantity"));
		    	
		    	arrBooks.add(book);
		    }
	    } catch ( Exception e ) {
	    	System.err.println( e.getClass().getName() + ": " + e.getMessage() );
	    	System.exit(0);
	    }
	}

	@Override
	public List<Book> listarTodosLivros() throws RemoteException {
		// Listar ISBN e título de todos os livros
		return arrBooks;
	}

	@Override
	public String getBookDescByIsbn(String isbn) throws RemoteException {
		//Exibir descrição de um livro
		String resposta = null;
		if(arrBooks != null) {
			for (Book book : arrBooks) {
				if(book.getIsbn().equals(isbn)) {
					resposta = book.getDescription();
				}
			}
		}
		return resposta;
	}

	@Override
	public Book getAllInfo(String isbn) throws RemoteException {
		//Exibir todas informacoes de um livro
		Book retBook = null;
		if(arrBooks != null) {
			for (Book book : arrBooks) {
				if(book.getIsbn().equals(isbn)) {
					retBook = book;
				}
			}
		}
		return retBook;
	}

	@Override
	public List<Book> getAllBooksInfo() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getBookQuant(String isbn) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void setBookQuant(String isbn) throws RemoteException {
		// TODO Auto-generated method stub
		
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
