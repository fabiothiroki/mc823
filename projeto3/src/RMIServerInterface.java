import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface RMIServerInterface extends Remote {
	// Listar ISBN e título de todos os livros
	public List<Book> listarTodosLivros() throws RemoteException;
	
	//Exibir descrição de um livro
	public String getBookDescByIsbn(String isbn) throws RemoteException;
	
	//Exibir todas informacoes de um livro
	public Book getAllInfo(String isbn) throws RemoteException;
	
	//Exibir todas informacoes de todos os livros
	public List<Book> getAllBooksInfo() throws RemoteException;
	
	//Exibir a quantidade de um livro
	public String getBookQuant(String isbn) throws RemoteException;
	
	//Alterar a quantidade de um livro
	public void setBookQuant(String isbn, String newQtd, Boolean isClientLibrary) throws RemoteException;
}
