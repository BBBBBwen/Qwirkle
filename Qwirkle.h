#ifndef ASSIGN2_QWIRKLE_H
#define ASSIGN2_QWIRKLE_H

#include <iostream>
#include <fstream>
#include <regex>

#include "LinkedList.h"
#include "Player.h"
#include "TileCodes.h"
#include "ColourHelp.h"

#define  GAME            false
#define  TEST            true
#define  EXIT_SUCCESS    0

class Qwirkle {
public:
    Qwirkle();
    ~Qwirkle();
    //main functions
    void menu();
    void newGame();
    void saveGame(const std::string fileNmae);
    void loadGame();
    void showInfo();
    void command();
    void testing();

    //secondary functions
    void initialBag();
    void initialMap();
    void play();
    void paintMap(bool isTest);
    void placeTile(std::vector<Tile> tile, std::vector<std::string> location);
    void extendMap(unsigned int mapSize);
    void helpCommand();
    bool placeAtCommand(std::string tiles, std::string location, bool isTest);
    bool replaceCommand(std::string tiles);
    bool isValid(std::vector<Tile>& tile, std::vector<std::string> location,
                 bool isTest);
    bool isColValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    bool isRowValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    int calculateScore(std::vector<Tile> tile, std::vector<std::string> location);
    int calculateColScore(Tile& tile, const unsigned int& x, const unsigned int& y);
    int calculateRowScore(Tile& tile, const unsigned int& x, const unsigned int& y);
    //helper functions
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
    std::vector< std::vector<Tile> > gameMap;
};

#endif // ASSIGN2_QWIRKLE_H
