#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "Tile.h"
#include "LinkedList.h"
#include <vector>
class Player {
public:

    Player();
    std::string getName();
    int getScore();
    LinkedList getTiles();
    void setScore(int score);
    void addTiles(Tile tile);
    void setName(std::string name);
    std::string printTiles();
private:
    std::string name;
    int score;
    int maxNumTiles;
    LinkedList  handTiles;
};

#endif // ASSIGN2_PLAYER_H
