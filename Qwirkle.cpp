#include "LinkedList.h"
#include "Player.h"
#include "TileCodes.h"
#include "Qwirkle.h"

#define EXIT_SUCCESS    0


int main(void) {
    Qwirkle* qwirkle = new Qwirkle();
    std::cout << "Welcome To Qwirkle!" << std::endl;
    std::cout << "-------------------" << std::endl;
    qwirkle->menu();
    delete qwirkle;
    return EXIT_SUCCESS;
}

Qwirkle::Qwirkle() :
    emptyTile(9, ' ') {
    initialBag();
    initialMap();
}

Qwirkle::~Qwirkle() {
    delete[] player;
    for(unsigned int i = 0; i < gameMap.size(); i++) {
        gameMap[i].clear();
    }
    gameMap.clear();
}

//initialise the bag, put every tile into the bag
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

//initialise the map
void Qwirkle::initialMap() {
    std::vector<Tile> temp;
    for(int i = 0; i < 6; i++) {
        temp.clear();
        for(int j = 0; j < 6; j++) {
            temp.push_back(emptyTile);
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
        exitCommand();
    } else {
        std::cout << "Unrecognize Input, Try Again" << std::endl;
        menu();
    }
}

//start a new game
void Qwirkle::newGame() {
    std::string name = "";
    std::cout << "Starting A New Game" << std::endl;
    for(int i = 0; i < numPlayer; i++) {
        std::cout << "Enter A Name For Player " << i + 1 <<
                  " (Uppercase Characters Only)" << std::endl;
        std::cout << "> ";
        std::cin >> name;
        if(!std::regex_match(name, std::regex("[A-Z]+"))) {
            std::cout << "Wrong Input, Enter Again" << std::endl;
        } else {
            player[i].setName(name);
        }
    }
    std::cout << "Let's Play!" << std::endl << std::endl;
    //initialize players hand tiles
    Tile tile = emptyTile;
    for(int i = 0; i < 6; i++) {
        tile = bag.draw();
        player[0].addTiles(tile);
        tile = bag.draw();
        player[1].addTiles(tile);
    }
    //start playing
    while(!gameOver()) {
        play();
    }
}

//save game data to the file
void Qwirkle::saveGame(std::string fileName) {
    std::ofstream outFile;
    outFile.open(fileName + ".sav");
    //store name, score and tiles on hand first
    for(int i = 0; i < numPlayer; i++) {
        outFile << player[i].getName() << std::endl;
        outFile << player[i].getScore() << std::endl;
        outFile << player[i].printTiles() << std::endl;
    }
    //store map
    outFile << gameMap.size() << std::endl;
    for(unsigned int i = 0; i < gameMap.size(); i++) {
        for(unsigned int j = 0; j < gameMap[0].size(); j++) {
            outFile << gameMap[i][j].print();
            if(j < gameMap[0].size() - 1) {
                outFile << ",";
            }
        }
        outFile << std::endl;
    }
    //store what is in the bag
    for(int i = 0; i < bag.getSize(); i++) {
        outFile << bag.get(i).print();
        if(i < bag.getSize() - 1) {
            outFile << ",";
        }
    }
    outFile << std::endl << turn;
    outFile.close();
}

//load game data from the file
void Qwirkle::loadGame() {
    std::string tempBuffer;
    std::cout << "Enter The Filename From Which Load A Game" << std::endl;
    std::cout << "> ";
    std::cin >> tempBuffer;
    std::ifstream inFile(tempBuffer + ".sav");
    if(inFile.is_open()) {
        std::vector<Tile> temp;
        int numSize = 0;
        while(!inFile.eof()) {
            //retrieve player data
            for(int i = 0; i < numPlayer; i++) {
                std::getline(inFile, tempBuffer, '\n');
                player[i].setName(tempBuffer);
                std::getline(inFile, tempBuffer, '\n');
                player[i].setScore(std::stoi(tempBuffer));
                std::getline(inFile, tempBuffer, '\n');
                temp = split(tempBuffer, ",");
                for(unsigned int j = 0; j < temp.size(); j++) {
                    std::cout << temp[j].print() << std::endl;
                    player[i].addTiles(temp[j]);
                }
            }
            //retrieve map
            std::getline(inFile, tempBuffer, '\n');
            numSize = std::stoi(tempBuffer);
            gameMap.clear();
            for(int i = 0; i < numSize; i++) {
                std::getline(inFile, tempBuffer, '\n');
                temp = split(tempBuffer, ",");
                gameMap.push_back(temp);
            }
            //retrieve bag
            std::getline(inFile, tempBuffer, '\n');
            temp = split(tempBuffer, ",");
            for(unsigned int i = 0; i < temp.size(); i++) {
                bag.addNote(temp[i]);
            }
            std::getline(inFile, tempBuffer, '\n');
            turn = std::stoi(tempBuffer);
        }
        //start playing
        while(!gameOver()) {
            play();
        }
    } else {
        std::cout << "There Is No File Called: " << tempBuffer << std::endl;
        std::cout << "The Game Is Shutting Down " << std::endl;
        menu();
    }
}

//split the string and store in a Tile vector
std::vector<Tile> Qwirkle::split(const std::string& str,
                                 const std::string& delimiter) {
    std::vector<Tile> tempVec;
    Shape shape = 0;
    Colour colour = ' ';
    std::string strs = str + delimiter;
    size_t pos = strs.find(delimiter);
    // move from one delimiter to the next
    while(pos != std::string::npos) {
        std::string tempStr = strs.substr(0, pos);
        if(tempStr.compare("  ") != 0) {
            shape = tempStr[1] - 48;
            colour = tempStr[0];
        } else {
            shape = 0;
            colour = ' ';
        }
        Tile newTile(shape, colour);
        tempVec.push_back(newTile);
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(delimiter);
    }
    return tempVec;
}

//display information of the group
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
    for(unsigned int i = 0; i < gameMap[0].size(); i++) {
        std::cout << i << "  ";
    }
    std::cout << std::endl;
    std::cout << "---";
    for(unsigned int i = 0; i < gameMap[0].size(); i++) {
        std::cout << "---";
    }
    std::cout << std::endl;
    for(unsigned int i = 0; i < gameMap.size(); i++) {
        std::cout << (char)('A' + i) << " |";
        for(unsigned int j = 0; j < gameMap[0].size(); j++) {
            std::cout << gameMap[i][j].print() << "|";
        }
        std::cout << std::endl;
    }
}

//processing the command
void Qwirkle::command() {
    bool check = false;
    while(!check) {
        std::string firstCommand;
        std::cout << "> ";
        std::cin >> firstCommand;
        toUpperCase(firstCommand);
        if(firstCommand.compare("SAVE") == 0) {
            std::cout << "Enter File Name You Want To Save" << std::endl;
            std::cin >> firstCommand;
            saveGame(firstCommand);
        } else if(firstCommand.compare("PLACE") == 0) {
            check = placeAtCommand();
        } else if(firstCommand.compare("QUIT") == 0
                  || firstCommand.compare("EXIT") == 0) {
            exitCommand();
        } else if(firstCommand.compare("HELP") == 0) {
            helpCommand();
        } else if(firstCommand.compare("REPLACE") == 0) {
            check = replaceCommand();
        } else {
            std::cout << "Unrecognised Command" << std::endl;
        }
    }
}

bool Qwirkle::replaceCommand() {
    bool check = false;
    std::string tileOnHold;
    std::cin >> tileOnHold;
    toUpperCase(tileOnHold);
    Shape shape = tileOnHold[1] - 48;
    Colour colour = tileOnHold[0];
    Tile newTile(shape, colour);
    std::string colours = "ROYGBP";
    if(colours.find(colour) == std::string::npos || shape < 0 || shape > 6) {
        std::cout << "Tile Doesnt Exists" << std::endl;
    } else if(!player[turn].hasTile(newTile)) {
        std::cout << "You Dont Have That Tile, Enter other tile" << std::endl;
    } else {
        Tile replaceTile = bag.replaceTile(newTile);
        player[turn].deleteTiles(newTile);
        player[turn].addTiles(replaceTile);
        check = true;
    }
    return check;
}
//processing exit/quit command
void Qwirkle::exitCommand() {
    std::cout << "Good Bye!" << std::endl;
    std::exit(0);
}

//processing help command
void Qwirkle::helpCommand() {
    std::cout << "Save   -   Save Game" << std::endl;
    std::cout << "Place (Tile) At (Location)   -   " <<
              "Choose A Tile To Place At Certain Location" << std::endl;
    std::cout << "Quit/Exit   -   Exit game" << std::endl;
    std::cout << "Help   -   Get Help" << std::endl;
}

//processing place at command
bool Qwirkle::placeAtCommand() {
    bool check = false;
    std::string secondCommand;
    std::string tileOnHold;
    std::string location;
    std::cin >> tileOnHold >> secondCommand >> location;
    toUpperCase(secondCommand);
    toUpperCase(tileOnHold);
    toUpperCase(location);
    //take following three string check if it is right
    if(secondCommand.compare("AT") == 0) {
        char* end = nullptr;
        unsigned int y = location.substr(0, 1)[0] - 65;
        unsigned int x = strtol(location.substr(1).c_str(), &end, 10);
        Shape shape = tileOnHold[1] - 48;
        Colour colour = tileOnHold[0];
        Tile newTile(shape, colour);
        //if location is beyond the limit, extend it
        if(y >= gameMap.size() || x >= gameMap.size()) {
            int mapSize = std::max(x, y) - gameMap.size() + 1;
            extendMap(mapSize);
        }
        //see if command is valid
        std::string colours = "ROYGBP";
        if(colours.find(colour) == std::string::npos || shape < 0 || shape > 6) {
            std::cout << "Tile Doesnt Exists" << std::endl;
        } else if(x < 0 || y < 0 || !gameMap[y][x].isEqual(emptyTile)) {
            std::cout << "Wrong Location, Try Again" << std::endl;
        } else if(!player[turn].hasTile(newTile)) {
            std::cout << "You Dont Have That Tile, Enter other tile" << std::endl;
        } else if(!isValid(newTile, x, y)) {
            std::cout << "You Cant Put The Tile There" << std::endl;
        } else {
            placeTile(newTile, x, y);
            isFirstTile = false;
            switchTurn();
            check = true;
        }
    }
    return check;
}
// transform tp upper case
void Qwirkle::toUpperCase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

//extend the size of the map
void Qwirkle::extendMap(unsigned int mapSize) {
    std::vector<Tile> temp;
    for(unsigned int i = 0; i < mapSize; i++) {
        temp.clear();
        for(unsigned int j = 0; j < gameMap.size(); j++) {
            gameMap[j].push_back(emptyTile);
            temp.push_back(emptyTile);
        }
        temp.push_back(emptyTile);
        gameMap.push_back(temp);
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
void Qwirkle::placeTile(Tile tile, unsigned int locationX,
                        unsigned int locationY) {
    gameMap[locationY][locationX] = tile;
    player[turn].setScore(calculateScore(tile, locationX, locationY));
    player[turn].deleteTiles(tile);
    if(bag.getSize() != 0)
        player[turn].addTiles(bag.draw());
}

//calculate the score the tile is going to get
int Qwirkle::calculateScore(Tile& tile, const unsigned int& locationX,
                            const unsigned int& locationY) {
    int score = player[turn].getScore() + 1, rowScore = 0, colScore = 0;
    bool checkUp = true, checkDown = true, checkRight = true, checkLeft = true;
    for(unsigned int i = 1; i < 6; i++) {
        //add up the score of the column
        if(locationX + i < gameMap.size() && checkRight) {
            if(!isEmpty(locationX + i, locationY)) {
                rowScore++;
            } else {
                checkRight = false;
            }
        }
        if(locationX >= i && checkLeft) {
            if(!isEmpty(locationX - i, locationY)) {
                rowScore++;
            } else {
                checkLeft = false;
            }
        }
        //add up the score of the row
        if(locationY + i < gameMap.size() && checkUp) {
            if(!isEmpty(locationX, locationY + i)) {
                colScore++;
            } else {
                checkUp = false;
            }
        }
        if(locationY >= i && checkDown) {
            if(!isEmpty(locationX, locationY - i)) {
                colScore++;
            } else {
                checkDown = false;
            }
        }
    }
    //calculate final score
    if(colScore == 5 && rowScore == 5) {
        std::cout << "Double Qwirkle!" << std::endl;
        score += 12;
    } else if(rowScore == 5 || colScore == 5) {
        std::cout << "Qwirkle!" << std::endl;
        score += 6;
    }
    if(rowScore > 1 && colScore > 1) {
        score += 1;
    }
    score += colScore + rowScore;
    return score;
}

//check if the location that the tile needs to place is valid or not
bool Qwirkle::isValid(Tile& tile, const int& x,
                      const int& y) {
    bool returnCheck = false;
    if(isEmpty(x + 1, y) && isEmpty(std::abs(x - 1), y) &&
            isEmpty(x, y + 1) && isEmpty(x, std::abs(y - 1))) {
        returnCheck = isFirstTile;
    } else if(isColValid(tile, x, y) && isRowValid(tile, x, y)) {
        returnCheck = true;
    }
    return returnCheck;
}

//check if the Col that the tile placed is valid
bool Qwirkle::isColValid(Tile& tile, const unsigned int& x,
                         const unsigned int& y) {
    bool returnCheck = false;
    bool sameColour = true, sameShape = true;
    bool checkEqual = true;
    bool checkUp = true, checkDown = true;
    int numCol = 0;
    for(unsigned int i = 1; i < 6; i++) {
        //check tiles below is valid
        if(y + i < gameMap.size() && checkDown) {
            if(isEmpty(x, y + i)) {
                checkDown = false;
            } else if(gameMap[y + i][x].isEqual(tile)) {
                checkEqual = false;
                checkDown = false;
            } else if(gameMap[y + i][x].getColour() != tile.getColour()) {
                sameColour = false;
                numCol++;
            } else if(gameMap[y + i][x].getShape() != tile.getShape()) {
                sameShape = false;
                numCol++;
            }
        }
        //check tiles above is valid
        if(y >= i && checkUp) {
            if(isEmpty(x, y - i)) {
                checkUp = false;
            } else if(gameMap[y - i][x].isEqual(tile)) {
                checkEqual = false;
                checkUp = false;
            } else if(gameMap[y - i][x].getColour() != tile.getColour()) {
                sameColour = false;
                numCol++;
            } else if(gameMap[y - i][x].getShape() != tile.getShape()) {
                sameShape = false;
                numCol++;
            }
        }
    }
    if((sameColour || sameShape) && checkEqual && numCol <= 5) {
        returnCheck = true;
    }
    return returnCheck;
}

//check if the row that the tile placed is valid
bool Qwirkle::isRowValid(Tile& tile, const unsigned int& x,
                         const unsigned int& y) {
    bool returnCheck = false;
    bool sameColour = true, sameShape = true;
    bool checkEqual = true;
    bool checkLeft = true, checkRight = true;
    int numRow = 0;
    for(unsigned int i = 1; i < 6; i++) {
        //check tiles on the right is valid
        if(x + i < gameMap.size() && checkRight) {
            if(isEmpty(x + i, y)) {
                checkRight = false;
            } else if(gameMap[y][x + i].isEqual(tile)) {
                checkEqual = false;
                checkRight = false;
            } else if(gameMap[y][x + i].getColour() != tile.getColour()) {
                sameColour = false;
                numRow++;
            } else if(gameMap[y][x + i].getShape() != tile.getShape()) {
                sameShape = false;
                numRow++;
            }
        }
        //check tiles on the left is valid
        if(x >= i && checkLeft) {
            if(isEmpty(x - i, y)) {
                checkLeft = false;
            } else if(gameMap[y][x - i].isEqual(tile)) {
                checkEqual = false;
                checkLeft = false;
            } else if(gameMap[y][x - i].getColour() != tile.getColour()) {
                sameColour = false;
                numRow++;
            } else if(gameMap[y][x - i].getShape() != tile.getShape()) {
                sameShape = false;
                numRow++;
            }
        }
    }
    if((sameColour || sameShape) && checkEqual && numRow <= 5) {
        returnCheck = true;
    }
    return returnCheck;
}

//check if the location is empty
bool Qwirkle::isEmpty(const unsigned int& x, const unsigned int& y) {
    bool check = gameMap[y][x].isEqual(emptyTile);
    return check;
}

//check if game is over
bool Qwirkle::gameOver() {
    bool check = false;
    for(int i = 0; i < numPlayer; i++) {
        if(player[i].getTiles().getSize() == 0) {
            check = true;
        }
    }
    if(check) {
        int max = player[0].getScore();
        std::string winner = player[0].getName();
        for(int i = 0; i < numPlayer; i++) {
            max = std::max(max, player[i].getScore());
            if(player[i].getScore() == max) {
                winner = player[i].getName();
            }
        }
        std::cout << "WINNER: " << winner << std::endl;
    }
    return check;
}
