#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

#include "logManagement.h"

/*Questa funzione prende il percorso del file di log come argomento e restituisce
 * un puntatore a un oggetto di tipo FILE. La funzione crea un nuovo file di log
 * utilizzando la modalità di apertura "w" (scrittura) tramite la funzione fopen*/
FILE* openLog(char *path) {
	FILE *logFile;

	//Prima creo il file .log
	if ((logFile = fopen(path, "w")) == NULL) {
		perror("Errore durante l'apertura del file di log");
		exit(EXIT_FAILURE);
	}
	return logFile;
}

/* La funzione scrive i dati presenti nel buffer nel file di log utilizzando fwrite, specificando la dimensione di ogni elemento (1 byte nel caso specifico),
 *  la quantità di elementi da scrivere (ottenuta utilizzando strlen(buffer)), e il puntatore al file di log.
 *  Successivamente, viene scritto un carattere di nuova riga ('\n') nel file di log utilizzando fputc per separare i dati scritti.
 *  Infine, fflush viene utilizzato per assicurare che i dati siano effettivamente scritti nel file. */
int writeLog(char *buffer, int i, int bufferLength, FILE *logFile) {
	int size = fwrite(buffer, 1, strlen(buffer), logFile);
	fputc('\n', logFile);
	fflush(logFile);
	return size;
}

//Funzione per chiudere i file di log
void closeLog(FILE *logFile) {
	close(logFile);
}

