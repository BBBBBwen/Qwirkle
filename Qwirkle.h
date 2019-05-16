#ifndef ASSIGN2_QWIRKLE_H
#define ASSIGN2_QWIRKLE_H

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
    void initialMap();
    void paintMap();
    void placeTile(std::vector<Tile> tile, std::vector<std::string> location);
    void play();
    void extendMap(unsigned int mapSize);
    void helpCommand();
    bool placeAtCommand(std::string tiles, std::string location);
    bool replaceCommand(std::string tiles);
    bool isValid(std::vector<Tile>& tile, std::vector<std::string> location);
    bool isColValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    bool isRowValid(Tile& tile, const unsigned int& x, const unsigned int& y);
    int calculateColScore(Tile& tile, const unsigned int& x, const unsigned int& y);
    int calculateRowScore(Tile& tile, const unsigned int& x, const unsigned int& y);
    //helper fuctions
    bool gameOver();
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
    int numPlayer = 2;
    int turn = 0;
    bool isFirstTile = true;
    bool gamePlay = true;
    Player* player = new Player[numPlayer];
    std::vector< std::vector<Tile> > gameMap;
};

#endif // ASSIGN2_QWIRKLE_H
