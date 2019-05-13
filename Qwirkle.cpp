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
            inFile >> playerName;
            inFile >> playerScore;
            std::getline(inFile, playerHand, '\n');
            std::istringstream is(playerHand);
            std::string str;
            while(is >> str) {
                Tile tile(str.at(0), str.at(1));
                player[i].addTiles(tile);
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
            if(secondCommand.find("AT") == std::string::npos)
                continue;
            int y = location[0] - 65;
            int x = location[1] - 48;
            if(!gameMap[y][x].isEqual(*emptyTile)) {
                std::cout << "Wrong Location, Try Again" << std::endl << "> ";
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

void Qwirkle::switchTurn() {
    if(turn == 0) {
        turn = 1;
    } else {
        turn = 0;
    }
}

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

void Qwirkle::placeTile(Tile tile, int locationX, int locationY) {
    int score = player[turn].getScore() + 1;
    int flag = 0;
    gameMap[locationY][locationX] = tile;
    for(int i = 0; i < gameMap.size(); i++) {
        if(!gameMap[locationY][i].isEqual(*emptyTile) && i != locationX) {
            score++;
            flag++;
        } else if(!gameMap[i][locationX].isEqual(*emptyTile) && i != locationY) {
            score++;
            flag++;
        }
    }
    if(flag == 2) score += 1;
    player[turn].setScore(score);
    player[turn].deleteTiles(tile);
    player[turn].addTiles(bag.draw());
}

bool Qwirkle::isValid(Tile tile, int x, int y) {
    if(checkFirstTile)
        return true;
    for(int i = -1; i < 2; i += 2) {
        if(x + i >= 0 && x + i < gameMap.size())
            if(!gameMap[y][x + i].isEqual(*emptyTile)) {
                if(gameMap[y][x + i].getShape() == tile.getShape()
                        || gameMap[y][x + i].getColour() == tile.getColour()) {
                    return true;
                }
            }
        if(y + i >= 0 && y + i < gameMap.size())
            if(!gameMap[y + i][x].isEqual(*emptyTile)) {
                if(gameMap[y + i][x].getShape() == tile.getShape()
                        || gameMap[y + i][x].getColour() == tile.getColour()) {
                    return true;
                }
            }
    }
    return false;
}
