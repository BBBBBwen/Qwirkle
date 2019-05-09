#include "LinkedList.h"
#include "Player.h"
#include "TileCodes.h"

#include <iostream>
#include <fstream>
#include <regex>
#define EXIT_SUCCESS    0


class Qwirkle {
public:
    Qwirkle();
    void menu();
    void newGame();
    void loadGame();
    void showInfo();
    void paintMap();
    void command(std::string str);
    void placeTile(Tile* tile);
    void saveGame(std::string fileNmae);
    void initialBag();
    void initialMap();
    bool isValid();

private:
    Player* player;
    LinkedList bag;
    int numPlayer;
    std::vector< std::vector<Tile> > gameMap;
};

int main(void) {
    Qwirkle* qwirkle = new Qwirkle();
    std::cout << "Welcome to Qwirkle!" << std::endl;
    std::cout << "-------------------" << std::endl;
    qwirkle->menu();
    return EXIT_SUCCESS;
}

Qwirkle::Qwirkle() {
    numPlayer = 2;
    player = new Player[numPlayer];
    initialBag();
    initialMap();
    Tile tile(0, ' ');
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
    }
}

void Qwirkle::newGame() {
    int i = 0, numPlayer = 2;
    std::string name = "";
    std::cout << "Starting a New Game" << std::endl;
    while(i < numPlayer) {
        std::cout << "Enter a name for player " << i + 1 <<
                  " (uppercase characters only)" << std::endl;
        std::cout << "> ";
        std::cin >> name;
        if(!std::regex_match(name, std::regex("[A-Z]+"))) {
            std::cout << "wrong input, enter again" << std::endl;
        } else {
            player[i].setName(name);
            i++;
        }
    }
    std::cout << "Let's Play!" << std::endl << std::endl;
    paintMap();
    std::cout << "> ";
    std::string fn;
    std::cin >> fn;
    saveGame(fn);
}

void Qwirkle::loadGame() {
    std::string fileName;
    std::cout << "Enter the filename from which load a game" << std::endl;
    std::cout << "> ";
    std::cin >> fileName;
    std::ifstream inFile(fileName + ".txt");
    std::string playerName;
    int playerScore;
    std::string playerHand;
    std::vector<Tile> temp;
    int numSize = 0;
    std::string mapPerLine;
    std::string bagLine;
    inFile >> numSize;
    for(int i = 0; i < numPlayer; i++){
    inFile >> playerName;
    inFile >> playerScore;
    inFile >> playerHand;
    std::istringstream is(playerHand);
    std::string str;
    while(is >> str){
        Tile tile(str.at(0),str.at(1));
        player[i].addTiles(tile);
    }
    player[i].setName(playerName);
    player[i].setScore(playerScore);
    }
    for(int i = 0; i < numSize; i++) {
        inFile >> mapPerLine;
        std::istringstream is(mapPerLine);
        std::string str;
        while(is >> str){
            Tile tile(str.at(0),str.at(1));
            temp.push_back(tile);
        }
        gameMap.push_back(temp);
    }
    inFile >> bagLine;
        std::istringstream is(bagLine);
        std::string str;
        while(is >> str){
            Tile tile(str.at(0),str.at(1));
            bag.addNote(tile);
        }
    inFile.close();
    paintMap();
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
    std::cout << player[0].getName() << ", it's your turn" << std::endl;
    std::cout << "Score for A: " << player[0].getScore() << std::endl;
    std::cout << "Score for B: " << player[1].getScore() << std::endl << std::endl;
    std::cout << "   ";
    for(int i = 0; i < gameMap.at(0).size(); i++)
        std::cout << i << "  ";
    std::cout << std::endl << "----------------------" << std::endl;
    for(int i = 0; i < gameMap.size(); i++) {
        std::cout << (char)('A' + i) << " |";
        for(int j = 0; j < gameMap[0].size(); j++) {
            std::cout << gameMap[i][j].print() << "|";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "Your hand is: " << player[0].printTiles() <<
              std::endl;
}

void Qwirkle::command(std::string str) {
    if(str.find("place") != std::string::npos) {
        std::cout << "cool" << std::endl;
    }
}

void Qwirkle::saveGame(std::string fileName) {
    std::ofstream outFile;
    outFile.open(fileName + ".txt");
    for(int i = 0; i < numPlayer; i++) {
        outFile << player[i].getName() << " " << player[i].getScore() << std::endl;
        outFile << player[i].printTiles() << std::endl;
    }
    outFile << gameMap.size() << std::endl;
    for(int i = 0; i < gameMap.size(); i++) {
        for(int j = 0; j < gameMap[0].size(); j++) {
            outFile << gameMap[i][j].print() << " ";
        }
        outFile << std::endl;
    }
    for(int i = 0;i < bag.getSize(); i++)
    outFile << bag.get(i).print() << " ";
    outFile.close();
}
