#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "stringChecking.h"

char* readLine(int fd) {
	//capicity è la capacità dell'array, viene messa a 10 percgè in media i messaggi non sono più grandi di 10 byte
	int capacity = 10;
	int size = 0;
	//Viene allocata memoria dinamicamente della dimensione di 10 char
	char *buffer = (char *) malloc(capacity * sizeof(char));

	if(buffer == NULL){
		perror("Malloc Error");
	}
	int n;
	char c;
	//Viene fatta una read di 1 byte alla volta
	while ((n = read(fd, &c, 1)) > 0) {
		//Se viene letto il carattere di fine stringa allora il ciclo termina
		if (c == '\0') {
			break;
		}
		//Viene meorizzato il carattere nel buffer
		buffer[size++] = c;
		//Se la grandezza del buffer non dovesse essere sufficiente
		if (size == capacity) {
			// Raddoppia la capacità dell'array se necessario
			capacity *= 2;
			// Rialloca l'array con la nuova capacità
			buffer = realloc(buffer, capacity * sizeof(char));
		}
	}
	// Ridimensiona l'array alla dimensione effettiva + 1 per il terminatore di fine stringa
	buffer = realloc(buffer, (size + 1) * sizeof(char));
	// Aggiunge il terminatore nullo alla fine dell'array
	buffer[size] = '\0';
	return buffer;
}

