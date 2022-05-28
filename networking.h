#ifndef NETWORKING_H
#define NETWORKING_H

int init_socket();
void send_to_server(int sock, char *outgoing_message);
void receive_from_server(int sock);
std::vector<std::string> parse_from_string(std::string string, std::string delimiter);

#endif // NETWORKING_H
