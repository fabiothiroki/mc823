#include <stdio.h>
#include <sqlite3.h>

int main(void) {

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

	 error = sqlite3_prepare_v2(conn, "select title from books order by title",1000, &res, &tail);

	 while (sqlite3_step(res) == SQLITE_ROW) {
	 	printf("%s \n",sqlite3_column_text(res, 0));
	 }

	return 0;
}