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
    char temp[] = {colour, '\0'};
    std::string B = std::to_string(shape);
    std::string str = temp + B;
    if(colour == ' ') {
        str = "  ";
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
