#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

#include "server.h"
#include "card_handling.h"

#define SERVER_PORT 8080

int player_count;

enum ServerState {WAITING, PREGAME, INGAME, POSTGAME} ServerState;

Stack::Stack(int count, bool drawstack) {
    printf("TEST");
    // Create the stack from which cards are drawn from
    if (drawstack) {
        int i, j, k;
        // Create the initial drawstack

        // 12 DOS cards
        for (i=0; i<12; i++) {
            Card card_ = Card(DOS, TWO);
            this->addToCards(card_);
        }

        // Each color has 3 Cards of the numbers 1, 3, 4, 5
        for (i=0;i<3;i++) {
            for (j=0;j<4;j++) {
                for (k=0;k<5;k++) {
                    if (k==1) {k++;}
                    Card card_ = Card(j, k);
                    this->addToCards(card_);
                }
            }
        }

        // Two cards each of the numbers 6, 7, 8, 9, 10, #
        for (i=0;i<2;i++) {
            for (j=0;j<4;j++) {
                for (k=5;k<11;k++) {
                    Card card_ = Card(j, k);
                    this->addToCards(card_);
                }
            }
        }
        this->shuffle();
    }
}

void Stack::shuffle() {
    srand (time(NULL));
    Stack drawingstack_shuffled;

    while(!this->getCards().empty()) {
        size_t rindex = rand() % this->getCards().size();
        Card card_ = this->getCardByIndex(rindex);
        drawingstack_shuffled.addToCards(card_);
        this->deleteCardByIndex(rindex);
    }
    this->setCards(drawingstack_shuffled.getCards());
    printf("SHUFFLED CARDS:\n");
    for(int i = 0;i<this->getCards().size();i++) {
        printf("%i\n", this->getCards()[i].getNumber());
    }
}


Board::Board() {
    // Draw the board Cards from drawing_stack
    // (2 different cards next to each other)
    this->addEmptyStack();
    this->addEmptyStack();
    Card card_ = drawingstack.getCards().back();
    this->addToBoardStackByIndex(0, card_);
    drawingstack.getCards().pop_back();
    card_ = drawingstack.getCards().back();
    this->addToBoardStackByIndex(1, card_);
    drawingstack.getCards().pop_back();
    printf("SIZE OF BOARDSTACK ONE: %i", this->getBoardStackByIndex(0).getCards().size());
    // Deal 7 cards from the drawing_stacks to each player
    this->addPlayer(player_1);
    this->addPlayer(player_2);
    printf("PLAYERCOUNT: %i\n", this->getPlayers().size());
    printf("DRAWINGSTACK SIZE: %i\n", drawingstack.getCards().size());
    for (int i=0;i<7;i++) {
        this->addToPlayerCards(0, drawingstack.getCards().back());
        drawingstack.deleteCardByIndex(drawingstack.getCards().size()-1);
        this->addToPlayerCards(1, drawingstack.getCards().back());
        drawingstack.deleteCardByIndex(drawingstack.getCards().size()-1);
    }
    printf("PLAYER 1 (CARDCOUNT: %i) NUMBERS:\n", this->getPlayers()[0].getCards().size());
    for (int i = 0; i<7; i++) {
        printf("%i\n", this->getPlayers()[0].getCards()[i].getNumber());
    }
    printf("PLAYER 2 NUMBERS:\n");
    for (int i = 0; i<7; i++) {
        printf("%i\n", this->getPlayers()[1].getCards()[i].getNumber());
    }
}

char* cards_to_c_str(Stack stackofcards, int type) {
    /* 
    ------------
    ARGUMENTS
    ------------
    int type: 0 for color, 1 for number
    -------------

    -------------
    RETURN
    -------------
    pointer to char array (either colors or numbers of cards)
    -------------
    */
    std::string returnstr;
    // USE FUNCTION TEMPLATE FOR THESE?
    if (type == 0) {
        for (auto i : stackofcards.getCards()) {
            returnstr.append(std::to_string(i.getColor()));
            printf("%i\n", i.getColor());
        }
        char * returncstr = new char [returnstr.length()+1];
        std::strcpy (returncstr, returnstr.c_str());
        return returncstr;
    }

    for (auto i : stackofcards.getCards()) {
        returnstr.append(std::to_string(i.getNumber()));
        printf("%i\n", i.getNumber());
    }
    char * returncstr = new char [returnstr.length()+1];
    std::strcpy(returncstr, returnstr.c_str());
    return returncstr;
}
/*
void sendAfterTurn() {
    
}

char* constructBuffer() {

}
*/

int main(int argc, char *argv[]) {
    int  listenerfd, new_socket, valread, addrlen;  // Server file descriptor
    int opt = 1;
    char buffer[1024] = {0};            // Buffer for incoming data
    struct sockaddr_in address;
    addrlen = sizeof(address);
    const char *test_msg = "Cards";
    const char *disconnect_msg = "DISCONNECT";
    const char *matchmaking_msg = "MATCHMAKING";
    const char *quit_msg = "QUIT SERVER";
    if((listenerfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Server file descriptor creation failed!\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(listenerfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed!\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY = localhost
    address.sin_port = htons(SERVER_PORT);

    if(bind(listenerfd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Server bind failed!\n");
        exit(EXIT_FAILURE);
    }

    if (listen(listenerfd, 3) < 0) {
        perror("Listening failed!\n");
        exit(EXIT_FAILURE);
    }

    fd_set current_sockets, ready_sockets;

    FD_ZERO(&current_sockets);

    FD_SET(listenerfd, &current_sockets);

    while (1) {
        ready_sockets = current_sockets;

        if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
            perror("Select error!\n");
            exit(EXIT_FAILURE);
        }

        for (int i=0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &ready_sockets)) {
                if (i == listenerfd) {  // Check for new connection -> accept new connection
                    if ((new_socket = accept(listenerfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
                        perror("Socket creation between server and client failed!\n");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(new_socket, &current_sockets);
                    printf("A new socket set.\n");
                }
                else {
                    printf("reading...\n");
                    valread = read(i, buffer, 1024);   // read data from buffer
                    if (!(strstr(buffer, disconnect_msg) == NULL)) {
                        printf("FROM CLIENT: DISCONNECT\n");
                        exit(0);
                        break;
                    }
                    else if (!(strstr(buffer, matchmaking_msg) == NULL)) {
                        player_count++;
                        printf("FROM CLIENT: MATCHMAKING\n");
                        if (player_count == 2) {
                            player_2.setSocketid(i);
                            // Two players connected -> switch state
                            ServerState = PREGAME;
                        }
                        else if (player_count == 1) {
                            player_1.setSocketid(i);
                        }
                    }
                    else if (!(strstr(buffer, quit_msg) == NULL)) {
                        // QUIT
                        exit(0);
                    }
                }
            }
        }
        if (ServerState == PREGAME) {
            printf("NUM: %i", board.getBoardStackByIndex(0).getCardByIndex(0).getNumber());
            printf("IN PREGAME: %s\n", cards_to_c_str(board.getBoardStackByIndex(0), 1));

            std::string asd = "gdf@gfgfds@dsa";

            // Send board cards to players
            // STACK 1
            printf("Sending...");
            send(player_1.getSocketid(), asd.c_str(), strlen(asd.c_str()), 0);
            send(player_2.getSocketid(), asd.c_str(), strlen(asd.c_str()), 0);
            /*send(player_1.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(0), 0), 64, 0);
            send(player_1.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(0), 1), 64, 0);
            send(player_2.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(0), 0), 64, 0);
            send(player_2.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(0), 1), 64, 0);
            // STACK 2
            send(player_1.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(1), 0), 64, 0);
            send(player_1.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(1), 1), 64, 0);
            send(player_2.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(1), 0), 64, 0);
            send(player_2.getSocketid(), cards_to_c_str(board.getBoardStackByIndex(1), 1), 64, 0);

            //Send starting cards to players
            send(player_1.getSocketid(), cards_to_c_str(player_1, 0), 64, 0);
            send(player_1.getSocketid(), cards_to_c_str(player_1, 1), 64, 0);
            send(player_2.getSocketid(), cards_to_c_str(player_2, 0), 64, 0);
            send(player_2.getSocketid(), cards_to_c_str(player_2, 1), 64, 0);
            */
            printf("sent.");
            ServerState = INGAME;
        }
    }
    return 0;
}
