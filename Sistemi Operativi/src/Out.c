#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/un.h>

#include "socketClient.h"
#include "stringChecking.h"

/*Funzione utilizzata nel caso di arresto e parcheggio*/
void reciveBreak(char *str){
	if(strncmp(str, "ARRESTO TOTALE", 18) == 0){
		exit(0);
	}else if(strncmp(str, "Park complete", 18) == 0){
		exit(0);
	}
}


int main() {
	char* msg;
	//Connessione al server ECU
	int sockFd = connectTo("ECU");
	printf("Connesso\n");
	while (1) {
		//Attende i messaggi che la Ecu le invia
		msg = receiveMessageFromServer(sockFd);
		//Se uno dei messaggi letti è ARRESTO TOTALE o Park Complete termina
		reciveBreak(msg);
		printf("Ricevuto da ECU.c: %s\n", msg);
		free(msg);
		//Pulisce l'array
		memset(msg, 0, sizeof(msg));
	}
	//Viene chiusa la connesione con la socket
	closeClientConnection(sockFd);
	return 0;
}

