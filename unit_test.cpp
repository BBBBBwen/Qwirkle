#include "TileCodes.h"
#include "Qwirkle.h"
#include "Player.h"
#include "LinkedList.h"

#include <fstream>
#include <iostream>
#include <string>

class unitTest {
public:


    std::vector< std::vector<Tile> > gameMap;
    Tile emptyTile;
};

int main(std::string args) {
    checkArgs(args);
    loadInput(args);
    loadOutput(args);
    return 0;
}

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

void checkArgs(std::string args) {
    std::string inputFilename = args + ".input";
    std::string outputFilename = args + ".output";
    if(DEBUG) {
        std::cout << "Input filename: " << mazeFilename << std::endl;
        std::cout << "Output filename: " << obsFilename << std::endl;
    }
    // Check files exist
    auto checkFile = [](std::string & filename) {
        std::ifstream in;
        in.open(filename);
        if(in.fail()) {
            throw std::runtime_error("Could not open file '" + filename + "'");
        }
        in.close();
    };
    checkFile(mazeFilename);
    checkFile(obsFilename);
    checkFile(posFilename);
}

void loadInput(std::string args) {
    std::string filename = args + ".input";
    std::ifstream in(filename);
    std::vector<std::string> lines;
    if(in.is_open()) {
        int numSize = 0;
        while(!inFile.eof()) {
            std::getline(inFile, tempBuffer, '\n');
            lines.push_back(tempBuffer);
        }
    }
    load_lines(lines);
    if(lines.size() == 0) {
        throw std::runtime_error("No Maze in file");
    }
}

void loadLines(std::vector<std::string> lines) {

}
