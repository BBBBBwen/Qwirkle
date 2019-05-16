#include "Tile.h"
#include "ColourText.h"

Tile::Tile(Shape shape, Colour colour) :
    colour(colour),
    shape(shape) {
};

Tile::Tile(std::string tileString) :
    colour(tileString[0]),
    shape(tileString[1] - 48) {
}

Tile::~Tile() {
}

//print the tile as string
std::string Tile::print() {
    if(colour == ' ')
        return "  ";
    char temp[] = {colour, '\0'};
    std::string B = std::to_string(shape);
    std::string str = temp + B;
    if(colour == 'R') {
        std::string tc = RED + str + WHITE;
        str = tc;
    } else if(colour == 'G') {
        std::string tc = GREEN + str + WHITE;
        str = tc;
    } else if(colour == 'Y') {
        std::string tc = YELLOW + str + WHITE;
        str = tc;
    } else if(colour == 'B') {
        std::string tc = BLUE + str + WHITE;
        str = tc;
    } else if(colour == 'P') {
        std::string tc = MAGENTA + str + WHITE;
        str = tc;
    } else if(colour == 'O') {
        std::string tc = ORANGE + str + WHITE;
        str = tc;
    }
    return str;
}

//get the shape
Shape Tile::getShape() {
    return shape;
}

//get the colour
Colour Tile::getColour() {
    return colour;
}

//check if two tiles are completely equal
bool Tile::operator==(Tile& tile) const {
    bool check = false;
    if(colour == tile.getColour() && shape == tile.getShape()) {
        check = true;
    }
    return check;
}

bool Tile::operator!=(Tile& tile) const {
    bool check = false;
    if(colour != tile.getColour() && shape != tile.getShape()) {
        check = true;
    }
    return check;
}
