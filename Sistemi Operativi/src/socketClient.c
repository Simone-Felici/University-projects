#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#include "socketClient.h"
#include "stringChecking.h"

/*La funzione connectTo prende come argomento una stringa msg che rappresenta il percorso del socket
 *  a cui connettersi. La funzione crea un socket utilizzando socket(AF_UNIX, SOCK_STREAM, 0),
 *  specificando l'indirizzo della famiglia AF_UNIX e il tipo di socket SOCK_STREAM.
 *	Successivamente, viene inizializzata una struttura clientAddr di tipo
 *   sockaddr_un per rappresentare l'indirizzo del client socket, specificando la famiglia AF_UNIX e copiando
 *   il percorso del socket in clientAddr.sun_path. Viene quindi eseguito un ciclo do-while per tentare la
 *    connessione al socket utilizzando connect. Se la connessione fallisce, viene stampato un messaggio di errore
 *    e si attende 1 secondo prima di ritentare. Il ciclo continua finché la connessione non viene stabilita con successo.
 *    Infine, viene restituito il file descriptor del socket appena creato*/
int connectTo(char *msg) {
	struct sockaddr_un clientAddr;
	int result;

	int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (sockFd < 0) {
			printf("client socket failure\n");
			perror("Socket");
			exit(1);
		}

		clientAddr.sun_family = AF_UNIX;
		strcpy(clientAddr.sun_path, msg);

		do {
				result = connect(sockFd, (struct sockaddr*) &clientAddr,
						sizeof(struct sockaddr));
				if (result == -1) {
					printf("connection problem;re-try in 1 sec\n");
					sleep(1);
				}
			} while (result == -1);

	return sockFd;
}

//Funzione per inviare messaggi al socket Server, nota che viene utilizata lunghezza +1 per il carattere \0
int sendMessageToServer(int toFileDescriptor, void *fromBuffer){
	return write(toFileDescriptor, fromBuffer, strlen(fromBuffer) + 1);
}

//utilizza la funzione readLine (vedi stringChecking.c) per leggere i dati dal socket
char* receiveMessageFromServer(int fromFileDescriptor){
	return readLine(fromFileDescriptor);
}

/*prende come argomento il file descriptor fileDescriptor del socket e configura il socket in modalità di lettura non bloccante.
 * Utilizza fcntl per ottenere i flag del socket corrente, imposta il flag O_NONBLOCK e aggiorna i flag del socket utilizzando fcntl nuovamente.*/
void createNonBlockReadClient(int fileDescriptor){
				int flags = fcntl(fileDescriptor, F_GETFL);
				flags |= O_NONBLOCK;
				fcntl(fileDescriptor, F_SETFL, flags);
}

//Viene chiusa la connesione con la socket
void closeClientConnection(int sockfd) {
	int result = close(sockfd);
	if (result == -1) {
		perror("Errore nella chiusura della socket");
	}
}
