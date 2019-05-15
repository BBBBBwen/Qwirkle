#include "LinkedList.h"
#include "Player.h"
#include "TileCodes.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#define EXIT_SUCCESS    0


class Qwirkle {
public:
    Qwirkle();
    void menu();
    void newGame();
    void loadGame();
    void showInfo();
    void paintMap();
    void command();
    void placeTile(Tile tile, int locationX, int locationY);
    void saveGame(std::string fileNmae);
    void initialBag();
    void initialMap();
    void play();
    void switchTurn();
    bool isValid(Tile tile, int x, int y);

private:
    Player* player;
    LinkedList bag;
    Tile* emptyTile;
    int numPlayer;
    int turn;
    bool checkFirstTile = true;
    std::vector< std::vector<Tile> > gameMap;
};

int main(void) {
    Qwirkle* qwirkle = new Qwirkle();
    std::cout << "Welcome To Qwirkle!" << std::endl;
    std::cout << "-------------------" << std::endl;
    qwirkle->menu();
    return EXIT_SUCCESS;
}

Qwirkle::Qwirkle() {
    numPlayer = 2;
    player = new Player[numPlayer];
    initialBag();
    initialMap();
    turn = 0;
    emptyTile = new Tile(0, ' ');
    Tile tile = *emptyTile;
    for(int i = 0; i < 6; i++) {
        tile = bag.draw();
        player[0].addTiles(tile);
        tile = bag.draw();
        player[1].addTiles(tile);
    }
}

//initialze the bag, put every tile into the bag
void Qwirkle::initialBag() {
    Shape shape[6] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};
    Colour colour[6] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 6; j++) {
            for(int n = 0; n < 6; n++) {
                Tile tile(shape[j], colour[n]);
                bag.addNote(tile);
            }
        }
    }
}

//initialze the map
void Qwirkle::initialMap() {
    std::vector<Tile> temp;
    Tile tile(0, ' ');
    for(int i = 0; i < 6; i++) {
        temp.clear();
        for(int j = 0; j < 6; j++) {
            temp.push_back(tile);
        }
        gameMap.push_back(temp);
    }
}

//display the menu and processing the choice
void Qwirkle::menu() {
    int choose;
    std::cout << "-----Menu-----" << std::endl;
    std::cout << "1.New Game" << std::endl;
    std::cout << "2.Load Game" << std::endl;
    std::cout << "3.Show Info" << std::endl;
    std::cout << "4.Quit" << std::endl << std::endl;
    std::cout << "> ";
    std::cin >> choose;
    if(choose == 1)
        newGame();
    else if(choose == 2)
        loadGame();
    else if(choose == 3) {
        showInfo();
        menu();
    } else if(choose == 4) {
        std::cout << "Good Bye!" << std::endl;
        std::exit(0);
    } else {
        std::cout << "Wrong Input, Try Again" << std::endl;
        menu();
    }
}

//start a new game
void Qwirkle::newGame() {
    int i = 0, numPlayer = 2;
    std::string name = "";
    std::cout << "Starting A New Game" << std::endl;
    while(i < numPlayer) {
        std::cout << "Enter A Name For Player " << i + 1 <<
                  " (Uppercase Characters Only)" << std::endl;
        std::cout << "> ";
        std::cin >> name;
        if(!std::regex_match(name, std::regex("[A-Z]+"))) {
            std::cout << "Wrong Input, Enter Again" << std::endl;
        } else {
            player[i].setName(name);
            i++;
        }
    }
    std::cout << "Let's Play!" << std::endl << std::endl;
    while(true)
        play();
}

//save game data to the file
void Qwirkle::saveGame(std::string fileName) {
    std::ofstream outFile;
    outFile.open(fileName + ".txt");
    for(int i = 0; i < numPlayer; i++) {
        outFile << player[i].getName() << std::endl;
        outFile << player[i].getScore() << std::endl;
        outFile << player[i].printTiles() << std::endl;
    }
    outFile << gameMap.size() << std::endl;
    for(int i = 0; i < gameMap.size(); i++) {
        for(int j = 0; j < gameMap[0].size(); j++) {
            outFile << gameMap[i][j].print() << " ";
        }
        outFile << std::endl;
    }
    for(int i = 0; i < bag.getSize(); i++)
        outFile << bag.get(i).print() << " ";
    outFile << std::endl << turn;
    outFile.close();
}

//load game data from the file
void Qwirkle::loadGame() {
    std::string fileName;
    std::cout << "Enter The Filename From Which Load A Game" << std::endl;
    std::cout << "> ";
    std::cin >> fileName;
    std::ifstream inFile(fileName + ".txt");
    if(inFile.is_open()) {
        std::string playerName;
        int playerScore;
        std::string playerHand;
        std::vector<Tile> temp;
        int numSize = 0;
        std::string mapPerLine;
        std::string bagLine;
        for(int i = 0; i < numPlayer; i++) {
            std::getline(inFile, playerName, '\n');
            std::string tempScore = "";
            std::getline(inFile, tempScore, '\n');
            playerScore = std::stoi(tempScore);
            std::getline(inFile, playerHand, '\n');
            std::istringstream is(playerHand);
            std::string str;
            while(is >> str) {
                Tile* tile = new Tile((int)str.at(0) - 48, str.at(1));
                std::cout << tile->print() << std::endl;
                player[i].addTiles(*tile);
            }
            player[i].setName(playerName);
            player[i].setScore(playerScore);
        }
        inFile >> numSize;
        for(int i = 0; i < numSize; i++) {
            inFile >> mapPerLine;
            std::istringstream is(mapPerLine);
            std::string str;
            while(is >> str) {
                Tile tile(str.at(0), str.at(1));
                temp.push_back(tile);
            }
            gameMap.push_back(temp);
        }
        inFile >> bagLine;
        std::istringstream is(bagLine);
        std::string str;
        while(is >> str) {
            Tile tile(str.at(0), str.at(1));
            bag.addNote(tile);
        }
        inFile >> turn;
        inFile.close();
        while(true)
            play();
    } else {
        std::cout << "There Is No File Called: " << fileName << std::endl;
        std::cout << "The Game Is Shuting Down " << std::endl;
    }
}

//display infomation of the group
void Qwirkle::showInfo() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Name: <Bowen Zhang>" << std::endl;
    std::cout << "Student ID: <s3617571>" << std::endl;
    std::cout << "Email: <s3617571@student.rmit.edu.au>" << std::endl << std::endl;
    std::cout << "Name: <>" << std::endl;
    std::cout << "Student ID: <>" << std::endl;
    std::cout << "Email: <>" << std::endl << std::endl;
    std::cout << "Name: <>" << std::endl;
    std::cout << "Student ID: <>" << std::endl;
    std::cout << "Email: <>" << std::endl << std::endl;
    std::cout << "Name: <>" << std::endl;
    std::cout << "Student ID: <>" << std::endl;
    std::cout << "Email: <>" << std::endl << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

//paint map with the frame
void Qwirkle::paintMap() {
    std::cout << "   ";
    for(int i = 0; i < gameMap[0].size(); i++)
        std::cout << i << "  ";
    std::cout << std::endl << "----------------------" << std::endl;
    for(int i = 0; i < gameMap.size(); i++) {
        std::cout << (char)('A' + i) << " |";
        for(int j = 0; j < gameMap[0].size(); j++) {
            std::cout << gameMap[i][j].print() << "|";
        }
        std::cout << std::endl;
    }
}

//processing the command
void Qwirkle::command() {
    bool check = true;
    while(check) {
        std::cout << "> ";
        std::string firstCommand;
        std::string secondCommand;
        std::string tileOnHold;
        std::string location;
        std::cin >> firstCommand;
        std::transform(firstCommand.begin(), firstCommand.end(), firstCommand.begin(),
                       ::toupper);
        if(firstCommand.find("SAVE") != std::string::npos) {
            std::cout << "Enter File Name You Want To Save" << std::endl;
            std::cin >> secondCommand;
            saveGame(secondCommand);
        } else if(firstCommand.find("PLACE") != std::string::npos) {
            std::cin >> tileOnHold >> secondCommand >> location;
            std::transform(secondCommand.begin(), secondCommand.end(),
                           secondCommand.begin(), ::toupper);
            std::transform(tileOnHold.begin(), tileOnHold.end(),
                           tileOnHold.begin(), ::toupper);
            std::transform(location.begin(), location.end(),
                           location.begin(), ::toupper);
            if(secondCommand.find("AT") == std::string::npos)
                continue;
            int y = location[0] - 65;
            int x = location[1] - 48;
            if(y >= gameMap.size() || x >= gameMap.size()) {
                int mapSize = 0;
                if(x >= y)
                    mapSize = x - gameMap.size() + 1;
                else
                    mapSize = y - gameMap.size() + 1;
                std::vector<Tile> temp;
                for(int i = 0; i < mapSize; i++) {
                    for(int j = 0; j < gameMap.size(); j++) {
                        gameMap[j].push_back(*emptyTile);
                        temp.push_back(*emptyTile);
                    }
                    temp.push_back(*emptyTile);
                }
                gameMap.push_back(temp);
            }
            if(!gameMap[y][x].isEqual(*emptyTile)) {
                std::cout << "Wrong Location, Try Again" << std::endl << "> ";
                std::cout << gameMap.size() << std::endl;
                std::cout << gameMap[0].size() << std::endl;
                continue;
            }
            Shape shape = tileOnHold[1] - 48;
            Tile newTile(shape, tileOnHold[0]);
            if(!player[turn].hasTile(newTile)) {
                std::cout << "You Dont Have That Tile, Enter other tile" << std::endl << "> ";
                continue;
            }
            if(!isValid(newTile, x, y)) {
                std::cout << "You Cant Put The Tile There" << std::endl << "> ";
                continue;
            }
            placeTile(newTile, x, y);
            checkFirstTile = false;
            switchTurn();
            check = false;
        } else if(firstCommand.find("QUIT") != std::string::npos
                  || firstCommand.find("EXIT") != std::string::npos) {
            std::cout << "Good Bye!" << std::endl;
            std::exit(0);
        } else if(firstCommand.find("HELP") != std::string::npos) {
            std::cout << "Save  -   Save Game" << std::endl;
            std::cout <<
                      "Place (Tile) At (Location)  -   Choose A Tile To Place At Certain Location" <<
                      std::endl;
            std::cout << "Quit/Exit  -   Exit game" << std::endl;
            std::cout << "Help  -   Get Help" << std::endl;
        } else {
            std::cout << "Unrecognized Command" << std::endl;
        }
    }
}

//switch the player
void Qwirkle::switchTurn() {
    if(turn == 0) {
        turn = 1;
    } else {
        turn = 0;
    }
}

//the board that game mainly run
void Qwirkle::play() {
    std::cout << player[turn].getName() << ", it's your turn" << std::endl;
    std::cout << "Score For A: " << player[0].getScore() << std::endl;
    std::cout << "Score For B: " << player[1].getScore() << std::endl << std::endl;
    paintMap();
    std::cout << std::endl << "Your Hand Is: " << player[turn].printTiles() <<
              std::endl;
    std::cout << std::endl;
    command();
}

//place the tile at the certain location and add the score of that tile as well
void Qwirkle::placeTile(Tile tile, int locationX, int locationY) {
    int score = player[turn].getScore() + 1;
    int rowScore = 0;
    int colScore = 0;
    bool checkRowPositive = true;
    bool checkRowNegative = true;
    bool checkColPositive = true;
    bool checkColNegative = true;
    int flag = 0;
    gameMap[locationY][locationX] = tile;
    for(int i = 1; i < 6; i++) {
        if(locationX + i < gameMap.size())
            if(!gameMap[locationY][locationX + i].isEqual(*emptyTile) && checkColPositive) {
                rowScore++;
                flag++;
            } else {
                checkColPositive = false;
            }
        if(locationX - i >= 0)
            if(!gameMap[locationY][locationX - i].isEqual(*emptyTile) && checkColNegative) {
                rowScore++;
                flag++;
            } else {
                checkColNegative = false;
            }
        if(locationY + i < gameMap.size())
            if(!gameMap[locationY + i][locationX].isEqual(*emptyTile) && checkRowPositive) {
                colScore++;
                flag++;
            } else {
                checkRowPositive = false;
            }
        if(locationY - i >= 0)
            if(!gameMap[locationY - i][locationX].isEqual(*emptyTile) && checkRowNegative) {
                colScore++;
                flag++;
            } else {
                checkRowNegative = false;
            }
    }
    if(colScore == 5) {
        std::cout << "Qwirkle!" << std::endl;
        colScore += 6;
    }
    if(rowScore == 5) {
        std::cout << "Qwirkle!" << std::endl;
        rowScore += 6;
    }
    if(flag > 1)
        score += 1;
    score += colScore + rowScore;
    player[turn].setScore(score);
    player[turn].deleteTiles(tile);
    player[turn].addTiles(bag.draw());
}

//check if the location that the tile needs to place is valid or not
bool Qwirkle::isValid(Tile tile, int x, int y) {
    if(checkFirstTile)
        return true;
    int checkRow = 0;
    int checkCol = 0;
    bool checkRowPositive = true;
    bool checkRowNegative = true;
    bool checkColPositive = true;
    bool checkColNegative = true;
    for(int i = 1; i < 6; i++) {
        if(x + i < gameMap.size())
            if(!gameMap[y][x + i].isEqual(*emptyTile) && checkColPositive) {
                if(gameMap[y][x + i].isEqual(tile))
                    return false;
                checkRow++;
            } else {
                checkColPositive = false;
            }
        if(x - i >= 0)
            if(!gameMap[y][x - i].isEqual(*emptyTile) && checkColNegative) {
                if(gameMap[y][x + i].isEqual(tile))
                    return false;
                checkRow++;
            } else {
                checkColNegative = false;
            }
        if(y + i < gameMap.size())
            if(!gameMap[y + i][x].isEqual(*emptyTile) && checkRowPositive) {
                if(gameMap[y][x + i].isEqual(tile))
                    return false;
                checkCol++;
            } else {
                checkRowPositive = false;
            }
        if(y - i >= 0)
            if(!gameMap[y - i][x].isEqual(*emptyTile) && checkRowNegative) {
                if(gameMap[y][x + i].isEqual(tile))
                    return false;
                checkCol++;
            } else {
                checkRowNegative = false;
            }
    }
    if(checkRow >= 6 || checkCol >= 6)
        return false;
    bool emptyAll = true;
    for(int i = -1; i < 2; i += 2) {
        if(x + i >= 0 && x + i < gameMap.size())
            if(!gameMap[y][x + i].isEqual(*emptyTile)) {
                if(gameMap[y][x + i].getShape() != tile.getShape()
                        && gameMap[y][x + i].getColour() != tile.getColour()) {
                    return false;
                }
                if(gameMap[y][x + i].isEqual(tile)) {
                    return false;
                }
                emptyAll = false;
            }
        if(y + i >= 0 && y + i < gameMap.size())
            if(!gameMap[y + i][x].isEqual(*emptyTile)) {
                if(gameMap[y + i][x].getShape() != tile.getShape()
                        && gameMap[y + i][x].getColour() != tile.getColour()) {
                    return false;
                }
                if(gameMap[y + i][x].isEqual(tile)) {
                    return false;
                }
                emptyAll = false;
            }
    }
    if(emptyAll)
        return false;
    return true;
}
