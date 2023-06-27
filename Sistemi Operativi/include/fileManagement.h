#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <stdbool.h>

FILE* openFile(char *filePath, char *option);
void readData(int fd, FILE *logFile, time_t duration, char *who, char* modality);
void convertExToAscii(char* buf, char *asciiBuffer);



#endif
