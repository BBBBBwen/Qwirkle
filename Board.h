#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

#include "LinkedList.h"
#include "Player.h"
#include "TileCodes.h"
#include "Qwirkle.h"


class Board {
public:
    Board(LinkedList bag, Player* player);
    ~Board();

    void initialMap();
    void paintMap();
    void placeTile(std::vector<Tile> tile, int turn,
                   std::vector<std::string> location);
    void extendMap(unsigned int mapSize);
    bool isValid(std::vector<Tile>& tile, std::vector<std::string> location);
    bool isColValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    bool isRowValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    int calculateScore(std::vector<Tile> tile,
                       std::vector<std::string> location);
    int calculateColScore(Tile& tile, const unsigned int& x, const unsigned int& y);
    int calculateRowScore(Tile& tile, const unsigned int& x, const unsigned int& y);
    std::vector< std::vector<Tile> > getMap();

private:
    LinkedList bag;
    Player* player;
    Tile emptyTile;
    std::vector< std::vector<Tile> > gameMap;

};

#endif // ASSIGN2_BOARD_H
