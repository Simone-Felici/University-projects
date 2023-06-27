#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

int connectTo(char *msg);
int sendMessageToServer(int toFileDescriptor, void *fromBuffer);
char* receiveMessageFromServer(int fromFileDescriptor);
void createNonBlockReadClient(int fileDescriptor);
void closeClientConnection(int sockfd);

#endif  // SOCKET_H

