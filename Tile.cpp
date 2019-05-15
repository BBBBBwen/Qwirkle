#include "Tile.h"
Tile::Tile(Shape shape, Colour colour) :
    colour(colour),
    shape(shape) {
};

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
bool Tile::isEqual(Tile tile) {
    if(this->colour == tile.getColour() && this->shape == tile.getShape())
        return true;
    return false;
}
