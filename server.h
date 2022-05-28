#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>

enum Color {BLUE=0, GREEN=1, RED=2, YELLOW=3, DOS=4};
enum Number {HASHTAG=0, ONE=1, TWO=2, THREE=3, FOUR=4, FIVE=5,
     SIX=6, SEVEN=7, EIGHT=8, NINE=9, TEN=10};

std::string ColorLUT[] = {"BLUE", "GREEN", "RED", "YELLOW", "DOS"};
std::string NumberLUT[] = {"HASHTAG", "ONE", "TWO", "THREE", "FOUR", "FIVE",
    "SIX", "SEVEN", "EIGHT", "NINE", "TEN"};


class Card {
private:
    int _color;
    int _number;
public:
    Card(int color, int number) : _color(color), _number(number) {}
    int getColor() {return _color;}
    int getNumber() {return _number;}
};

class Stack {
private:
    std::vector<Card> _cards;
public:
    Stack(int count, bool drawstack);
    Stack() {};
    std::vector<Card> getCards() {return _cards;}
    void setCards(std::vector<Card> cards) {_cards = cards;}
    Card getCardByIndex(int index) {return _cards[index];}
    void deleteCardByIndex(int index) {_cards.erase(_cards.begin() + index);}
    void addToCards(Card &card) {_cards.push_back(card);}
    void shuffle();
} drawingstack(108, 1);



class Player : public Stack {
private:
    int _socketid;
public:
    Player() {};
    int getSocketid() {return _socketid;}
    void setSocketid(int socketid) {_socketid = socketid;}
} player_1, player_2;

class Board {
private:
    std::vector<Stack> _boardstacks;
    std::vector<Player> _players;
public:
    Board();
    std::vector<Stack> getBoardStacks() {return _boardstacks;}
    Stack getBoardStackByIndex(int index) {return _boardstacks[index];}
    void addToBoardStackByIndex(int index, Card &card) {_boardstacks[index].addToCards(card);}
    std::vector<Player> getPlayers() {return _players;}
    void addToPlayerCards(int player, Card &card) {_players[player].addToCards(card);}
    void addPlayer(Player &player) {_players.push_back(player);}
    void addEmptyStack() {_boardstacks.push_back(Stack());}
} board;

std::string cards_to_string(Stack stackofcards, int type);

#endif // SERVER_H
