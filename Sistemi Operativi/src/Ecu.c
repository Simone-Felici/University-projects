#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
//Librerie personali
#include "stringChecking.h"
#include "executable.h"
#include "ecuUtils.h"
#include "socketServer.h"
#include "logManagement.h"
//Definizione file di log
#define LOG_FILE "log/ECU.log"
//signalHandler per l'arresto da throttleControl.c
void signalHandler(int sigNum);

FILE *logFile;
int velocity = 0;
int fdOut;

int main(int argc, char *argv[]) {
	//fileDescriptor = {[In][Out][FrontWC][BreakBW][ThrottleC][SteerBW][ForwardFR][ParkA][CentralEcu]}
	int fileDescriptor[9];
	//fileDescriptor = {[In][BreakBW][ThrottleC][ForwardFR][SteerBW][ForwardFR][ParkA]}
	int sonsPid[8];
	//signalHandler per l'arresto da throttleControl.c
	signal(SIGUSR2, signalHandler);
	//I messagi ricevuti dalle socket vengono ricevuti su *msg* e poi copiati in buf*
	char bufIn[11];
	char bufOut[11];
	char bufFWC[11];
	char bufFFR[11];
	char bufPA[11];
	char *msgIn;
	char *msgFWC;
	char *msgFFR;
	char *msgPA;
	char *adasMode;
	//Variabili per una migliore lettura del codice (vedi riga 176)
	int num;
	char *FRENO_5 = "FRENO 5";
	char *INCREMENTO_5 = "INCREMENTO 5";
	char *DESTRA = "STO GIRANDO A DESTRA";
	char *SINISTRA = "STO GIRANDO A SINISTRA";

	/*Controlla se all'eseguibile è stata passata la stringa corretta
    per settare, tramite una stringa, la modalità */

	if (strncmp(argv[1], "NORMALE", 8) == 0) {
		adasMode = "NORMALE";
	} else if (strncmp(argv[1], "ARTIFICIALE", 12) == 0) {
		adasMode = "ARTIFICIALE";
	}

	printf("Start\n");

	//Viene aperto il file di log
	logFile = openLog(LOG_FILE);
	//Viene creato il socket server che con una coda di massimo 8 processi
	fileDescriptor[8] = createSocketSever("ECU");
	//Viene accettata la connessione al processo OUT
	fileDescriptor[1] = acceptConnection(fileDescriptor[8]);
	//Viene salvato il fileDescriptor per usarlo nel signalHandler dato che fdOut è variabile globale
	fdOut = fileDescriptor[1];
	printf("Out Connesso\n");
	//Viene eseguita la fork per creare In
	sonsPid[0] = fork();

	if (sonsPid[0] == 0) {
		execute("./bin/In", "In");
	} else {
		//Viene accettata la connesione da In
		fileDescriptor[0] = acceptConnection(fileDescriptor[8]);
		printf("IN connesso\n");
		//Una volta che In è connesso si mette in attesa di ricevere "INIZIO"
		do {
			msgIn = receiveMessageFromClient(fileDescriptor[0]);
		} while (!recivedFrom(msgIn, "INIZIO"));

		velocity = 0;
		/*Viene copiata la stringa ricevuta ("INIZIO") su un altra stringa
		  e liberata l'area di memoria usata da msgIn (si veda stringChecking.c) */
		strcpy(bufIn, msgIn);
		free(msgIn);
		writeLog(bufIn, 1, strlen(bufIn), logFile);

		//Invia la stringa "INIZIO" ad Out
		sendMessageToClient(fileDescriptor[1], bufIn);
		/*A questo punto il programma è pronto per iniziare e quindi genera tutti i processi
		  di cui ha bisgono tranne "Park Assist" e "SurroundViewCamera".
		  Viene creato un processo alla volta, viene eseguito in parallelo tramite execute
		  ed il padre (la central Ecu) attende di connettersi ad ognuno di essi */
		sonsPid[1] = fork();
		if (sonsPid[1] == 0) {
			execute("./bin/brakeByWire", "brakeByWire");
		} else {
			fileDescriptor[3] = acceptConnection(fileDescriptor[8]);
			printf("BBW connesso\n");
			sonsPid[2] = fork();
			if (sonsPid[2] == 0) {
				execute("./bin/throttleControl", "throttleControl");
			} else {
				fileDescriptor[4] = acceptConnection(fileDescriptor[8]);
				printf("TC connesso\n");
				sonsPid[3] = fork();
				if (sonsPid[3] == 0) {
					execute("./bin/frontWindShieldCamera",
							"frontWindShieldCamera");
				} else {
					fileDescriptor[2] = acceptConnection(fileDescriptor[8]);
					printf("FWD connesso\n");
					sonsPid[4] = fork();

					if (sonsPid[4] == 0) {
						execute("./bin/steerByWire", "steerByWire");
					} else {
						fileDescriptor[5] = acceptConnection(fileDescriptor[8]);
						printf("SBW connesso\n");
						sonsPid[5] = fork();

						if (sonsPid[5] == 0) {
							execute("./bin/forwardFacingRadar",
									"forwardFacingRadar");
						} else {
							fileDescriptor[6] = acceptConnection(fileDescriptor[8]);
							printf("FFR connesso\n");

							//Invia a Forward Facing Radar, la stringa della modalità ("NORMALE" o "ARTIFICIALE")
							sendMessageToClient(fileDescriptor[6], adasMode);
							//Le read su In e su Forward Facing Radar vengono messe come non bloccanti
							createNonBlockReadServer(fileDescriptor[0]);
							createNonBlockReadServer(fileDescriptor[6]);

							while (1) {
								//Attende un messaggio da In (read non bloccante)
								msgIn = receiveMessageFromClient(fileDescriptor[0]);
								strcpy(bufIn, msgIn);
								free(msgIn);
								/*Se il messagi ricevuto da in è ARRESTO o PARCHEGGIO
								  vengono eseguite le rispettive procedure (vedi ecuUtils.c) */
								if (recivedFrom(bufIn, "ARRESTO")) {
									velocity = 0;
									breakProcedure(sonsPid[1], fileDescriptor[1], bufIn, logFile);
								} else if (recivedFrom(bufIn, "PARCHEGGIO")) {
									parkingProcedure(sonsPid, fileDescriptor,velocity, FRENO_5, logFile, adasMode);
								}

								//Ricevo dati da Forward Facing Radar
								msgFFR = receiveMessageFromClient(fileDescriptor[6]);
								strcpy(bufFFR, msgFFR);
								free(msgFFR);

								//Viene ricevuto un messaggio da Forward Facing Radar
								msgFWC = receiveMessageFromClient(fileDescriptor[2]);
								strcpy(bufFWC, msgFWC);
								free(msgFWC);
								//Tutto ciò che viene ricevuto da Forward Facing Radar viene poi inviato ad Out
								sendMessageToClient(fileDescriptor[1], bufFWC);
								writeLog(bufFWC, 1, strlen(bufFWC), logFile);
								/*Viene in primis controllato se ciò che abbiamo ricevuto da Forward Facing Radar è
								 * un numero, in questo caso viene convertita la stringa ricevuta in un numero, poi
								 * avviata la proceduta a seconda che il numero  sia maggiore o minore della velocità corrente,
								 * questo viene fatto fino a che non si raggiunge la velocità desiderata.
								 * Altrimenti, se viene ricevuta una stringa, si avvia la seguente procedura in base a quale
								 * stringa abbiamo ricevuto */
								if (containNumber(bufFWC)) {
									num = takeNumberFromString(bufFWC);
									do {
										velocity = increase(num, velocity, fileDescriptor[4], INCREMENTO_5, fileDescriptor[1], logFile);
										velocity = decrease(num, velocity,fileDescriptor[3], FRENO_5, fileDescriptor[1], logFile);
									} while (velocity < num || velocity > num);
								} else {
									//Ramo in cui sappiamo di aver ricevuto una stringa
									if (recivedFrom(bufFWC, "DESTRA")) {
										moveRight(fileDescriptor, DESTRA,logFile);
									} else if (recivedFrom(bufFWC,"SINISTRA")) {
										moveLeft(fileDescriptor, SINISTRA,logFile);
									} else if (recivedFrom(bufFWC,"PERICOLO")) {
										velocity = 0;
										breakProcedure(sonsPid[1],fileDescriptor[1], bufFWC,logFile);
									} else if (recivedFrom(bufFWC,"PARCHEGGIO")) {
										parkingProcedure(sonsPid, fileDescriptor, velocity, FRENO_5, logFile, adasMode);
									}
									sleep(1);
								}
								//Vengono svuotati gli array
								memset(bufIn, 0, sizeof(bufIn));
								memset(bufOut, 0, sizeof(bufIn));
								memset(bufFWC, 0, sizeof(bufFWC));
								memset(bufFFR, 0, sizeof(bufFFR));
							}
						}
					}
				}
			}
		}
	}
	//Viene chiusa la connesione socket AFUNIX
	closeSocketServer(fileDescriptor[8]);
	//Viene chiuso il file di Log
	closeLog(logFile);
	return 0;
}

//signalHandler per l'arresto da throttleControl.c per l'arresto totale (si veda "facoltativo 8)"
void signalHandler(int sigNum) {
	velocity = 0;
	char *ARRESTO = "ARRESTO TOTALE";
	writeLog(ARRESTO, 1, strlen(ARRESTO), logFile);
	sendMessageToClient(fdOut, ARRESTO);
	exit(0);
}

