#define EXIT_SUCCESS    0
#include "Qwirkle.h"

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
}

Qwirkle::~Qwirkle() {
    delete[] player;
    player = nullptr;
}

//initialise the bag, put every tile into the bag
void Qwirkle::initialBag() {
    Shape shape[6] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};
    Colour colour[6] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
    for(int i = 0; i < 1; i++) {
        for(int j = 0; j < 6; j++) {
            for(int n = 0; n < 6; n++) {
                Tile tile(shape[j], colour[n]);
                bag.addNote(tile);
            }
        }
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
    }
    else if(choose == 2) {
        loadGame();
    }
    else if(choose == 3) {
        showInfo();
        menu();
    }
    else if(choose == 4) {
        std::cout << "Good Bye!" << std::endl;
    }
    else {
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
    board = new Board(bag, player);
    for(int i = 0; i < numPlayer; i++) {
        checkName = true;
        while(checkName) {
            std::cout << "Enter A Name For Player " << i + 1 <<
                      " (Uppercase Characters Only)" << std::endl;
            std::cout << "> ";
            std::cin >> name;
            if(!std::regex_match(name, std::regex("[A-Z]+"))) {
                std::cout << "Wrong Input, Enter Again" << std::endl;
            }
            else {
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
    outFile << board.getMap().size() << std::endl;
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
    }
    else {
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
    std::cout << "Name: <>" << std::endl;
    std::cout << "Student ID: <>" << std::endl;
    std::cout << "Email: <>" << std::endl << std::endl;
    std::cout << "Name: <Lucas Strilakos>" << std::endl;
    std::cout << "Student ID: <s3722050>" << std::endl;
    std::cout << "Email: <s3722050@student.rmit.edu.au>" << std::endl << std::endl;
    std::cout << "Name: <Ty Ty Chau>" << std::endl;
    std::cout << "Student ID: <s3668469>" << std::endl;
    std::cout << "Email: <s3668469@student.rmit.edu.au>" << std::endl << std::endl;
    std::cout << "----------------------------------------" << std::endl;
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
            std::cin >> command;
            saveGame(command);
        }
        else if(tempStr[0].compare("PLACE") == 0 && tempStr.size() == 4) {
            if(tempStr[2].compare("AT") == 0) {
                check = placeAtCommand(tempStr[1], tempStr[3]);
            }
        }
        else if(command.compare("QUIT") == 0
                || command.compare("EXIT") == 0) {
            gamePlay = false;
            check = true;
        }
        else if(command.compare("HELP") == 0) {
            helpCommand();
        }
        else if(tempStr[0].compare("REPLACE") == 0) {
            check = replaceCommand(tempStr[1]);
        }
        else {
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
    }
    else if(tileOnHold.find(",") != std::string::npos) {
        std::cout << "You Cant Replace Mutiple Tile" << std::endl;
    }
    else if(!player[turn].hasTile(tile[0])) {
        std::cout << "You Dont Have That Tile, Enter other tile" << std::endl;
    }
    else {
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
        }
        else if(x < 0 || x >= gameMap.size() || y < 0 || y >= gameMap.size()) {
            std::cout << location[i] << " Is Out Of Bound, Try Again" << std::endl;
            check = false;
        }
        else if(!isEmpty(x, y)) {
            std::cout << location[i]
                      << " Is Not Empty, Try Again" << std::endl;
            check = false;
        }
        else if(!player[turn].hasTile(tile[i])) {
            std::cout << "You Dont Have " << tile[i]
                      .print() << ", Enter Another Tile" <<
                      std::endl;
            check = false;
        }
        else if(!isValid(tile, location)) {
            check = false;
        }
        //if location is beyond the limit, extend it
        if(y == gameMap.size() - 1 || x == gameMap.size() - 1) {
            checkExtend = gameMap.size() + 1;
        }
        else if(x == 0 || y == 0) {
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

//check if the location is empty
bool Qwirkle::isEmpty(const unsigned int& x, const unsigned int& y) {
    bool check = false;
    if(x >= gameMap.size() || y >= gameMap.size()) {
        check = true;
    }
    else {
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
