
#include "Tile.h"
Tile::Tile(Shape shape, Colour colour) {
    this->colour = colour;
    this->shape = shape;
};

std::string Tile::print() {
    if(colour == ' ')
        return "  ";
    char temp[] = {colour,'\0'};
    std::string B = std::to_string(shape);
    std::string str = temp + B;
    return str;
}

Shape Tile::getShape() {
    return shape;
}

Colour Tile::getColour(){
    return colour;
}

bool Tile::isEqual(Tile tile) {
    if(this->colour == tile.getColour() && this->shape == tile.getShape())
        return true;
    return false;
}
