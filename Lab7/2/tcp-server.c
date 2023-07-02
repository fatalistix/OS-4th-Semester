#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>


int errorHandler(char * message, char * programName)
{
    char error[256];
    sprintf(error, "%s: ", programName);
    strcat(error, message);
    perror(error);
    return errno;
}


void sigchildHandler(int code)
{
    int status;
    wait(&status);
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



    ssd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ssd == -1)
    {
        return errorHandler("cannot create tcp socket", argv[0]);
    }

    printf("%s: socket created\n", argv[0]);
    
    if (bind(ssd, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) == -1)
    {
        int error = errorHandler("error binding socket", argv[0]);
        close(ssd);
        return error;
    }

    printf("%s: bind completed\n", argv[0]);

    if (listen(ssd, 3) == -1)
    {
        int error = errorHandler("error switching to listen mode", argv[0]);
        close(ssd);
        return error;
    }

    printf("%s: switched to listen\n", argv[0]);


    if (signal(SIGCHLD, sigchildHandler) == SIG_ERR)
    {
        int error = errorHandler("error registering function", argv[0]);
        close(ssd);
        return error;
    }

    printf("%s: signal handler added\n", argv[0]);

    printf("%s: listening for messages...\n", argv[0]);


    char recvBuffer[256] = "";
    char sendBuffer[256] = "";
    struct sockaddr_in senderSockAddr;
    unsigned int senderSockAddrSize = sizeof(senderSockAddr);
    int senderMessageLength;
    while (1)
    {
        int connectionSD = accept(ssd, (struct sockaddr *) &senderSockAddr, &senderSockAddrSize);
        if (connectionSD == -1)
        {
            int error = errorHandler("cannot call accept", argv[0]);
            close(ssd);
            printf("%s: server shutdown\n", argv[0]);
            return error;
        }
        
        printf("%s: new connection with %s\n", argv[0], inet_ntoa(senderSockAddr.sin_addr));
        pid_t pid = fork();
        if (pid == -1)
        {
            int error = errorHandler("cannot create a new process", argv[0]);
            close(ssd);
            printf("%s: server shutdown\n", argv[0]);
            return error;
        }
        if (pid == 0)
        {
            do
            {
                senderMessageLength = recv(connectionSD, recvBuffer, 256, 0);
                if (senderMessageLength == -1)
                {
                    int error = errorHandler("error recieving a packet", argv[0]);
                    close(connectionSD);
                    printf("%s: connection with %s droped\n", argv[0], inet_ntoa(senderSockAddr.sin_addr));
                    return error;
                }
                if (senderMessageLength == 0)
                {
                    break;
                }

                printf("%s: got %d bytes data from %s: %s\n", argv[0], senderMessageLength, inet_ntoa(senderSockAddr.sin_addr), recvBuffer);
                strcpy(sendBuffer, recvBuffer);

                if (send(connectionSD, sendBuffer, senderMessageLength, 0) == -1)
                {
                    int error = errorHandler("error sending a packet", argv[0]);
                    close(connectionSD);
                    printf("%s: connection with %s droped\n", argv[0], inet_ntoa(senderSockAddr.sin_addr));
                    return error;
                }
            } while (strcmp(recvBuffer, "disconnect"));
            printf("%s: got disconnect signal from %s: closing connection...\n", argv[0], inet_ntoa(senderSockAddr.sin_addr));
            close(connectionSD);
            close(ssd);
            printf("%s: disconnected successfully\n", argv[0]);
            return 0;
        }
        else 
        {
            close(connectionSD);
        }
    }

    printf("%s: server shutdown\n", argv[0]);
    close(ssd);

    return 0;
}
