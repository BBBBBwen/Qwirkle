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
    qwirkle = nullptr;
    return EXIT_SUCCESS;
}

Qwirkle::Qwirkle() :
    emptyTile(9, ' ') {
    initialBag();
    initialMap();
}

Qwirkle::~Qwirkle() {
}

//initialise the bag, put every tile into the bag
void Qwirkle::initialBag() {
    Shape shape[6] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};
    Colour colour[6] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
    for(int i = 0; i < 2; i++) {
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
    std::string str;
    int choose;
    std::cout << "-----Menu-----" << std::endl;
    std::cout << "1.New Game" << std::endl;
    std::cout << "2.Load Game" << std::endl;
    std::cout << "3.Show Info" << std::endl;
    std::cout << "4.Quit" << std::endl << std::endl;
    std::cout << "> ";
    std::cin >> str;
    std::cin.get();
    choose = std::atoi(str.c_str());
    if(choose == 1) {
        newGame();
    } else if(choose == 2) {
        loadGame();
    } else if(choose == 3) {
        showInfo();
        menu();
    } else if(choose == 4) {
        std::cout << "Good Bye!" << std::endl;
        gamePlay = false;
    } else {
        std::cout << "Unrecognised Input, Try Again" << std::endl;
        menu();
    }
    //delete \n in buffer
}

//start a new game
void Qwirkle::newGame() {
    bool checkName = true;
    std::string name = "";
    std::string str = "";
    std::cout << "Starting A New Game" << std::endl;
    while(numPlayer > 4 || numPlayer < 2) {
        std::cout << "Enter The Number Of Player(2 - 4)" << std::endl;
        std::cin >> str;
        numPlayer = std::atoi(str.c_str());
    }
    player = new Player[numPlayer];
    for(int i = 0; i < numPlayer; i++) {
        checkName = true;
        while(checkName) {
            std::cout << "Enter A Name For Player " << i + 1 <<
                      " (Uppercase Characters Only)" << std::endl;
            std::cout << "> ";
            std::cin >> name;
            if(!std::regex_match(name, std::regex("[A-Z]+"))) {
                std::cout << "Wrong Input, Enter Again" << std::endl;
            } else {
                player[i].setName(name);
                checkName = false;
            }
        }
    }
    std::cout << "Let's Play!" << std::endl << std::endl;
    //initialise players hand tiles
    Tile tile = emptyTile;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < numPlayer; j++) {
            tile = bag.draw();
            player[j].addTiles(tile);
        }
    }
    //delete \n in buffer
    std::cin.get();
    //start playing
    while(gamePlay) {
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
    std::cout << "Save Successfully" << std::endl;
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
                temp = splitToTile(tempBuffer, ",");
                for(unsigned int j = 0; j < temp.size(); j++) {
                    player[i].addTiles(temp[j]);
                }
            }
            //retrieve map
            std::getline(inFile, tempBuffer, '\n');
            numSize = std::stoi(tempBuffer);
            gameMap.clear();
            for(int i = 0; i < numSize; i++) {
                std::getline(inFile, tempBuffer, '\n');
                temp = splitToTile(tempBuffer, ",");
                gameMap.push_back(temp);
            }
            //retrieve bag
            std::getline(inFile, tempBuffer, '\n');
            temp = splitToTile(tempBuffer, ",");
            for(unsigned int i = 0; i < temp.size(); i++) {
                bag.addNote(temp[i]);
            }
            std::getline(inFile, tempBuffer, '\n');
            turn = std::stoi(tempBuffer);
        }
        //delete \n in buffer
        std::cin.get();
        //start playing
        while(gamePlay) {
            play();
        }
    } else {
        std::cout << "There Is No File Called: " << tempBuffer << std::endl;
        std::cout << "The Game Is Shutting Down " << std::endl;
        menu();
    }
}

//split the string and store in a Tile vector
std::vector<Tile> Qwirkle::splitToTile(const std::string& str,
                                       const std::string& delimiter) {
    Tile tile(emptyTile);
    std::vector<Tile> tempVec;
    std::string strs = str + delimiter;
    size_t pos = strs.find(delimiter);
    // move from one delimiter to the next
    while(pos != std::string::npos) {
        std::string tempStr = strs.substr(0, pos);
        if(tempStr.compare("  ") != 0) {
            tile = (tempStr);
        }
        tempVec.push_back(tile);
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(delimiter);
    }
    return tempVec;
}

//split the string and store in a string vector
std::vector<std::string> Qwirkle::splitToString(const std::string& str,
        const std::string& delimiter) {
    std::vector<std::string> tempVec;
    std::string strs = str + delimiter;
    size_t pos = strs.find(delimiter);
    // move from one delimiter to the next
    while(pos != std::string::npos) {
        std::string tempStr = strs.substr(0, pos);
        tempVec.push_back(tempStr);
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
    std::cout << "Name: <Cheng Chen>" << std::endl;
    std::cout << "Student ID: <s3728207>" << std::endl;
    std::cout << "Email: <s3728207@student.rmit.edu.au>" << std::endl << std::endl;
    std::cout << "Name: <Lucas Strilakos>" << std::endl;
    std::cout << "Student ID: <s3722050>" << std::endl;
    std::cout << "Email: <s3722050@student.rmit.edu.au>" << std::endl << std::endl;
    std::cout << "Name: <Ty Ty Chau>" << std::endl;
    std::cout << "Student ID: <s3668469>" << std::endl;
    std::cout << "Email: <s3668469@student.rmit.edu.au>" << std::endl << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

//paint map with the frame
void Qwirkle::paintMap() {
    std::cout << "   ";
    for(unsigned int i = 0; i < gameMap[0].size(); i++) {
        if(i < 10)
            std::cout << i << "  ";
        else
            std::cout << i << " ";
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
        std::cout << "> ";
        std::string command;
        std::vector<std::string> tempStr;
        std::getline(std::cin, command);
        toUpperCase(command);
        tempStr = splitToString(command, " ");
        if(command.compare("SAVE") == 0) {
            std::cout << "Enter File Name You Want To Save" << std::endl;
            std::cout << "> ";
            std::cin >> command;
            saveGame(command);
            check = true;
            menu();
        } else if(tempStr[0].compare("PLACE") == 0 && tempStr.size() == 4) {
            if(tempStr[2].compare("AT") == 0) {
                check = placeAtCommand(tempStr[1], tempStr[3]);
            }
        } else if(command.compare("QUIT") == 0
                  || command.compare("EXIT") == 0) {
            gamePlay = false;
            check = true;
        } else if(command.compare("HELP") == 0) {
            helpCommand();
        } else if(tempStr[0].compare("REPLACE") == 0) {
            check = replaceCommand(tempStr[1]);
        } else {
            std::cout << "Unrecognised Command" << std::endl;
        }
    }
}

bool Qwirkle::replaceCommand(std::string tileOnHold) {
    bool check = false;
    std::vector<Tile> tile = splitToTile(tileOnHold, ",");
    std::string colours = "ROYGBP";
    if(colours.find(tile[0].getColour()) == std::string::npos
            || tile[0].getShape() < 0 || tile[0].getShape() > 6
            || tileOnHold.length() > 2) {
        std::cout << "Tile Doesnt Exists" << std::endl;
    } else if(tileOnHold.find(",") != std::string::npos) {
        std::cout << "You Cant Replace Mutiple Tile" << std::endl;
    } else if(!player[turn].hasTile(tile[0])) {
        std::cout << "You Dont Have That Tile, Enter other tile" << std::endl;
    } else {
        Tile replaceTile = bag.replaceTile(tile[0]);
        player[turn].deleteTiles(tile[0]);
        player[turn].addTiles(replaceTile);
        check = true;
    }
    switchTurn();
    return check;
}

//processing help command
void Qwirkle::helpCommand() {
    std::cout << "Save   -   Save Game" << std::endl;
    std::cout << "Place (Tile) At (Location)   -   " <<
              "Choose A Tile To Place At Certain Location" << std::endl;
    std::cout <<
              "Place (Tile),(Tile)... At (Location),(Location)...   -   Place Mutiple Tile Using ',' To Seperate Tile And Location"
              << std::endl;
    std::cout << "Replace (Tile)   -   Replace The Chosen Tile With A New One" <<
              std::endl;
    std::cout << "Quit/Exit   -   Return To Menu" << std::endl;
    std::cout << "Help   -   Get Help" << std::endl;
}

//processing place at command
bool Qwirkle::placeAtCommand(std::string tiles, std::string locations) {
    bool check = true;
    int checkExtend = 1;
    //take following three string check if it is right
    std::vector<Tile> tile = splitToTile(tiles, ",");
    std::vector<std::string> location = splitToString(locations, ",");
    std::string colours = "ROYGBP";
    //see if command is valid
    for(unsigned int i = 0; i < tile.size(); i++) {
        unsigned int y = location[i].substr(0, 1)[0] - 65;
        unsigned int x = strtol(location[i].substr(1).c_str(), NULL, 10);
        //check validation
        if(colours.find(tile[i].getColour()) == std::string::npos
                || tile[i].getShape() < 0
                || tile[i].getShape() > 6) {
            std::cout << "Tile Doesnt Exists" << std::endl;
            check = false;
        } else if(x < 0 || x >= gameMap.size() || y < 0 || y >= gameMap.size()) {
            std::cout << location[i] << " Is Out Of Bound, Try Again" << std::endl;
            check = false;
        } else if(!isEmpty(x, y)) {
            std::cout << location[i]
                      << " Is Not Empty, Try Again" << std::endl;
            check = false;
        } else if(!player[turn].hasTile(tile[i])) {
            std::cout << "You Dont Have " << tile[i]
                      .print() << ", Enter Another Tile" <<
                      std::endl;
            check = false;
        } else if(!isValid(tile, location)) {
            check = false;
        }
        //if location is beyond the limit, extend it
        if(y == gameMap.size() - 1 || x == gameMap.size() - 1) {
            checkExtend = gameMap.size() + 1;
        } else if(x == 0 || y == 0) {
            checkExtend = 0;
        }
    }
    if(check) {
        placeTile(tile, location);
        extendMap(checkExtend);
        isFirstTile = false;
        switchTurn();
    }
    return check;
}

// transform tp upper case
void Qwirkle::toUpperCase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

//extend the size of the map
void Qwirkle::extendMap(unsigned int mapSize) {
    std::vector<std::vector<Tile>> temp;
    std::vector<Tile> tempCol;
    if(mapSize == 0) {
        std::vector<Tile> tempFront;
        for(unsigned int i = 0; i < gameMap.size() + 1; i++) {
            tempFront.push_back(emptyTile);
        }
        temp.push_back(tempFront);
        for(unsigned int i = 0; i < gameMap.size(); i++) {
            tempCol.clear();
            tempCol.push_back(emptyTile);
            for(unsigned int j = 0; j < gameMap[i].size(); j++) {
                tempCol.push_back(gameMap[i][j]);
            }
            temp.push_back(tempCol);
        }
        gameMap = temp;
    } else if(mapSize == gameMap.size() + 1) {
        for(unsigned int j = 0; j < gameMap.size(); j++) {
            gameMap[j].push_back(emptyTile);
            tempCol.push_back(emptyTile);
        }
        tempCol.push_back(emptyTile);
        gameMap.push_back(tempCol);
    }
}

//switch the player
void Qwirkle::switchTurn() {
    turn++;
    if(turn == numPlayer) {
        turn = 0;
    }
}

//the board that game mainly run
void Qwirkle::play() {
    std::cout << player[turn].getName() << ", it's your turn" << std::endl;
    for(int i = 0; i < numPlayer; i++) {
        std::cout << "Score For " << player[i].getName() << ": " << player[i].getScore()
                  << std::endl;
    }
    paintMap();
    std::cout << std::endl << "Your Hand Is: " << player[turn].printTiles() <<
              std::endl;
    std::cout << std::endl;
    command();
    gameOver();
}

//place the tile at the certain location and add the score of that tile as well
void Qwirkle::placeTile(std::vector<Tile> tile,
                        std::vector<std::string> location) {
    std::vector<int> colScore;
    std::vector<int> rowScore;
    std::vector<int> xVec;
    std::vector<int> yVec;
    int score = player[turn].getScore();
    if(isFirstTile && tile.size() == 1) {
        score += 1;
    }
    //place tile and store useful data for getting score
    for(unsigned int i = 0; i < tile.size(); i++) {
        unsigned int y = location[i].substr(0, 1)[0] - 65;
        unsigned int x = strtol(location[i].substr(1).c_str(), NULL, 10);
        gameMap[y][x] = tile[i];
        player[turn].deleteTiles(tile[i]);
        xVec.push_back(x);
        yVec.push_back(y);
        rowScore.push_back(calculateRowScore(tile[i], x, y));
        colScore.push_back(calculateColScore(tile[i], x, y));
    }
    //delete the duplicated score
    for(unsigned int i = 0; i < xVec.size() - 1; i++) {
        for(unsigned int j = i; j < xVec.size() - 1; j++) {
            if(yVec[j] == yVec[j + 1]) {
                if(rowScore[j] > rowScore[j + 1]) {
                    rowScore[j + 1] = 0;
                } else {
                    rowScore[j] = 0;
                }
            }
            if(xVec[j] == xVec[j + 1]) {
                if(colScore[j] > colScore[j + 1]) {
                    colScore[j + 1] = 0;
                } else {
                    colScore[j] = 0;
                }
            }
        }
    }
    //set final score to the current player
    for(unsigned int i = 0; i < tile.size(); i++) {
        score += colScore[i] + rowScore[i];
    }
    player[turn].setScore(score);
    if(bag.getSize() != 0) {
        for(unsigned int i = 0; i < tile.size(); i ++) {
            player[turn].addTiles(bag.draw());
        }
    }
}

//calculate the score the tile is going to get
int Qwirkle::calculateRowScore(Tile& tile, const unsigned int& x,
                               const unsigned int& y) {
    int score = 0;
    bool checkRight = true, checkLeft = true;
    //add up the score of the row
    for(unsigned int i = 1; i < 6; i++) {
        if(x + i < gameMap.size() && checkRight) {
            if(!isEmpty(x + i, y)) {
                score++;
            } else {
                checkRight = false;
            }
        }
        if(x >= i && checkLeft) {
            if(!isEmpty(x - i, y)) {
                score++;
            } else {
                checkLeft = false;
            }
        }
    }
    //calculate final score
    if(score == 5) {
        std::cout << "Qwirkle!" << std::endl;
        score += 6;
    }
    if(score > 0) {
        score += 1;
    }
    return score;
}
//calculate the score the tile is going to get
int Qwirkle::calculateColScore(Tile& tile, const unsigned int& x,
                               const unsigned int& y) {
    int score = 0;
    bool checkUp = true, checkDown = true;
    //add up the score of the column
    for(unsigned int i = 1; i < 6; i++) {
        if(y + i < gameMap.size() && checkUp) {
            if(!isEmpty(x, y + i)) {
                score++;
            } else {
                checkUp = false;
            }
        }
        if(y >= i && checkDown) {
            if(!isEmpty(x, y - i)) {
                score++;
            } else {
                checkDown = false;
            }
        }
    }
    //calculate final score
    if(score == 5) {
        std::cout << "Qwirkle!" << std::endl;
        score += 6;
    }
    if(score > 0) {
        score += 1;
    }
    return score;
}

//check if the location that the tile needs to place is valid or not
bool Qwirkle::isValid(std::vector<Tile>& tile,
                      std::vector<std::string> location) {
    bool returnCheck = true;
    bool checkShape = true;
    bool checkColour = true;
    Shape shape = tile[0].getShape();
    Colour colour = tile[0].getColour();
    unsigned int j = 0;
    //
    for(unsigned int i = 0; i < tile.size(); i++) {
        int y = location[i].substr(0, 1)[0] - 65;
        int x = strtol(location[i].substr(1).c_str(), NULL, 10);
        if(tile[i].getShape() != shape) {
            checkShape = false;
        }
        if(tile[i].getColour() != colour) {
            checkColour = false;
        }
        if(isEmpty(x + 1, y) && isEmpty(std::abs(x - 1), y) &&
                isEmpty(x, y + 1) && isEmpty(x, std::abs(y - 1))) {
            returnCheck = isFirstTile;
        } else if(!isColValid(tile[i], x, y) || !isRowValid(tile[i], x, y)) {
            returnCheck = false;
        } else {
            gameMap[y][x] = tile[i];
            j++;
            if(j <= tile.size()) {
                i = 0;
                returnCheck = true;
            } else {
                i = tile.size();
            }
        }
    }
    //
    if(!checkColour && !checkShape) {
        returnCheck = false;
    }
    for(unsigned int i = 0; i < tile.size(); i++) {
        if(!returnCheck) {
            std::cout << "You Cant Place " << tile[i].print() << " At " << location[i] <<
                      std::endl;
        }
        int y = location[i].substr(0, 1)[0] - 65;
        int x = strtol(location[i].substr(1).c_str(), NULL, 10);
        gameMap[y][x] = emptyTile;
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
    for(unsigned int i = 1; i < 7; i++) {
        //check tiles below is valid
        if(y + i < gameMap.size() && checkDown) {
            if(isEmpty(x, y + i)) {
                checkDown = false;
            } else if(gameMap[y + i][x] == tile) {
                checkEqual = false;
                checkDown = false;
            } else {
                if(gameMap[y + i][x].getColour() != tile.getColour()) {
                    sameColour = false;
                    numCol++;
                }
                if(gameMap[y + i][x].getShape() != tile.getShape()) {
                    sameShape = false;
                    numCol++;
                }
            }
        }
        //check tiles above is valid
        if(y >= i && checkUp) {
            if(isEmpty(x, y - i)) {
                checkUp = false;
            } else if(gameMap[y - i][x] == tile) {
                checkEqual = false;
                checkUp = false;
            } else {
                if(gameMap[y - i][x].getColour() != tile.getColour()) {
                    sameColour = false;
                    numCol++;
                }
                if(gameMap[y - i][x].getShape() != tile.getShape()) {
                    sameShape = false;
                    numCol++;
                }
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
    for(unsigned int i = 1; i < 7; i++) {
        //check tiles on the right is valid
        if(x + i < gameMap.size() && checkRight) {
            if(isEmpty(x + i, y)) {
                checkRight = false;
            } else if(gameMap[y][x + i] == tile) {
                checkEqual = false;
                checkRight = false;
            } else {
                if(gameMap[y][x + i].getColour() != tile.getColour()) {
                    sameColour = false;
                    numRow++;
                }
                if(gameMap[y][x + i].getShape() != tile.getShape()) {
                    sameShape = false;
                    numRow++;
                }
            }
        }
        //check tiles on the left is valid
        if(x >= i && checkLeft) {
            if(isEmpty(x - i, y)) {
                checkLeft = false;
            } else if(gameMap[y][x - i] == tile) {
                checkEqual = false;
                checkLeft = false;
            } else {
                if(gameMap[y][x - i].getColour() != tile.getColour()) {
                    sameColour = false;
                    numRow++;
                }
                if(gameMap[y][x - i].getShape() != tile.getShape()) {
                    sameShape = false;
                    numRow++;
                }
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
    bool check = false;
    if(x >= gameMap.size() || y >= gameMap.size()) {
        check = true;
    } else {
        check = gameMap[y][x] == emptyTile;
    }
    return check;
}

//check if game is over
void Qwirkle::gameOver() {
    for(int i = 0; i < numPlayer; i++) {
        if(player[i].getNumTiles() == 0) {
            gamePlay = false;
        }
    }
    if(bag.getSize() == 0) {
        gamePlay = false;
    }
    if(!gamePlay) {
        int max = player[0].getScore();
        std::string winner = player[0].getName();
        for(int i = 0; i < numPlayer; i++) {
            max = std::max(max, player[i].getScore());
            if(player[i].getScore() == max) {
                winner = player[i].getName();
            }
        }
        std::cout << "Game Over" << std::endl;
        for(int i = 0; i < numPlayer; i++) {
            std::cout << "Score For " << player[i].getName() << ": " << player[i].getScore()
                      << std::endl;
        }
        std::cout << "Player " << winner << " Won!" << std::endl;
        std::cout << "Goodbye" << std::endl;
    }
}
