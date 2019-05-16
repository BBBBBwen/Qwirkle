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
    if(head == nullptr) {
        head = newNode;
    } else {
        Node* curr = head;
        while(curr->next != nullptr) {
            curr = curr->next;
        }
        newNode->previous = curr;
        curr->next = newNode;
        curr = nullptr;
    }
    newNode = nullptr;
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
    curr = nullptr;
    delete curr;
    return newTile;
}

//draw a tile from a random node and delete the node
Tile LinkedList::draw() {
    Tile newTile = get(getRandom());
    deleteNode(newTile);
    return newTile;
}

//get a random integer for other purpose
int LinkedList::getRandom() {
    int rand = -1;
    std::random_device engine;
    std::uniform_int_distribution<int> dist(0, maxTiles);
    while(rand < 0) {
        rand = dist(engine);
    }
    return rand;
}

//replace the tile from a random node
Tile LinkedList::replaceTile(Tile tile) {
    int rand = getRandom();
    Tile newTile = get(rand);
    Node* temp = head;
    while(temp->tile != tile) {
        temp = temp->next;
    }
    temp->tile = tile;
    return newTile;
}

void LinkedList::deleteAll() {
    Node* curr = head;
    Node* temp = nullptr;
    while(curr -> next != nullptr) {
        temp = curr;
        curr = curr->next;
        delete temp;
        temp = nullptr;
    }
    delete curr;
    curr = nullptr;
}
