#include <stdio.h>
#include <stdlib.h>

int main() {

    printf("\nHello, world!\n\n");

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

};