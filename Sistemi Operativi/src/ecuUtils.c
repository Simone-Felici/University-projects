#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include<string.h>
#include <signal.h>

#include "ecuUtils.h"
#include "logManagement.h"
#include "socketServer.h"

/*Funzione utilizzata per controllare se, data una stringa, questa
 * contiene un numero. La funzione sscanf viene utilizzata per tentare
 * di leggere un intero (%d) dalla stringa str. Il valore di ritorno di
 * sscanf indica il numero di conversioni riuscite che ha effettuato*/
bool containNumber(char *str) {
	int num;
	if (sscanf(str, "%d", &num) == 1) {
		return true;
	}
	return false;
}

/*Funzione che, data una stringa di caratteri, ritorna l'intero contenuto nella stringa*/

int takeNumberFromString(char *str) {
	int num;
	sscanf(str, "%d", &num);
	return num;
}

/*Funzione utilizzata per controllare se il numero ricevuto da Front Windshield Camera è
 * maggiore della velocità attuale, in questo caso:
 * 1)invia il valore a throttle control
 * 2)invia il valore ad Out
 * 3)scrive nel file di log della Ecu
 * 4)aumenta la velocità */
int increase(int num, int velocity, int toFD, char *msg, int outFD, FILE *logFile) {
	if (num > velocity) {
		sendMessageToClient(toFD, msg);
		sendMessageToClient(outFD, msg);
		writeLog(msg, 1, strlen(msg), logFile);
		velocity += 5;
		sleep(1);
	}
	return velocity;
}

/**Funzione utilizzata per controllare se il numero ricevuto da Front Windshield Camera è
 * minore della velocità attuale, in questo caso:
 * 1)invia il valore a Break By Wire
 * 2)invia il valore ad Out
 * 3)scrive nel file di log della Ecu
 * 4)diminuisce la velocità*/
int decrease(int num, int velocity, int toFD, char *msg, int outFD,
		FILE *logFile) {
	if (num < velocity) {
		sendMessageToClient(toFD, msg);
		sendMessageToClient(outFD, msg);
		writeLog(msg, 1, strlen(msg), logFile);
		velocity += -5;
		sleep(1);
	}
	return velocity;
}

/*Se la Ecu riceve "DESTRA" da Front Windshield Camera allora per 4 secondi
 * viene:
 * 1)inviato il messaggio a Steer By Wire
 * 2)inviato il messaggio ad Out
 * 3)scritto nel file di log della Ecu*/
void moveRight(int *fileDescriptor, char *DESTRA, FILE *logFile) {
	for (int i = 0; i < 4; i++) {
		sendMessageToClient(fileDescriptor[5], DESTRA);
		sendMessageToClient(fileDescriptor[1], DESTRA);
		writeLog(DESTRA, 1, strlen(DESTRA), logFile);
		sleep(1);
	}
}

/*Se la Ecu riceve "SINISTRA" da Front Windshield Camera allora per 4 secondi
 * viene:
 * 1)inviato il messaggio a Steer By Wire
 * 2)inviato il messaggio ad Out
 * 3)scritto nel file di log della Ecu*/
void moveLeft(int *fileDescriptor, char *SINISTRA, FILE *logFile) {
	for (int i = 0; i < 4; i++) {
		sendMessageToClient(fileDescriptor[5], SINISTRA);
		sendMessageToClient(fileDescriptor[1], SINISTRA);
		writeLog(SINISTRA, 1, strlen(SINISTRA), logFile);
		sleep(1);
	}
}

/*Se la Ecu legge/riceve ARRRESTO o PERICOLO allora viene:
 *1)inviato un segnale a breakByWire
 *2)scitto nel file di log della Ecu*/
void breakProcedure(int Pid, int fileDescriptor, char *bufIn, FILE *logFile) {
	kill(Pid, SIGUSR1);
	sendMessageToClient(fileDescriptor, bufIn);
	writeLog(bufIn, 1, strlen(bufIn), logFile);
}

void resizeData(char *str, int length){
	 int i;
	 for (i = 0; i < length; i += 8) {
	 char group[9];
	 strncpy(group, str + i, 8);
	 group[8] = '\0';
	 printf("SurroundVC data: %s\n", group);
	 }
}

/*Funzione utilizzata dalla Ecu durante la procesura di parcheggio*/
void parkingProcedure(int *sonsPid, int *fileDescriptor, int velocity, char *FRENO_5, FILE *logFile, char *adasMode) {
	//Viene inviato un messagio ad Out
	sendMessageToClient(fileDescriptor[1], "Parking...\n");

	char bufPA[100];
	char *msgPA;
	//Viene decrementata la velocità fino a 0
	do {
		velocity = decrease(0, velocity, fileDescriptor[3], FRENO_5, fileDescriptor[1], logFile);
	} while (velocity > 0);
	//Viene fatta una fork per creare il figlio Park Assist e farlo connettere tramite socket
	sonsPid[6] = fork();
	if (sonsPid[6] == 0) {
		sleep(1);
		execute("./bin/parkAssist", "parkAssist");
	} else {
		fileDescriptor[7] = acceptConnection(fileDescriptor[8]);
		printf("PA connesso\n");
		//Viene inviata la modalità (NORMALE o ARTIFICIALE) al processo Park Assist
		sendMessageToClient(fileDescriptor[7], adasMode);
		//Vengono uccisi tutto i processi tranne Park Assist, Out, In, ed Ecu
		for (int i = 0; i < 6; i++) {
			kill(sonsPid[i], SIGKILL);
		}
		printf("Kill all\n");

		int timeElapsed = 0;
		while (1) {
			do {
				sleep(1);
				//Si attende messagi da Park Assist o da Surround View Camera
				msgPA = receiveMessageFromClient(fileDescriptor[7]);
				strcpy(bufPA, msgPA);
				free(msgPA);

				if(strlen(bufPA) > 9){
					resizeData(bufPA, strlen(bufPA));
				}else{
				printf("ParkAssist data: %s\n", bufPA);
				}
				timeElapsed++;

				/*Se il messaggio ricevuto è corretto e se sono passati 30 secondi allora esci e termina il
				  parcheggio*/
				if (dataErroreNotRecived(bufPA) && timeElapsed > 31) {
					printf("Park complete\n");
					sendMessageToClient(fileDescriptor[7], "Park complete");
					//Uccidi Park Asssist
					kill(sonsPid[6], SIGKILL);
					//Invia ad out il messagio di fine parcheggio
					sendMessageToClient(fileDescriptor[1], "Park complete");
					waitpid(sonsPid[6], NULL, 0);
					exit(0);
				}

			} while (dataErroreNotRecived(bufPA)); //Il ciclo si ripete finche riceve dati corretti
			//Se si ricevono dati errati allora viene azzerato il tempo e riparte il ciclo interno
			printf("DATA ERROR!!!\n Restart Parking...\n");
			timeElapsed = 0;
		}
	}
}

/*Controlla se all'interno della stringa di caratteri non via contenuto uno
 * dei valori non validi controllando indice per indice*/
bool dataErroreNotRecived(char *msg) {
	char *unvalidStrings[] = { "172A", "D693", "0000", "BDD8", "FAEE", "4300" };
		int numValidStrings = sizeof(unvalidStrings) / sizeof(unvalidStrings[0]);

		for (int i = 0; i < numValidStrings; i++) {
			if (strstr(msg, unvalidStrings[i]) != NULL) {
				return false;
			}
		}
		return true;
}

/*Funzione per controllare l'uguaglianza fra stringhe (creata per rendere il codice più pulito)*/
bool recivedFrom(char *stringToCompare, char *fromBuffer) {
	if (strncmp(fromBuffer, stringToCompare, strlen(stringToCompare) + 1)
			== 0) {
		return true;
	}
	return false;
}

