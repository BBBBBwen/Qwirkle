#include "Player.h"
#include <exception>
#include <string>
#include <sstream>

Player::Player() :
    name(""), score(0), numTiles(0), handTiles() {
}

Player::~Player() {
}

//get player's name
std::string Player::getName() {
    return name;
}

//get the player's score
int Player::getScore() {
    return score;
}

//get all tiles
LinkedList Player::getTiles() {
    return handTiles;
}

int Player::getNumTiles() {
    return numTiles;
}

//print all tiles on player's hand
std::string Player::printTiles() {
    std::string str = "";
    for(int i = 0; i < handTiles.getSize(); i++) {
        str += handTiles.get(i).print();
        if(i != handTiles.getSize() - 1)
            str += ",";
    }
    return str;
}

//print all tiles on player's hand
std::string Player::printPlainTiles() {
    std::string str = "";
    for(int i = 0; i < handTiles.getSize(); i++) {
        str += handTiles.get(i).printPlainTile();
        if(i != handTiles.getSize() - 1)
            str += ",";
    }
    return str;
}

//change the name of the player
void Player::setName(std::string name) {
    this->name = name;
}

//change the score of the player
void Player::setScore(unsigned int score) {
    this->score = score;
}

//add a tile to player's hand
void Player::addTiles(Tile tile) {
    if(handTiles.getSize() < 6) {
        this->handTiles.addNote(tile);
        numTiles++;
    } else {
        throw "too much tiles on hand";
    }
}

//superdraw for testing
void Player::superDraw(Tile tile) {
    this->handTiles.addNote(tile);
    numTiles++;
}

//delete the tile from player's hand
void Player::deleteTiles(Tile tile) {
    if(hasTile(tile)) {
        this->handTiles.deleteNode(tile);
        numTiles--;
    }
}

//check if the player has the certain tile
bool Player::hasTile(Tile tile) {
    bool check = false;
    for(int i = 0; i < handTiles.getSize(); i++) {
        if(handTiles.get(i) == tile) {
            check = true;
        }
    }
    return check;
}
