#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "Tile.h"
#include "LinkedList.h"
#include <vector>
class Player {
public:

    Player();
    ~Player();
    std::string getName();
    int getScore();
    int getNumTiles();
    LinkedList getTiles();
    void setScore(unsigned int score);
    void addTiles(Tile tile);
    void superDraw(Tile tile);
    void deleteTiles(Tile tile);
    void setName(std::string name);
    bool hasTile(Tile tile);
    std::string printTiles();
    std::string printPlainTiles();

private:
    std::string name;
    int         score;
    int         numTiles;
    LinkedList  handTiles;
};

#endif // ASSIGN2_PLAYER_H
