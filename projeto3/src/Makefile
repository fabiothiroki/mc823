all: compile
	@echo ""

clean:
	rm *.class

compile: interface cserver cclient

cserver:
#	javac -classpath sqlite-jdbc-3.7.2.jar Movie.java RMIServerInterface.java RmiServer.java
	javac RmiServer.java

cclient:
	javac RmiClient.java
	
database:
	javac Book.java

interface:
	javac RMIServerInterface.java

server:
#	java RmiServer
	java -classpath .:sqlite-jdbc-3.7.2.jar: RmiServer
# 	java -classpath src:lib/etapa2.jar main/Main $(INPUT) $(OUTPUT)
 	

# client:
# 	java RmiClient ${ARGS}


