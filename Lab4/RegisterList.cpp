#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr; //points to register
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
    Register* temp = head;
    while (temp != nullptr) {
        Register* next = temp->get_next();
        delete temp;
        temp = next;
    }
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() { 
  return size;
}


Register* RegisterList::get_min_items_register() {
    // loop through all registers to find the register with the least number of items
    int min = INT_MAX; // initialize to a large value
    Register* temp = head;
    Register* res = nullptr;

    while (temp != nullptr) {
        int num = temp->get_queue_list()->get_items();
        if (num < min) {
            min = num;
            res = temp;
        }
        temp = temp->get_next();
    }

    return res;
}

Register* RegisterList::get_free_register() {
    // return the register with no customers
    // if all registers are occupied, return nullptr
    Register* temp = head;
    while(temp != nullptr){
        if(temp->get_queue_list() == nullptr) return temp;
        temp = temp->get_next();
    }
    return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
    if (head == nullptr) {
        head = newRegister;
        size++;
        return;
    }
    Register* temp = head;
    while (temp->get_next() != nullptr) {
        temp = temp->get_next();
    }
    temp->set_next(newRegister);
    newRegister->set_next(nullptr);  // Set the next pointer to nullptr (just in case)
    size++;
  // Assume the next of the newRegister is set to null
  // You will have to increment size
}

bool RegisterList::foundRegister(int ID) {
    // look for a register with the given ID
    // return true if found, false otherwise
    Register* temp = head;
    while(temp != nullptr){
        if(temp->get_ID() == ID) return true;
        temp = temp->get_next();
    }
    return false;
}

Register* RegisterList::dequeue(int ID) {
    // dequeue the register with given ID
    //if(!foundRegister(ID)) return nullptr;
    Register* temp = head;
    Register* prev = nullptr;
    while(temp != nullptr){
        if(temp->get_ID() == ID) goto found;
        prev = temp;
        temp = temp->get_next();
    }
    return nullptr; //returns null if not found or if head is null
found: {
        Register* dequeue = temp;
        prev->set_next(temp->get_next()); //remove temp from the queue
        dequeue->set_next(nullptr); //nothing is working nd I cant figue out why
        return temp; //returns dequeued register
    }

}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
    // return the register with minimum time of departure of its customer
    if(head == nullptr) return nullptr;
    Register* res, *temp = head;
    //doing first round of calculations
    double min = head->calculateDepartTime();
    int counter = 0; //purpose of counter is to see if all registers are empty. Counter ticks up every time a register is 0
    if(min == 0) ++counter;
    while(temp->get_next() != nullptr){
        temp = temp->get_next();
        double val = temp->calculateDepartTime();
        if(val < min) {
            min = val;
            res = temp;
        } else if (val == 0) ++counter;
    }
    if(counter == size) return nullptr;
    return res;
    // if all registers are free, return nullptr
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
