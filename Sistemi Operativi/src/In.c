#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/un.h>
#include <signal.h>

#include "socketClient.h"
#include "stringChecking.h"

int main() {
	//Viene stabilita la connessione alla ECU
	int sockFd = connectTo("ECU");
	char msg[11];

	do{
	printf("Enter you message:\n");
	//Viene ricevuto un input da tastiera
	scanf("%s", &msg);
	//L'input ricevuto viene inviato alla Ecu
	sendMessageToServer(sockFd, msg);
	}while(1);
	//Viene chiusa la connessione con la socket
	closeClientConnection(sockFd);
    return 0;
}
