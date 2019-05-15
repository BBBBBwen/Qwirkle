#include "Node.h"

Node::Node(Tile tile, Node* next, Node* previous) :
    tile(tile), next(next), previous(previous) {
}

Node::Node(Node& other) :
    tile(other.tile),
    next(other.next),
    previous(other.previous) {
}

Node::~Node() {
}
