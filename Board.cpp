#include "Board.h"
Board::Board(LinkedList bag, Player* player) :
    bag(bag), player(player), emptyTile(9, ' '){
        initialMap();
}

Board::~Board(){
}

std::vector< std::vector<Tile> > Board::getMap() {
    return gameMap;
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

//initialise the map
void Board::initialMap() {
    std::vector<Tile> temp;
    for(int i = 0; i < 6; i++) {
        temp.clear();
        for(int j = 0; j < 6; j++) {
            temp.push_back(emptyTile);
        }
        gameMap.push_back(temp);
    }
}

//paint map with the frame
void Board::paintMap() {
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

//extend the size of the map
void Board::extendMap(unsigned int mapSize) {
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
    }
    else if(mapSize == gameMap.size() + 1) {
        for(unsigned int j = 0; j < gameMap.size(); j++) {
            gameMap[j].push_back(emptyTile);
            tempCol.push_back(emptyTile);
        }
        tempCol.push_back(emptyTile);
        gameMap.push_back(tempCol);
    }
}

//place the tile at the certain location and add the score of that tile as well
void Board::placeTile(std::vector<Tile> tile,int turn,
                        std::vector<std::string> location) {
    for(unsigned int i = 0; i < tile.size(); i++) {
        unsigned int y = location[i].substr(0, 1)[0] - 65;
        unsigned int x = strtol(location[i].substr(1).c_str(), NULL, 10);
        gameMap[y][x] = tile[i];
        player[turn].deleteTiles(tile[i]);
    }
    int score = calculateScore(tile, location);
    player[turn].setScore(score);
    if(bag.getSize() != 0) {
        for(unsigned int i = 0; i < tile.size(); i ++) {
            player[turn].addTiles(bag.draw());
        }
    }
}


int Board::calculateScore(std::vector<Tile> tile,
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
                }
                else {
                    rowScore[j] = 0;
                }
            }
            if(xVec[j] == xVec[j + 1]) {
                if(colScore[j] > colScore[j + 1]) {
                    colScore[j + 1] = 0;
                }
                else {
                    colScore[j] = 0;
                }
            }
        }
    }
    //set final score to the current player
    for(unsigned int i = 0; i < tile.size(); i++) {
        score += colScore[i] + rowScore[i];
    }
    return score;
}
//calculate the score the tile is going to get
int Board::calculateRowScore(Tile& tile, const unsigned int& x,
                               const unsigned int& y) {
    int score = 0;
    bool checkRight = true, checkLeft = true;
    //add up the score of the row
    for(unsigned int i = 1; i < 6; i++) {
        if(x + i < gameMap.size() && checkRight) {
            if(!isEmpty(x + i, y)) {
                score++;
            }
            else {
                checkRight = false;
            }
        }
        if(x >= i && checkLeft) {
            if(!isEmpty(x - i, y)) {
                score++;
            }
            else {
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
int Board::calculateColScore(Tile& tile, const unsigned int& x,
                               const unsigned int& y) {
    int score = 0;
    bool checkUp = true, checkDown = true;
    //add up the score of the column
    for(unsigned int i = 1; i < 6; i++) {
        if(y + i < gameMap.size() && checkUp) {
            if(!isEmpty(x, y + i)) {
                score++;
            }
            else {
                checkUp = false;
            }
        }
        if(y >= i && checkDown) {
            if(!isEmpty(x, y - i)) {
                score++;
            }
            else {
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
bool Board::isValid(std::vector<Tile>& tile,
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
        }
        else if(!isColValid(tile[i], x, y) || !isRowValid(tile[i], x, y)) {
            returnCheck = false;
        }
        else {
            gameMap[y][x] = tile[i];
            j++;
            if(j <= tile.size()) {
                i = 0;
                returnCheck = true;
            }
            else {
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
bool Board::isColValid(Tile& tile, const unsigned int& x,
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
            }
            else if(gameMap[y + i][x] == tile) {
                checkEqual = false;
                checkDown = false;
            }
            else {
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
            }
            else if(gameMap[y - i][x] == tile) {
                checkEqual = false;
                checkUp = false;
            }
            else {
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
bool Board::isRowValid(Tile& tile, const unsigned int& x,
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
            }
            else if(gameMap[y][x + i] == tile) {
                checkEqual = false;
                checkRight = false;
            }
            else {
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
            }
            else if(gameMap[y][x - i] == tile) {
                checkEqual = false;
                checkLeft = false;
            }
            else {
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
