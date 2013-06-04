all: compile
	@echo ""

clean:
	rm *.class

compile: interface cserver cclient

cserver:
	javac RmiServer.java

cclient:
	javac RmiClient.java
	
database:
	javac Book.java

interface:
	javac RMIServerInterface.java

server:
	java -classpath .:sqlite-jdbc-3.7.2.jar: RmiServer

client:
	java RmiClient ${ARGS}


