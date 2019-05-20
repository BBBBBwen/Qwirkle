#ifndef ASSIGN2_QWIRKLE_H
#define ASSIGN2_QWIRKLE_H

#include "LinkedList.h"
#include "Player.h"
#include "TileCodes.h"
#include "Board.h"

#include <iostream>
#include <fstream>
#include <regex>


class Qwirkle {

public:
    Qwirkle();
    ~Qwirkle();
    //main fuctions
    void menu();
    void newGame();
    void saveGame(const std::string fileNmae);
    void loadGame();
    void showInfo();
    void command();

    //secondery functions
    void initialBag();
    void play();
    void helpCommand();
    bool placeAtCommand(std::string tiles, std::string location);
    bool replaceCommand(std::string tiles);
    //helper fuctions
    void gameOver();
    void switchTurn();
    bool isEmpty(const unsigned int& x, const unsigned int& y);
    void toUpperCase(std::string& str);
    std::vector<Tile> splitToTile(const std::string& str,
                                  const std::string& delimiter);
    std::vector<std::string> splitToString(const std::string& str,
                                           const std::string& delimiter);

private:
    LinkedList bag;
    Tile emptyTile;
    int numPlayer = 0;
    int turn = 0;
    bool isFirstTile = true;
    bool gamePlay = true;
    Player* player;
    Board* board;

};

#endif // ASSIGN2_QWIRKLE_H
