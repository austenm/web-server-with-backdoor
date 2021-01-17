#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {

    //pack this in a variable to send to client instead of printf
    if (argc < 2) {
        printf("Please enter a port number.\n");
        exit(0);
    }
    else if (argc == 2) {
        printf("The port specified is %i\n", atoi(argv[1]));
    }

    int port = atoi(argv[1]);

    //Create socket, define address, bind to address, listen to port
    int mah_socket;
    mah_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(mah_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(mah_socket, 0);    
    
    //GET requests of valid URL format get 404
    //GET requests with URL of form /exec/<command> execute it using libc function
    //HTTP response is stdout of executed command, status code 200
    //No limit on characters, be able to handle anything

    static char* not_found_response_template =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-type: text/html\r\n"
        "\r\n"
        "<html>\r\n"
        " <body>\r\n"
        "  <h1>Not Found</h1>\r\n"
        "  <p>The requested URL was not found on this server.</p>\r\n"
        " </body>\r\n"
        "</html>\r\n";

    //len = strlen(not_found_response_template);
    //send(newSct, not_found_response_template, len, 0);

    //Flush socket and close at end!!
}