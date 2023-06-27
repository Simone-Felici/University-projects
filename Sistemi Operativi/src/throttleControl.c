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
#include <time.h>

#include "logManagement.h"
#include "socketClient.h"

#define LOG_FILE "log/throttle.log"

void probabilityOfBrake(FILE *logFile){
		srand(time(NULL));
		//Viene creata la variabile per stabilire la proabilità (10^-5)
		double probability = 1e-5;
		// Genera un numero casuale compreso tra 0 e 1 perchè RAND_MAX è il valore massimo
		// che rand può generare
		double randomValue = (double) rand() / RAND_MAX;

		//Se il numero casuale è minore della probabilità avviamo l'ARRESTO TOTALE
		if (randomValue < probability) {
			printf("ARRESTO TOTALE\n");
			//Viene scritto nel file di log
			writeLog("ACCELLERAZIONE FALLITA", 1, 11, logFile);
			// Invia il segnale di ARRESTO TOTALE alla Central ECU
			kill(getppid(), SIGUSR2);
		}
}

int main() {
	char *msg;
	char bufTC[6];
	//Viene stabilita la connessione alla Ecu
	int sockFd = connectTo("ECU");
	//Viene chiuso il file di log
	FILE *logFile = openLog(LOG_FILE);

	while (1) {
		//Attende la ricezione del messaggio dalla Ecu
		msg = receiveMessageFromServer(sockFd);
		strcpy(bufTC, msg);
		free(msg);
		//Se viene ricevuto "INCREMENTO %"
		if (strncmp(bufTC, "INCREMENTO 5", 13) == 0) {
			probabilityOfBrake(logFile);
			//Viene scritta la data nel file di log e il messaggio ricevuto
			writeLog(__DATE__, 1, 11, logFile);
			writeLog(bufTC, 1, strlen(bufTC), logFile);
		}
		//Viene pulito l'array
		memset(bufTC, 0, sizeof(bufTC));
	}
	//Viene chiusa la connessione con la socket
	closeClientConnection(sockFd);
	//Viene chiuso il file di log
	closeLog(logFile);
	return 0;
}

