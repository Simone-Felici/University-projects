#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "logManagement.h"
#include "socketClient.h"

#define LOG_FILE "log/steer.log"

int main() {
	char *msg;
	char bufSBW[6];
	char *NO_ACTION = "NO ACTION";
	//Viene stabilita la connessione alla socket
	int sockFd = connectTo("ECU");
	//Viene aperto il file di log
	FILE *logFile = openLog(LOG_FILE);
	//crea la read in modo non bloccante
	createNonBlockReadClient(sockFd);

	while (1) {
		//Attende di ricevere (o no) un messaggio dalla Ecu
		msg = receiveMessageFromServer(sockFd);
		strcpy(bufSBW, msg);
		free(msg);
		//Confronta il messaggio letto dall Ecu e di conseguenza scrive nel file di log in base all'input
		if (strncmp(bufSBW, "STO GIRANDO A DESTRA", 7) == 0) {
			writeLog(bufSBW, 1, strlen(bufSBW), logFile);
		}else if(strncmp(bufSBW, "STO GIRANDO A SINISTRA", 9) == 0){
			writeLog(bufSBW, 1, strlen(bufSBW), logFile);
		}else{
			writeLog(NO_ACTION, 1, strlen(bufSBW), logFile);
		}
		//Viene pulito l'array
		memset(bufSBW, 0, sizeof(bufSBW));
		sleep(1);
	}
	//Viene chiusa la connesione alla socket
	closeClientConnection(sockFd);
	//Viene chiuso il file di log
	closeLog(logFile);
	return 0;
}

