#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>

#include "logManagement.h"
#include "socketClient.h"
#include "fileManagement.h"

#define DATA_FILE "bin/fileUtils/frontCamera.data"
#define LOG_FILE "log/camera.log"

int main() {
	//Viene stabilità la connesione con ECU
	int sockfd = connectTo("ECU");
	//Viene aperto il file di log
	FILE *logFile = openLog(LOG_FILE);
	FILE *dataFile;

	while (1) {
		//Viene aperto il file frontCamera.data in modilità lettura
		dataFile = openFile(DATA_FILE, "r");
		char buffer[11];
		//Cicla finchè viene letta una riga dal file frontCamera.data
		while (fgets(buffer, sizeof(buffer), dataFile) != NULL) {
			//Viene rimosso il carattere 'newline' e sostituito con \0
			buffer[strcspn(buffer, "\n")] = '\0';
			//Viene inviata la riga di dati ad Ecu
			sendMessageToServer(sockfd, buffer);
			//Vengono scritti i dati nel file di log
			writeLog(buffer, 1, strlen(buffer), logFile);
			sleep(1);
		}
		//Viene chiuso il file
		fclose(dataFile);
	}
	//Viene chiuso il file di log
	closeLog(logFile);
	//Viene chiusa la connessione al socket
	closeClientConnection(sockfd);
	return 0;
}
