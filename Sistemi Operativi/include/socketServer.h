#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

int createSocket(char *msg);
int acceptConnection(int fd);
int sendMessageToClient(int toFileDescriptor, void *fromBuffer);
char* receiveMessageFromClient(int fromFileDescriptor);
void createNonBlockReadServer(int fileDescriptor);
void closeSocketServer(int sockfd);

#endif  // SOCKET_H


