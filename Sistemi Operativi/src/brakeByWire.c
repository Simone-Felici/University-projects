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

#define LOG_FILE "log/brake.log"
void signalHandler(int sigNum);
FILE *logFile;

int main() {
	char *msg;
	char bufBBW[6];
	//Viene richiesta una connesione al socket ECU
	int sockFd = connectTo("ECU");
	//Viene aperto il file di log
	logFile = openLog(LOG_FILE);
	//Viene gestita la kill dalla Ecu nel caso in di pericolo
	signal(SIGUSR1, signalHandler);

	while (1) {
		//Riceve il messaggio di fernare dalla Ecu
		msg = receiveMessageFromServer(sockFd);
		strcpy(bufBBW, msg);
		free(msg);
		//Controlla se il messaggio è "FRENO 5"
		if (strncmp(bufBBW, "FRENO 5", 7) == 0) {
			//Viene scritta la data e "FRENO 5" nel file di log
			writeLog(__DATE__, 1, 11, logFile);
			writeLog(bufBBW, 1, strlen(bufBBW), logFile);
		}
		//Viene pulito l'array
		memset(bufBBW, 0, sizeof(bufBBW));
	}
	//Viene chiusa la connesione con la socket
	closeClientConnection(sockFd);
	//Viene chiuso il file descriptor
	closeLog(logFile);
	return 0;
}

/*In caso di pericolo la Ecu invia un segnale a questo processo e quindi gestito
  dal signal handler sottostante */
void signalHandler(int sigNum) {
	char *msg = "ARRESTO";
	writeLog(msg, 1, strlen(msg), logFile);
}
