all: compile
	@echo ""

clean:
	rm *.class

compile: interface cserver cclient

cserver:
        # Compila o client
	javac RmiServer.java

cclient:
        # Compila o client
	javac RmiClient.java
	
database:
        # Compila a db
	javac Book.java

interface:
        # Compila a interface
	javac RMIServerInterface.java

server:
        # Roda o server
	java -classpath .:sqlite-jdbc-3.7.2.jar: RmiServer

client:
        # Roda o client
        # Exemplo como usar na linha de comando: 
        # make client HOST=localhost PORT=49152 USER=library
	java RmiClient ${HOST} ${PORT} ${USER}


