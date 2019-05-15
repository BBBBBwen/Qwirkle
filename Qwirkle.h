#ifndef ASSIGN2_QWIRKLE_H
#define ASSIGN2_QWIRKLE_H

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

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
    void initialMap();
    void paintMap();
    void placeTile(Tile tile, const unsigned int locationX,
                   const unsigned int locationY);
    void play();
    void extendMap(unsigned int mapSize);
    void exitCommand();
    void helpCommand();
    bool placeAtCommand();
    bool replaceCommand();
    bool isValid(Tile& tile, const int& x, const int& y);
    bool isColValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    bool isRowValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    int calculateScore(Tile& tile, const unsigned int& locationX,
                       const unsigned int& locationY);

    //helper fuctions
    bool gameOver();
    void switchTurn();
    bool isEmpty(const unsigned int& x, const unsigned int& y);
    void toUpperCase(std::string& str);
    std::vector<Tile> split(const std::string& str, const std::string& delimiter);

private:
    LinkedList bag;
    Tile emptyTile;
    int numPlayer = 2;
    int turn = 0;
    bool isFirstTile = true;
    Player* player = new Player[numPlayer];
    std::vector< std::vector<Tile> > gameMap;
};

#endif // ASSIGN2_QWIRKLE_H
