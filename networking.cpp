#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <vector>

#include "networking.h"
#define SERVER_PORT 8080

int valread;
char buffer[1024] = {0};    // Buffer for incoming data
const std::string delimiter_numcol = "."; 
const std::string delimiter_card = ","; 
const std::string delimiter_stack = "!"; 
const std::string delimiter_section = "@";


int init_socket() {
    /*
    Networking initalization
    */

    int new_socket = 0;
    struct sockaddr_in serv_addr;
    if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Failed to create the socket!\n");
    }
    else {
        printf("socket creation success!\n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.101.101");

    if (connect(new_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed!\n");
    }
    else {
        printf("Connection success!\n");
    }

    return new_socket;

    /*
    End of networking initalization
    */
}

void send_to_server(int sock, char *outgoing_message) {
    // Sends a string to the server
    printf("sending: %s", outgoing_message);
    send(sock, outgoing_message, strlen(outgoing_message) ,0);
    printf("Outgoing message sent\n");
}

void receive_from_server(int sock) {
    // Checks if the server has sent a string to this client
    std::string received_string;
    std::vector<std::string> sections;

    valread = read(sock, buffer, 1024);
    printf("RAW: %s", buffer);
    // Separate sections to separate strings into an array
    printf("Starting to parse.\n");
    sections = parse_from_string(buffer, delimiter_section);
    printf("Parsing done\n");


    /*for (int i=0;i<7;i++) {
        printf("%i\n", buffer[3]);
    }
    if (!(strstr(buffer, disconnect_msg) == NULL)) {
    */
}
std::vector<std::string> parse_from_string(std::string string, std::string delimiter) {
    std::string token;
    std::vector<std::string> asd;
    asd.push_back("asd");
    size_t pos = 0;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        token = string.substr(0, pos);
        printf("%s", token.c_str());
        string.erase(0, pos + delimiter.length());
    }
    printf("%s", token.c_str());
    return asd;
}
