#ifndef LOGMANAGEMENT_H
#define LOGMANAGEMENT_H

FILE* openLog(char *path);
int writeLog(char *buffer, int i, int bufferLength, FILE *logFile);
void closeLog(FILE *logFile);


#endif
