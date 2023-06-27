#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#include "logManagement.h"
#include "fileManagement.h"
#include "socketClient.h"
#include "socketServer.h"
#include "executable.h"

#define LOG_FILE "log/assist.log"

int main() {
	char *adasModeRecived;
	char adasMode[12];
	//fileDescriptor ={[Ecu][SurroundWC][ParkA]}
	int fileDescriptor[3];
	//Viene stabilità la connesione alla ECU
	fileDescriptor[0] = connectTo("ECU");
	//Attende la ricezione della modalità (NORMALE o ARTIFICIALE)
	adasModeRecived = receiveMessageFromServer(fileDescriptor[0]);
	strcpy(adasMode, adasModeRecived);
	free(adasModeRecived);
	//Viene creata una socket server di nome "ParkAssist"
	fileDescriptor[2] = createSocketSever("ParkAssist");
	//Viene aperto il file di log
	FILE *logFile = openLog(LOG_FILE);
	char *bufMsg;
	char msg[11];
	char *endMsg;
	char *endingMsg[15];

	createNonBlockReadClient(fileDescriptor[0]);
	//Viene creato il figlio surround view camera ed accettata la connesione
	int svcPid = fork();
	if (svcPid == 0) {
		execute("./bin/surroundViewCamera", "surroundViewCamera");
	} else {
		fileDescriptor[1] = acceptConnection(fileDescriptor[2]);
		printf("SWC connesso\n");
		//Viene inviata la modalità al figlio surround view camera
		sendMessageToClient(fileDescriptor[1], adasMode);

		while (1) {
			//Vengono letti i dati dalla sorgente
			readData(fileDescriptor[0], logFile, 30, "ParkAssist", adasMode);
			//Attende la ricezione dei dati da surround view camera
			bufMsg = receiveMessageFromClient(fileDescriptor[1]);
			strcpy(msg, bufMsg);
			free(bufMsg);
			//I dati di surround view camera vengono inviati alla Ecu
			sendMessageToServer(fileDescriptor[0], msg);
			//In caso in cui il parcheggio finisca riceve il messagio per uccidiere Surround View Camera
			endMsg = receiveMessageFromServer(fileDescriptor[0]);
			strcpy(endingMsg, endMsg);
			free(endMsg);
			if (strncmp(endingMsg, "Park complete", strlen(endingMsg) + 1) == 0) {
			kill(svcPid, SIGKILL);
			}
			sleep(2);
		}
	}
	//viene chiusa la connesione con la socket
	closeClientConnection(fileDescriptor[0]);
	//Viene chiuso il file di log
	closeLog(logFile);
	return 0;
}
