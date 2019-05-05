
#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"

class Node {
public:

    Node(Tile tile, Node* next, Node* previous);
    Node(Node& other);
    ~Node();
    void setTile(Tile tile);

    Tile     tile;
    Node*    next;
    Node*    previous;
};

#endif // ASSIGN2_NODE_H
