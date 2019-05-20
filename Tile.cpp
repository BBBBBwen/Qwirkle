#include "Tile.h"
#include "ColourText.h"

Tile::Tile(Shape shape, Colour colour) :
    colour(colour),
    shape(shape) {
};

Tile::Tile(std::string tileString) :
    colour(tileString[0]),
    shape(strtol(tileString.substr(1).c_str(), NULL, 10)) {
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
    bool check = true;
    if(this->colour == tile.getColour() && this->shape == tile.getShape()) {
        check = false;
    }
    return check;
}
