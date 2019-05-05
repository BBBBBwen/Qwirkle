#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H
#include <string>
// Define a Colour type
typedef char Colour;

// Define a Shape type
typedef int Shape;

class Tile {
public:
    Tile(Shape shape, Colour colour);
    std::string print();
    Shape getShape();
    Colour getColour();
    bool isEqual(Tile tile);
private:
    Colour colour;
    Shape  shape;
};

#endif // ASSIGN2_TILE_H
