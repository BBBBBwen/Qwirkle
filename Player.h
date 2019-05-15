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
    void setScore(unsigned int score);
    void addTiles(Tile tile);
    void deleteTiles(Tile tile);
    void setName(std::string name);
    bool hasTile(Tile tile);
    std::string printTiles();
private:
    std::string name;
    int         score;
    int         maxNumTiles;
    LinkedList  handTiles;
};

#endif // ASSIGN2_PLAYER_H
