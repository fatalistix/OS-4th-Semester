#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


int errorHandler(char * message, char * programName)
{
    char error[256];
    sprintf(error, "%s: ", programName);
    strcat(error, message);
    perror(error);
    return errno;
}

int main(int argc, char * argv[])
{
    char * serverIpAddr = "127.0.0.1";
    char message[256] = "";
    short serverPort = 6969;
    struct sockaddr_in serverSockAddr;
    int csd;

    if (argc > 1)
    {
        serverIpAddr = argv[1];
    }
    if (argc > 2)
    {
        serverPort = atoi(argv[2]);
    }

    printf("%s: configuring server data connection with ip:port = %s:%d...\n", argv[0], serverIpAddr, serverPort);

    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port   = htons(serverPort);
    int ptonStatus = inet_pton(AF_INET, serverIpAddr, &(serverSockAddr.sin_addr));
    if (ptonStatus == 0)
    {
        fprintf(stderr, "%s: invalid ip address %s\n", argv[0], serverIpAddr);
        return EINVAL;
    }
    if (ptonStatus == -1)
    {
        return errorHandler("error writing ip address", argv[0]);
    }



    csd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (csd == -1)
    {
        return errorHandler("cannot create udp socket", argv[0]);
    }

    printf("%s: socket created\n", argv[0]);

    while (1)
    {
        printf("%s: enter your message (255 symbols max): ", argv[0]);
        fgets(message, 255, stdin);
        printf("%s: sending message to %s...\n", argv[0], serverIpAddr);
    
        int messageLength = strlen(message);
        message[messageLength - 1] = '\0';
        --messageLength;
    
        int sendMessageLength = sendto(csd, message, messageLength + 1, 0, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr));
        if (sendMessageLength == -1)
        {
            int error = errorHandler("error sending datagram", argv[0]);
            close(csd);
            return error;
        }
        
        printf("%s: sent message size = %d\n", argv[0], sendMessageLength);
        printf("%s: waiting for echo reply...\n", argv[0]);
    
        struct sockaddr_in recvSockAddr;
        unsigned int recvSockAddrSize = sizeof(recvSockAddr);
        char recvBuffer[256] = "";
        int recvMessageLength = recvfrom(csd, recvBuffer, sendMessageLength, 0, (struct sockaddr *) &recvSockAddr, &recvSockAddrSize);
        if (recvMessageLength == -1)
        {
            int error = errorHandler("error recieving message", argv[0]);
            close(csd);
            return error;
        }
    
        printf("%s: echo reply from %s: %s\n", argv[0], inet_ntoa(recvSockAddr.sin_addr), recvBuffer);

    }
    
    close(csd);

    return 0;
}
