#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#include "socketServer.h"
#include "stringChecking.h"

struct sockaddr_un serverAddr;

/*la funzione crea un socket server AF_UNIX, associa un percorso a esso, lo configura per accettare connessioni
 * e restituisce il file descriptor del socket server.*/
int createSocketSever(char *msg) {
	int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockFd < 0) {
		printf("client socket failure\n");
		perror("Socket");
		exit(1);
	}

	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, msg);

	unlink(msg);

	if (bind(sockFd, (struct sockaddr*) &serverAddr, sizeof(struct sockaddr))
			< 0) {
		printf("server bind failure\n");
		perror("server: ");
		exit(1);
	}

	if (listen(sockFd, 8) < 0) {
		printf("server accept failure\n");
		perror("server: ");
		exit(1);
	}

	return sockFd;
}

//Funzione per accettare le connesione dai vari client
int acceptConnection(int fd){
	int fdAccepted;

	if ((fdAccepted = accept(fd, (struct sockaddr*) &serverAddr, &serverAddr)) < 0) {
		     		perror("Accettazione della connessione fallita");
		     	}
	return fdAccepted;
}

//Funzione per inviare messaggi al socket Client, nota che viene utilizata lunghezza +1 per il carattere \0
int sendMessageToClient(int toFileDescriptor, void *fromBuffer){
	return write(toFileDescriptor, fromBuffer, strlen(fromBuffer) + 1);
}

//utilizza la funzione readLine (vedi stringChecking.c) per leggere i dati dal socket
char* receiveMessageFromClient(int fromFileDescriptor){
	return readLine(fromFileDescriptor);
}

/*prende come argomento il file descriptor fileDescriptor del socket e configura il socket in modalità di lettura non bloccante.
 * Utilizza fcntl per ottenere i flag del socket corrente, imposta il flag O_NONBLOCK e aggiorna i flag del socket utilizzando fcntl nuovamente.*/
void createNonBlockReadServer(int fileDescriptor){
				int flags = fcntl(fileDescriptor, F_GETFL);
				flags |= O_NONBLOCK;
				fcntl(fileDescriptor, F_SETFL, flags);
}

//Funzione per chiudere la connesione con la socket
void closeSocketServer(int sockfd) {
	int result = close(sockfd);
	if (result == -1) {
		perror("Errore nella chiusura della socket");
	}
}
