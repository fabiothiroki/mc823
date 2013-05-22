import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface RMIServerInterface extends Remote {
	public List<Movie> getAllMovieTitles() throws RemoteException;
	public String getMovieSynById(String id) throws RemoteException;
	public Movie getMovieById(String id) throws RemoteException;
	public List<Movie> getAllMovies() throws RemoteException;
	public void rateMovieById(String id, Double rate) throws RemoteException;
	public Double getRatingById(String id) throws RemoteException;
	public Integer getVotersById(String id) throws RemoteException;
}
