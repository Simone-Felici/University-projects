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

#define LOG_FILE "log/radar.log"

int main() {

	char *recivedAdasModality;
	char adasModality[12];
	//Viene connesso al socket ECU
	int sockFd = connectTo("ECU");
	//Viene aperto il file di log
	FILE *logFile = openLog(LOG_FILE);
	//Attende la ricezione del messaggio "NORMALE" o "ARTIFICIALE" per la modalità
	recivedAdasModality = receiveMessageFromServer(sockFd);
	strcpy(adasModality, recivedAdasModality);
	free(recivedAdasModality);

	//Una volta al secondo legge i dati dalla specifica sorgente
	while(1){
	readData(sockFd, logFile, 1, "forwardFacingRadar", adasModality);
	}
	//Viene chiusa la connesione con la socket
	closeClientConnection(sockFd);
	//Viene chiuso il file di log
	closeLog(logFile);
	return 0;
}
