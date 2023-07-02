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
    struct sockaddr_in serverSockAddr;
    int ssd;
    short serverPort = 6969;

    if (argc > 1)
    {
        serverIpAddr = argv[1];
    }
    if (argc > 2)
    {
        serverPort = atoi(argv[2]);
    }

    printf("%s: configuring server socket with ip:port = %s:%d...\n", argv[0], serverIpAddr, serverPort);

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



    ssd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ssd == -1)
    {
        return errorHandler("cannot create udp socket", argv[0]);
    }

    printf("%s: socket created\n", argv[0]);
    
    if (bind(ssd, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) == -1)
    {
        int error = errorHandler("error binding socket", argv[0]);
        close(ssd);
        return error;
    }

    printf("%s: bind completed\n", argv[0]);

    printf("%s: Listening for messages...\n", argv[0]);


    char recvBuffer[256] = "";
    char sendBuffer[256] = "";
    struct sockaddr_in senderSockAddr;
    unsigned int senderSockAddrSize;
    int senderMessageLength;
    while (1)
    {
        senderMessageLength = recvfrom(ssd, recvBuffer, 256, 0, 
                    (struct sockaddr *) &senderSockAddr, &senderSockAddrSize);
        if (senderMessageLength == -1)
        {
            int error = errorHandler("error recieving datagram", argv[0]);
            close(ssd);
            return error;
        }

        printf("%s: got data from %s: %s\n", argv[0], inet_ntoa(senderSockAddr.sin_addr), recvBuffer);
        strcpy(sendBuffer, recvBuffer);

        if (sendto(ssd, sendBuffer, senderMessageLength, 0, (struct sockaddr *) &senderSockAddr, senderSockAddrSize) == -1)
        {
            int error = errorHandler("error sending datagram", argv[0]);
            close(ssd);
            return error;
        }
    }

    close(ssd);

    return 0;
}