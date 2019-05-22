#include "Qwirkle.h"

// Main Program
int main(void) {
    Qwirkle* qwirkle = new Qwirkle();
    
    //Game Launched
    std::cout << "" << std::endl;
    std::cout << " [ "<<BUC<<"Welcome To Qwirkle!"<<RESET<<" ]" << WColour << std::endl << std::endl << std::endl;
    
    //Bring up menu
    qwirkle->menu();
    delete qwirkle;
    qwirkle = nullptr;
    return EXIT_SUCCESS;
}

// Test
void Qwirkle::testing(){
    numPlayer = 1;
    player = new Player[numPlayer];
    Tile superDraw= emptyTile;
    for(int i = 0; i<72; i++){
        superDraw = bag.draw();
        player[0].superDraw(superDraw);
    }

    std::string tile, position;

    while(std::cin >> tile >> position){
        placeAtCommand(tile, position, TEST);
    }

    paintMap(TEST);

}

Qwirkle::Qwirkle() :
    emptyTile(9, ' '), player(nullptr) {
    initialBag();
    initialMap();
}

Qwirkle::~Qwirkle() {
    if(player) {
        delete[] player;
        player = nullptr;
    }
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
    
    
    std::cout <<GColour<< " --------[ " <<BUG<< "Menu" <<RESET<< GColour << " ]--------" << WColour << std::endl << std::endl;
    
    std::cout << " 1. New Game" << std::endl;
    std::cout << " 2. Load Game" << std::endl;
    std::cout << " 3. Show Group Info" << std::endl;
    std::cout << " 4. Quit" << std::endl << std::endl;
    
    std::cout << GColour << " ------------------------" << WColour << std::endl;
    std::cout << " Select option: " << std::endl;
    std::cout << Cyan <<  " > ";
    
    
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
        std::cout << "" << std::endl << std::endl;
        std::cout << RColour << " [ Exiting Game ]  " << WColour << std::endl <<std::endl;
        gamePlay = false;
    } else {
        std::cout << RColour <<" Invalid input. Please only select number between 1 - 4." << WColour << std::endl;
        menu();
    }
    //delete \n in buffer
}

//start a new game
void Qwirkle::newGame() {
    bool checkName = true;
    std::string name = "";
    std::string str = "";
    
    
    std::cout << "" << std::endl;
    
    std::cout << Cyan << " [Loading Game]  " << WColour << std::endl << std::endl;
    
    
    while(numPlayer > 4 || numPlayer < 2) {
        
        std::cout << " Enter the amount of players(2 - 4):" << std::endl << std::endl;
        std::cout << Cyan << " > ";
        
        std::cin >> str;
        numPlayer = std::atoi(str.c_str());
    }
    player = new Player[numPlayer];
    for(int i = 0; i < numPlayer; i++) {
        checkName = true;
        while(checkName) {
            
            std::cout << WColour << " Enter Player " << i + 1 << "'s Name:"<<
                      " (Uppercase characters only)" << std::endl;
            std::cout << Cyan << " > ";
            
            std::cin >> name;
            if(!std::regex_match(name, std::regex("[A-Z]+"))) {
                
                std::cout << RColour << " Invalid input. Please try again." << WColour << std::endl;
                
            } else {
                player[i].setName(name);
                checkName = false;
            }
        }
    }
    
    
     std::cout << GColour << " ------------------------" << WColour << std::endl;
    std::cout << "      [ " << BUC << "Game Start"<< RESET <<" ]"<< WColour << std::endl;
    std::cout << GColour << " ------------------------" << WColour << std::endl << std::endl ;
    
    
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
    play();
}

//save game data to the file
void Qwirkle::saveGame(std::string fileName) {
    std::ofstream outFile;
    outFile.open(fileName + ".sav");
    //store name, score and tiles on hand first
    outFile << numPlayer << std::endl;
    for(int i = 0; i < numPlayer; i++) {
        outFile << player[i].getName() << std::endl;
        outFile << player[i].getScore() << std::endl;
        outFile << player[i].printPlainTiles() << std::endl;
    }
    //store map
    outFile << gameMap.size() << std::endl;
    for(unsigned int i = 0; i < gameMap.size(); i++) {
        for(unsigned int j = 0; j < gameMap[0].size(); j++) {
            outFile << gameMap[i][j].printPlainTile();
            if(j < gameMap[0].size() - 1) {
                outFile << ",";
            }
        }
        outFile << std::endl;
    }
    //store what is in the bag
    for(int i = 0; i < bag.getSize(); i++) {
        outFile << bag.get(i).printPlainTile();
        if(i < bag.getSize() - 1) {
            outFile << ",";
        }
    }
    outFile << std::endl << turn;
    outFile.close();
    std::cout << std::endl;
    std::cout << " Saved successfully." << std::endl << std::endl;
}

//load game data from the file
void Qwirkle::loadGame() {
    std::string tempBuffer;
    
    std::cout << "" << std::endl;
    
    std::cout << Cyan << " Input save name to load: " << std::endl << std::endl;
    
    std::cout << Cyan << " > ";
    
    
    std::cin >> tempBuffer;
    std::ifstream inFile(tempBuffer + ".sav");
    if(inFile.is_open()) {
        std::vector<Tile> temp;
        int numSize = 0;
        while(!inFile.eof()) {
            //retrieve player data
            std::getline(inFile, tempBuffer, '\n');
            numPlayer = std::stoi(tempBuffer.c_str());
            player = new Player[numPlayer];
            for(int i = 0; i < numPlayer; i++) {
                std::getline(inFile, tempBuffer, '\n');
                player[i].setName(tempBuffer);
                std::getline(inFile, tempBuffer, '\n');
                player[i].setScore(std::stoi(tempBuffer.c_str()));
                std::getline(inFile, tempBuffer, '\n');
                temp = splitToTile(tempBuffer, ",");
                for(unsigned int j = 0; j < temp.size(); j++) {
                    player[i].addTiles(temp[j]);
                }
            }
            //retrieve map
            std::getline(inFile, tempBuffer, '\n');
            numSize = std::stoi(tempBuffer.c_str());
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
            turn = std::stoi(tempBuffer.c_str());
        }
        //delete \n in buffer
        std::cin.get();
        //start playing
        play();
    } else {
        
        std::cout << "" << std::endl;
        std::cout << RColour << " No files are named: " << WColour << tempBuffer << std::endl << std::endl;
        
        std::cout << RColour << " Please try again." << WColour << std::endl << std::endl;
        
        menu();
    }
}

//split the string and store in a Tile vector
std::vector<Tile> Qwirkle::splitToTile(const std::string& str,
                                       const std::string& delimiter) {
    std::vector<Tile> tempVec;
    std::string strs = str + delimiter;
    size_t pos = strs.find(delimiter);
    // move from one delimiter to the next
    while(pos != std::string::npos) {
        Tile tile(emptyTile);
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
    std::cout << "" << std::endl << std::endl << std::endl;
    std::cout << " ----------------------------------------" << std::endl <<std::endl;
    
    std::cout << "             [ "<< BUR <<"Group Info"<<RESET<<" "<<Cyan<<"]" << WColour << std::endl << std::endl;
    
    std::cout << Cyan << " Name: "<< WColour <<"<Bowen Zhang>" << std::endl;
    
    std::cout << Cyan << " Student ID: "<< WColour <<"<s3617571>" << std::endl;
    
    std::cout << Cyan << " Email: "<< WColour <<"<s3617571@student.rmit.edu.au>" << std::endl << std::endl;
    
    /*===================================*/
    
    std::cout << Cyan << " Name: "<< WColour <<"<Cheng Chen>" << std::endl;
    
    std::cout << Cyan << " Student ID: "<< WColour <<"<s3728207>" << std::endl;
    
    std::cout << Cyan << " Email: "<< WColour <<"<s3728207@student.rmit.edu.au>" << std::endl << std::endl;
    
    /*===================================*/
    
    std::cout << Cyan << " Name: "<< WColour <<"<Lucas Strilakos>" << std::endl;
    std::cout << Cyan << " Student ID: "<< WColour <<"<s3722050>" << std::endl;
    std::cout << Cyan << " Email: "<< WColour <<"<s3722050@student.rmit.edu.au>" << std::endl << std::endl;
    
    /*===================================*/
    
    std::cout << Cyan << " Name: "<< WColour <<"<Ty Ty Chau>" << std::endl;
    std::cout << Cyan << " Student ID: "<< WColour <<"<s3668469>" << std::endl;
    std::cout << Cyan << " Email: "<< WColour <<"<s3668469@student.rmit.edu.au>" << std::endl << std::endl;
    
    
    std::cout << " ----------------------------------------" << std::endl << std::endl << std::endl;
}

//paint map with the frame
void Qwirkle::paintMap(bool isTest) {
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
            if(isTest){
                std::cout << gameMap[i][j].printPlainTile() << "|";
            }else{
                std::cout << gameMap[i][j].print() << "|";
            }
        }
        std::cout << std::endl;
    }
}

//processing the command
void Qwirkle::command() {
    bool check = false;
    while(!check) {
        std::cout << Cyan << " > ";
        
        std::string command;
        std::vector<std::string> tempStr;
        std::getline(std::cin, command);
        toUpperCase(command);
        tempStr = splitToString(command, " ");
        if(command.compare("SAVE") == 0) {
           std::cout << std::endl;
            std::cout << " Save file as: " << std::endl << std::endl;
            
            std::cout << Cyan << " > ";
            std::cin >> command;
            saveGame(command);
            check = true;
            menu();
        } else if(tempStr[0].compare("PLACE") == 0 && tempStr.size() == 4) {
            if(tempStr[2].compare("AT") == 0) {
                check = placeAtCommand(tempStr[1], tempStr[3], GAME);
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
            std::cout << RColour << " Invalid Command." << WColour << std::endl;
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
        
        std::cout << RColour << " Invalid tile." << WColour << std::endl;
        
    } else if(tileOnHold.find(",") != std::string::npos) {
        
        std::cout << std::endl;
        std::cout << RColour << " You can't replace multiple tiles." << WColour << std::endl;
        
    } else if(!player[turn].hasTile(tile[0])) {
        std::cout << std::endl;
         std::cout << RColour << " Invalid tiles. Please try again." << WColour << std::endl << std::endl;
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
    std::cout << std::endl;
    
    std::cout <<GColour<< " ----------------[ " <<BUG<< "Help Menu" <<RESET<< GColour << " ]----------------" << WColour << std::endl;
    
    std::cout << std::endl;
    std::cout << GColour << " [ Save ]" << std::endl;
    std::cout << WColour << " Save current game." << std::endl << std::endl << std::endl;
    
    
    
    std::cout << GColour << " [ Place ]" << std::endl;
    std::cout << WColour << " Place the chosen tiles at a chosen location on the game board." << std::endl;
    std::cout << "     - " << BUM << "Example" << RESET << "" << WColour << ": Place R2(Tile) at B3(Location)." << std::endl << std::endl;
    
    std::cout << " To place multiple tiles at multiple locates, separate it with a comma." << std::endl;
    std::cout << "     - " << BUM << "Example" << RESET << "" << WColour << ": Place R2,Y2 at B3, B4." << std::endl << std::endl << std::endl;
    
    
    
    std::cout << GColour << " [ Replace ]" << std::endl;
    std::cout << WColour <<" Replace a tile that's in your hand. (Doing this will count as a turn.)" << std::endl;
    std::cout << "     - " << BUM << "Example" << RESET << "" << WColour << ": Replace Y4(Tile on hand)." << std::endl << std::endl << std::endl;
    
    
    std::cout << GColour << " [ Quit/Exit ]" << std::endl;
    std::cout << WColour <<" End the game." << std::endl << std::endl << std::endl;
    
    
    std::cout << GColour << " [ Help ]" <<std::endl;
    std::cout << WColour <<" Display the available commands." << std::endl << std::endl << std::endl;
        
    std::cout << GColour << " ------------------------" << WColour << std::endl;
}

//processing place at command
bool Qwirkle::placeAtCommand(std::string tiles, std::string locations, bool isTest) {
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
        if(tile.size() != location.size()) {
            
            std::cout << "" << std::endl;
            std::cout << RColour <<  " Tile and location must be equal." << WColour << std::endl << std::endl;
            
            i = tile.size();
            check = false;
        } else if(colours.find(tile[i].getColour()) == std::string::npos
                  || tile[i].getShape() < 0
                  || tile[i].getShape() > 6) {
            std::cout << RColour << " Tile does not exists." << WColour << std::endl << std::endl;
            
            check = false;
        } else if(x < 0 || x >= gameMap.size() || y < 0 || y >= gameMap.size()) {
            
            std::cout << "" << std::endl;
            
            std::cout << RColour << " " << location[i] << " is out of bound. Try again." << WColour << std::endl << std::endl;
            
        } else if(!isEmpty(x, y)) {
            
            std::cout << "" << std::endl;
            
            std::cout << RColour << " " << location[i]
                      << " is not an empty location. Try Again." << WColour << std::endl << std::endl;

            check = false;
        } else if(!player[turn].hasTile(tile[i])) {
          
            std::cout << "" << std::endl;
            
            std::cout << RColour << " You do not have " << tile[i]
                      .print() << RColour <<", please select another tile." << WColour <<
                      std::endl << std::endl;

            check = false;
        } else if(!isValid(tile, location, isTest)) {
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
    
    while(gamePlay) {
        std::cout << std::endl;
        std::cout << GColour << "-------------------------------------------------------------" << std::endl;
        std::cout << Cyan << " [ It is "<<player[turn].getName() << "'s turn ]" << WColour << std::endl <<std::endl;
        
        for(int i = 0; i < numPlayer; i++) {
            std::cout << " [" << player[i].getName() << "'s score: " << player[i].getScore()
                      << "]" << std::endl;
        }
        std::cout << std::endl << std::endl;
        paintMap(GAME);
        std::cout << std::endl << " Tiles on hands: " << player[turn].printTiles() <<
                  std::endl;

        std::cout <<" "<< std::endl;
        command();
        gameOver();
    }
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
        std::cout << " Qwirkle!" << std::endl;
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
        std::cout << " Qwirkle!" << std::endl;
        score += 6;
    }
    if(score > 0) {
        score += 1;
    }
    return score;
}

//check if the location that the tile needs to place is valid or not
bool Qwirkle::isValid(std::vector<Tile>& tile,
                      std::vector<std::string> location, bool isTest) {
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
            if(isTest){
                std::cout << "" << std::endl;
                std::cout << RColour <<"You can't place " << tile[i].print() << RColour << " at " << location[i] << WColour <<
                      std::endl << std::endl;
            }else{
                std::cout << "" << std::endl;
                std::cout << RColour <<"You can't place " << tile[i].print() << RColour << " at " << location[i] << WColour <<
                      std::endl << std::endl;
            }
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
    
        std::cout << GColour << "-------------------------------------------------------------" << std::endl <<std::endl;
        std::cout << WColour;
        std::cout  << " ["<<BUY<<"Scoreboard"<<RESET<<WColour<<"]" << std::endl;
        
        std::cout << "" << std::endl;
        for(int i = 0; i < numPlayer; i++) {
           std::cout << " " << "Score For " << player[i].getName() << ": " << player[i].getScore() << std::endl;
        }
        
        std::cout << std::endl;
        std::cout << " "<<BUM<<"WINNER"<<RESET<<": " << winner <<std::endl <<std::endl;
        
        std::cout <<GColour<< " --------[ " <<BUC<< "Thanks for playing" <<RESET<< GColour << " ]--------" << WColour << std::endl << std::endl;
    }
}
