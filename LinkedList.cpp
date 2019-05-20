#include "LinkedList.h"
#include <random>
#include <iostream>
LinkedList::LinkedList() :
    head(nullptr),
    maxTiles(0) {
}

LinkedList::~LinkedList() {
    for(int i = 0;i < maxTiles; i++) {
        Node* del = head;
        head = head->next;
        if(del){
            delete del;
            del = nullptr;
        }
    }
}

int LinkedList::getSize() {
    return maxTiles;
}

//add a tile to a node and add that node to the LinkedList
void LinkedList::addNote(Tile tile) {
    Node* newNode = new Node(tile, nullptr, nullptr);
    if(head == nullptr) {
        head = newNode;
    } else {
        Node* curr = head;
        while(curr->next != nullptr) {
            curr = curr->next;
        }
        newNode->previous = curr;
        curr->next = newNode;
    }
    this->maxTiles++;
}

//delete the node that the tile belonged to
void LinkedList::deleteNode(Tile delTile) {
    Node* curr = head;
    while(curr != nullptr && curr->tile != delTile) {
        curr = curr->next;
    }
    if(curr) {
        if(curr == head) {
            head = head->next;
        } else if(curr->next == nullptr) {
            curr->previous->next = nullptr;
        } else {
            curr->previous->next = curr->next;
            curr->next->previous = curr->previous;
        }
    }
    delete curr;
    curr = nullptr;
    this->maxTiles--;
}

//get the certain tile from the LinkedList
Tile LinkedList::get(int index) {
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
    Tile newTile = get(rand);
    deleteNode(newTile);
    return newTile;
}

//get a random integer for other purpose
int LinkedList::getRandom() {
    int rand = -1;
    std::random_device engine;
    std::uniform_int_distribution<int> dist(0, maxTiles - 1);
    while(rand < 0) {
        rand = dist(engine);
    }
    return rand;
}

//replace the tile from a random node
Tile LinkedList::replaceTile(Tile tile) {
    Tile newTile = draw();
    addNote(tile);
    return newTile;
}
