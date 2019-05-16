#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H
#include <iostream>
#include <string>
// Define a Colour type
typedef char Colour;

// Define a Shape type
typedef int Shape;

class Tile {
public:
    Tile(Shape shape, Colour colour);
    Tile(std::string tileString);
    ~Tile();
    std::string print();
    Shape getShape();
    Colour getColour();
    bool isEqual(Tile tile);
    bool operator==(Tile& tile) const;
    bool operator!=(Tile& tile) const;
private:
    Colour colour;
    Shape  shape;
};

#endif // ASSIGN2_TILE_H
