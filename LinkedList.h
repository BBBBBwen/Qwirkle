#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include <vector>
class LinkedList {
public:

    LinkedList();
    ~LinkedList();
    int getSize();
    void addNote(Tile tile);
    Tile get(unsigned int index);
    Tile draw();
    Tile replaceTile(Tile tile);
    void deleteNode(Tile delTile);
    int getRandom();

private:
    Node* head;
    int maxTiles;

};

#endif // ASSIGN2_LINKEDLIST_H
