
import java.io.Serializable;

public class Book implements Serializable{
	
	/* serialVersionUID: used during deserialization to verify that the sender and receiver of a
	 * serialized object have loaded classes for that object that are compatible 
	 * with respect to serialization*/
	private static final long serialVersionUID = 721766176382487053L;
	
	private String isbn;
	private String title;
	private String author;
	private String description;
	private String publisher;
	private String year;
	private String quantity;

	public String getIsbn() {
		return isbn;
	}
	public void setIsbn(String isbn) {
		this.isbn = isbn;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getAuthor() {
		return author;
	}
	public void setAuthor(String author) {
		this.author = author;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public String getPublisher() {
		return publisher;
	}
	public void setPublisher(String publisher) {
		this.publisher = publisher;
	}
	public String getYear() {
		return year;
	}
	public void setYear(String year) {
		this.year = year;
	}
	public String getQuantity() {
		return quantity;
	}
	public void setQuantity(String quantity) {
		this.quantity = quantity;
	}
}
