#ifndef ECUUTILS_H
#define ECUUTILS_H
#include <stdbool.h>

bool containNumber(char *str);
int takeNumberFromString(char *str);
int increase(int num, int velocity, int toFD, char *msg, int outFD, FILE *logFile);
int decrease(int num, int velocity, int toFD, char *msg, int outFD, FILE *logFile);
int forcedDecrease(int velocity, int toFD, char *msg, int outFD, FILE *logFile);
void moveRight(int *fileDescriptor, char *DESTRA, FILE *logFile);
void moveLeft(int *fileDescriptor, char *DESTRA, FILE *logFile);
void breakProcedure(int Pid, int fileDescriptor, char * bufIn, FILE *logFile);
void parkingProcedure(int *sonsPid, int *fileDescriptor, int velocity, char *FRENO_5, FILE *logFile, char *adasMode);
bool dataErroreNotRecived(char *msg);
bool recivedFrom(char *stringToCompare, char *fromBuffer);


#endif
