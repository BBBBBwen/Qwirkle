#include "LinkedList.h"
#include <random>
#include <iostream>
LinkedList::LinkedList() :
    head(nullptr),
    maxTiles(0) {
}

LinkedList::~LinkedList() {
}

int LinkedList::getSize() {
    return maxTiles;
}

//add a tile to a node and add that node to the LinkedList
void LinkedList::addNote(Tile tile) {
    Node* newNode = new Node(tile, nullptr, nullptr);
    if(head == nullptr)
        head = newNode;
    else {
        Node* curr = head;
        while(curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = newNode;
        newNode->previous = curr;
    }
    this->maxTiles++;
}

//delete the node that the tile belonged to
void LinkedList::deleteNode(Tile delTile) {
    Node* delPtr = nullptr;
    Node* curr = head;
    Node* temp = head;
    while(curr != nullptr && !curr->tile.isEqual(delTile)) {
        temp = curr;
        curr = curr->next;
    }
    if(curr) {
        delPtr = curr;
        curr = curr->next;
        temp->next = curr;
        if(delPtr == head) {
            head = head->next;
            temp = nullptr;
        }
    }
    delete delPtr;
    this->maxTiles--;
}

//get the certain tile from the LinkedList
Tile LinkedList::get(unsigned int index) {
    Node* curr = head;
    for(int i = 0; i < index; i++) {
        curr = curr->next;
    }
    Tile newTile = curr->tile;
    return newTile;
}

//draw a tile from a random node and delete the node
Tile LinkedList::draw() {
    int rand = getRandom();
    Tile newTile = get(getRandom());
    deleteNode(newTile);
    return newTile;
}

//get a random integer for other purpose
int LinkedList::getRandom() {
    std::random_device engine;
    std::uniform_int_distribution<int> dist(0, maxTiles);
    return dist(engine);
}

//replace the tile from a random node
Tile LinkedList::replaceTile(Tile tile) {
    int rand = getRandom();
    Tile newTile = get(rand);
    Node* temp = head;
    while(!temp->tile.isEqual(tile)) {
        temp = temp->next;
    }
    temp->tile = tile;
    return newTile;
}

