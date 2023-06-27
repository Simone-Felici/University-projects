#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

#include "executable.h"

//Funzione creata per avere un codice più pulito
int execute(char *path, char *executableName) {
	printf("%s in esecuzione\n", executableName);
	return execlp(path, executableName,NULL);
	perror("Errore nell'esecuzione di In");
	exit(EXIT_FAILURE);
}
