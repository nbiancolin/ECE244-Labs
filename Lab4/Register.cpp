
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem; //how long the cashier at the register takes to process each item
  overheadPerCustomer = overhead; // == setupTime, how long it takes the customer to get set up at the register
  availableTime = entryTime; //
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() { }//do I need a delete in here?

QueueList* Register::get_queue_list() {return queue;}

Register* Register::get_next() {return next; }

int Register::get_ID() { return ID; }

double Register::get_secPerItem() { return secPerItem; }

double Register::get_overheadPerCustomer() {  return overheadPerCustomer;}

double Register::get_availableTime() { return availableTime; }

void Register::set_next(Register* nextRegister) {  next = nextRegister;}


void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() { //should be update / set lmao
    // Get the departure time of the first customer in the queue
    // returns -1 if no customer is in the queue
    if(queue == nullptr) return -1;
    if(queue->get_head() == nullptr) return -1;
    //return queue->get_head()->get_departureTime();
    //if they arrive after the current time or (check if arrival time is bigger than available time
    double depTime = secPerItem * queue->get_head()->get_numOfItems() + overheadPerCustomer;
    if(queue->get_head()->get_arrivalTime() >= availableTime) return (depTime + queue->get_head()->get_arrivalTime());
    else return depTime;
}

void Register::departCustomer(QueueList* doneList) {
    // dequeue the head, set last dequeue time, add to doneList,
    if(queue == nullptr) return;
    if(queue->get_head() == nullptr) return;
    availableTime += calculateDepartTime(); //updates available time of register
    doneList->enqueue(queue->dequeue()); //moves head customer to donelist
    
}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
