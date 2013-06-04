
import java.io.BufferedWriter;
import java.io.FileWriter;
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
	
	private final int thisPort = 49152; // porta efemera escolhida para o servidor
	private String thisAddress;
	private Registry registry; // rmi registry for lookup the remote objects.
	
	private long lStartTime; // start time
	private long lEndTime; // end time
	
	List<Book> arrBooks = null; // lista onde sera armazenado os livros na memoria

	/*
	 * Construtor da classe RmiServer: tenta criar um servidor local na porta escolhida
	 * previamente
	 */
	public RmiServer() throws RemoteException {
		try {
			// Returns the local host
			thisAddress = (InetAddress.getLocalHost()).toString();
		} catch (Exception e) {
			throw new RemoteException("can't get inet address.");
		}
		
		try {
			// Creates and exports a Registry instance on the local 
                        // host that accepts requests on the specified port
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
         * Utiliza o driver java para sqlite para conectar e realizar as
         * consultas no banco
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
		    stmt.close();
			c.close();
	    } catch ( Exception e ) {
	    	System.err.println( e.getClass().getName() + ": " + e.getMessage() );
	    	System.exit(0);
	    }
	}

	@Override
	public List<Book> listarTodosLivros() throws RemoteException {
		// Listar ISBN e título de todos os livros
		lStartTime = System.nanoTime();
		lEndTime = System.nanoTime();
		
		relatorio("relatorio_1.txt",""+(lEndTime - lStartTime));
		return arrBooks;
	}

	@Override
	public String getBookDescByIsbn(String isbn) throws RemoteException {
		//Exibir descrição de um livro
		lStartTime = System.nanoTime();
		String resposta = null;
		if(arrBooks != null) {
			for (Book book : arrBooks) {
				if(book.getIsbn().equals(isbn)) {
					resposta = book.getDescription();
				}
			}
		}
		lEndTime = System.nanoTime();
		relatorio("relatorio_2.txt",""+(lEndTime - lStartTime));
		return resposta;
	}

	@Override
	public Book getAllInfo(String isbn) throws RemoteException {
		//Exibir todas informacoes de um livro
		lStartTime = System.nanoTime();
		Book retBook = null;
		if(arrBooks != null) {
			for (Book book : arrBooks) {
				if(book.getIsbn().equals(isbn)) {
					retBook = book;
				}
			}
		}
		lEndTime = System.nanoTime();
		relatorio("relatorio_3.txt",""+(lEndTime - lStartTime));
		return retBook;
	}

	@Override
	public List<Book> getAllBooksInfo() throws RemoteException {
		// Exibir todas as informacoes de todos os livros
		lStartTime = System.nanoTime();
		lEndTime = System.nanoTime();
		relatorio("relatorio_4.txt",""+(lEndTime - lStartTime));
		return arrBooks;
	}

	@Override
	public String getBookQuant(String isbn) throws RemoteException {
		//Exibir a quantidade de um livro
		lStartTime = System.nanoTime();
		Book retBook = null;
		if(arrBooks != null) {
			for (Book book : arrBooks) {
				if(book.getIsbn().equals(isbn)) {
					retBook = book;
				}
			}
		}
		lEndTime = System.nanoTime();
		relatorio("relatorio_5.txt",""+(lEndTime - lStartTime));
		return retBook.getQuantity();
	}

	@Override
	public void setBookQuant(String isbn, String newQtd, Boolean isClientLibrary) throws RemoteException {
                // Altera a quantidade de um livro
                // Disponivel somente para usuarios do tipo "livraria"
                // Utiliza o driver java para sqlite para realizar a conexao
                // e as alteracoes no banco
		lStartTime = System.nanoTime();
		if (isClientLibrary) {
			Connection c = null;
		    Statement stmt = null;
			try {
				Class.forName("org.sqlite.JDBC");
				c = DriverManager.getConnection("jdbc:sqlite:database.sqlite");
	//			System.out.println("Opened database successfully");
				stmt = c.createStatement();

				String sql = "UPDATE books SET quantity ='";
				sql +=newQtd;
				sql +="' WHERE ISBN = '";
				sql +=isbn;
				sql +="'";

				System.out.println(sql);

				stmt.execute(sql);

			    stmt.close();
			    c.close();

			    // Atualiza valor na memoria
			    if(arrBooks != null) {
				for (Book book : arrBooks) {
					if(book.getIsbn().equals(isbn)) {
						book.setQuantity(newQtd);
					}
				}
		}

		    } catch ( Exception e ) {
		    	System.err.println( e.getClass().getName() + ": " + e.getMessage() );
		    	System.exit(0);
		    }
		}
		lEndTime = System.nanoTime();
		relatorio("relatorio_6.txt",""+(lEndTime - lStartTime));
	}
	
        /*
         * Recebe o nome de um arquivo como parametro e um valor de tempo
         * em nanosegundos.
         * Concatena esse tempo no arquivo seguido de uma quebra de linha
         */
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
