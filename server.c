/* A simple server in the internet domain via UDP */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// prints the appropriate error message
void exception(const char *msg)
{
    perror(msg);
    exit(1);
}

// server application main
int main(int argc, char *argv[])
{
    // declare neccesary structs and variables
    int sockfd;
    int newsockfd;
    int portno = 5001;
    int req_code;
    int bind_socket;
    int success;
    int required_args = 1;
    socklen_t client_len;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;

    // check if correct arguments are passed and instantiate
    if (argc != required_args) {
        fprintf(stderr, "ERROR invalid number of arguments.\n");
        fprintf(stderr, "USAGE: ./server.sh <req_code>\n");
        exit(1);
    } else {
        req_code = atoi(argv[1]);
    }

    // test to see if command line arguments passed correctly
    printf("The req_code is: %d\n", req_code);

    // continously try to open a UDP socket connection
    do {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    } while (sockfd < 0);

    // configure socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // continously try to bind socket, incrementing portno on failure
    do {
        portno += 1;
        serv_addr.sin_port = htons(portno);
        bind_socket = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    } while (bind_socket < 0);
    printf("SERVER_PORT=%d\n", portno);

    // listen and accept on socket
    listen(sockfd, 5);
    client_len = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client_len);
    if (newsockfd < 0) {
        exception("ERROR on accept.\n");
    }

    // read client message into buffer and print to stdout
    bzero(buffer,256);
    success = read(newsockfd,buffer,255);
    if (success < 0) {
        exception("ERROR reading from socket.\n");
    }
    printf("Here is the message: %s.\n", buffer);

    // write acknowledgement back to client and close connection
    success = write(newsockfd, "I got your message.\n", 18);
    if (success < 0) {
        exception("ERROR writing to socket.\n");
    }
    close(newsockfd);
    close(sockfd);
    return 0; 
}
