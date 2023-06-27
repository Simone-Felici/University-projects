#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>

#include "logManagement.h"
#include "fileManagement.h"
#include "socketClient.h"

#define LOG_FILE "log/cameras.log"

int main() {
	//Viene stabilità la connessione al server "ParkAssist"
	int sockFd = connectTo("ParkAssist");
	//Viene aperto il file di log
	FILE *logFile = openLog(LOG_FILE);
	char *adasModeRecived;
	char adasMode[12];
	//Attende la ricezione della modalità da Park Asssit (NORMALE o ARTIFICIALE)
	adasModeRecived = receiveMessageFromServer(sockFd);
	strcpy(adasMode, adasModeRecived);
	free(adasModeRecived);
	//Viene letto iterativamente dalla sorgente
	while (1) {
		readData(sockFd, logFile, 30, "surroundViewCamera", adasMode);
	}
	//Viene chiusa la connessione con la socket
	closeClientConnection(sockFd);
	//Viene chiuso il file di log
	closeLog(logFile);
	return 0;
}
