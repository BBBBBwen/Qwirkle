#include "Player.h"
#include <exception>

Player::Player() :
    name(""), score(0), maxNumTiles(6) {
}

std::string Player::getName() {
    return name;
}

int Player::getScore() {
    return score;
}

LinkedList Player::getTiles() {
    return handTiles;
}

std::string Player::printTiles() {
    std::string str = "";
    for(int i = 0; i < handTiles.getSize(); i++) {
        str += handTiles.get(i).print();
        if(i != handTiles.getSize() - 1)
            str += ",";
    }
    return str;
}

void Player::setName(std::string name) {
    this->name = name;
}

void Player::setScore(int score) {
    this->score = score;
}

void Player::addTiles(Tile tile) {
    if(handTiles.getSize() < 6)
        this->handTiles.addNote(tile);
    else
        throw "too much tiles on hand";
}
