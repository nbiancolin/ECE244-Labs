#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  //TODO: Learn how the destructor should work
    Customer* temp = head;
    while (temp != nullptr) {
        Customer *next = temp->get_next();
        delete temp;
        temp = next;
    }
}

Customer* QueueList::get_head() {  return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  if(head == nullptr) head = customer;
  else {
      Customer *iterate = head;
        while (iterate->get_next() != nullptr) {
            iterate = iterate->get_next();
        }
        iterate->set_next(customer);
  }
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  if(head == nullptr) return nullptr;
  Customer* temp = head;
  head = head->get_next();
  return temp;
}

int QueueList::get_items() {
  Customer* iterate = head;
  int sum = 0;
  while(iterate != nullptr){
      sum += iterate->get_numOfItems();
      iterate = iterate->get_next();
  }
  return sum;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  if(head == nullptr) return false;
  Customer* iterate = head;
  while(iterate != nullptr){
      Customer* temp = iterate;
      iterate = iterate->get_next();
      delete temp;
  }
  return true;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
