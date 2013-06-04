import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

// interface que definirá os métodos que podem ser invocados remotamente
public interface RMIServerInterface extends Remote {
	// 1 - Listar ISBN e título de todos os livros
	public List<Book> listarTodosLivros() throws RemoteException;
	
	// 2 - Exibir descrição de um livro
	public String getBookDescByIsbn(String isbn) throws RemoteException;
	
	// 3 - Exibir todas informacoes de um livro
	public Book getAllInfo(String isbn) throws RemoteException;
	
	// 4 - Exibir todas informacoes de todos os livros
	public List<Book> getAllBooksInfo() throws RemoteException;
	
	// 5 - Exibir a quantidade de um livro
	public String getBookQuant(String isbn) throws RemoteException;
	
	//6 - Alterar a quantidade de um livro
	public void setBookQuant(String isbn, String newQtd, Boolean isClientLibrary) throws RemoteException;
}
