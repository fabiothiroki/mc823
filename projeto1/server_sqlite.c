#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

typedef struct bookStruct {
  char isbn[11];		
  char title[50];
  char author[50];
  char description[300];
  char publisher[50];
  char year[3];	
  char horarios[18];	
  char quantity[2];	
} Book;

Book arr_books[10];

void loadBooks(){

	sqlite3 *conn;
	sqlite3_stmt *res;
	int error = 0;
	int rec_count = 0;
	const char *errMSG;
	const char *tail;

	error = sqlite3_open("database.sqlite", &conn);

	if (error) {
		printf("Can not open database");
	}

	 error = sqlite3_prepare_v2(conn, "select title,author,description from books order by title",1000, &res, &tail);

	 int i = 0;
	 while (sqlite3_step(res) == SQLITE_ROW) {

	 	strcpy(arr_books[i].title,sqlite3_column_text(res, 0));
	 	strcpy(arr_books[i].author,sqlite3_column_text(res, 1));
	 	strcpy(arr_books[i].description,sqlite3_column_text(res, 2));

	 	printf("\n");
	 	printf("%s \n",arr_books[i].title);
	 	printf("%s \n",arr_books[i].author);
	 }

}

int main(void) {

	loadBooks();

	return 0;
}