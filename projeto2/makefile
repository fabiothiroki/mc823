all: client server_sqlite

server_sqlite:
	gcc -lsqlite3 server_udp_sqlite.c -o server_udp_sqlite

client:
	gcc -g client_udp.c -o client

clean:
	rm -rf *.o makefile~ server client server_udq_sqlite
