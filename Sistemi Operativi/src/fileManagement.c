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

#include "fileManagement.h"
#include "logManagement.h"
#include "socketClient.h"

#define DATA_SIZE 8

/*Funzione utilizzata per l'apertura di un file*/
FILE* openFile(char *filePath, char *option) {
	FILE *dataFile;
	if ((dataFile = fopen(filePath, option)) == NULL) {
		perror("Errore durante l'apertura del file di dati");
		exit(EXIT_FAILURE);
	}
	return dataFile;
}

/*Funzione utilizzata per leggere da una sorgente di dati 8 byte alla volta*/
void readData(int fd, FILE *logFile, time_t duration, char *who, char *modality) {
	char *path;
	int deviceFd;
	//In base alla modalità viene eseguita la lettura da una sorgente diversa
	if (strncmp(modality, "NORMALE", 8) == 0) {
		path = "/dev/urandom";
		deviceFd = open(path, O_RDONLY, S_IRUSR | S_IWUSR);
	} else if (strncmp(modality, "ARTIFICIALE", 12) == 0) {
		path = "bin/fileUtils/urandomARTIFICIALE.binary";
		deviceFd = open(path, O_RDONLY, S_IRUSR | S_IWUSR);
	}
	if (deviceFd == -1) {
		perror("Error in the opening of the device.");
		exit(0);
	}
	/*Dichiarazione delle variabili buffer per i dati letti, bytes read per il numero di dati letti
	 * e ascciBuffer per i dati esadecimali convertiti in ascii*/
	char buffer[DATA_SIZE];
	int bytesRead;
	char asciiBuffer[2 * DATA_SIZE + 1];
	/*Viene acquisito il tempo corrente in start time e calcolato il tempo finale in endTime*/
	time_t startTime = time(NULL);
	time_t currentTime;
	time_t endTime = startTime + duration;
	/*Viene controllato se il componente che usa questa funzioneè parkAssist o no, nel primo caso:
	 * vengono letti byte senza controllare che ne abbia letti esattamente 8 */
	if (strncmp(who, "ParkAssist", 11) == 0) {
		/*Finchè non sono passati n secondi (in base a duration) in seguito:
		 * 1) vengono letti byte dalla sorgente
		 * 2) convertiti in ascii
		 * 3) inviati al server
		 * 4) scritti nel file di log*/
		while ((currentTime = time(NULL)) <= endTime) {
			read(deviceFd, buffer, DATA_SIZE);
			convertExToAscii(buffer, asciiBuffer);
			sendMessageToServer(fd, asciiBuffer);
			writeLog(asciiBuffer, 1, strlen(asciiBuffer), logFile);
			sleep(1);
			memset(buffer, 0, sizeof(buffer));
			memset(asciiBuffer, 0, sizeof(asciiBuffer));

		}
	} else if (strncmp(who, "surroundViewCamera", 19) == 0) {
		/*Iterativamente vengono letti dati dalla sorgente e questi:
		 * 1) vengono letti byte dalla sorgente e controllo di averne letty 8 byte
		 * 2) convertiti in ascii
		 * 3) inviati al server
		 * 4) scritti nel file di log*/
		char concatenatedBuffer[DATA_SIZE * duration];
		concatenatedBuffer[0] = '\0';
		while ((currentTime = time(NULL)) <= endTime) {
			bytesRead = read(deviceFd, buffer, DATA_SIZE);
			if (bytesRead == DATA_SIZE) {
				convertExToAscii(buffer, asciiBuffer);
				strcat(concatenatedBuffer, asciiBuffer);
				writeLog(asciiBuffer, 1, strlen(asciiBuffer), logFile);
			}
			sleep(1);
		}
		sendMessageToServer(fd, concatenatedBuffer);
	} else {
		while (1) {
			bytesRead = read(deviceFd, buffer, DATA_SIZE);
			if (bytesRead == DATA_SIZE) {
				convertExToAscii(buffer, asciiBuffer);
				sendMessageToServer(fd, asciiBuffer);
				writeLog(asciiBuffer, 1, strlen(asciiBuffer), logFile);
			}
			sleep(1);

		}
	}
}

//Funzione per convertire i dati letti dal dispositivo (contenuti nel buffer buffer) in formato ASCII prima di inviarli al server
/* La funzione "sprintf" viene utilizzata per formattare i dati di input nel buffer buf[i] come una stringa esadecimale
 a due cifre (%02X) e memorizzarla nel buffer &asciiBuffer[i * 2]. Questo significa che ogni
 byte del buffer buf verrà convertito in una stringa esadecimale di due caratteri e
 verrà posizionato consecutivamente nel buffer asciiBuffer.*/
void convertExToAscii(char *buf, char *asciiBuffer) {
	int i;
	for (i = 0; i < DATA_SIZE; i++) {
		sprintf(&asciiBuffer[i * 2], "%02X", buf[i]);
	}
	asciiBuffer[DATA_SIZE] = '\0';
}
