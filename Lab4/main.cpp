#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

void processCustomers();

void printStatistics();

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    //processCustomers(); -- Called in individual functions
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  // You have to make sure all dynamically allocated memory is freed 
  // before return 0

  //do stats
  printStatistics();

  //TODO free all dynamic memory

  return 0;
}

void printStatistics() {
    cout << "Finished at time ";
    cout << expTimeElapsed << endl;
    cout << "Statistcs:" << endl;
    cout << "Maximum wait time: ";
    //cout << maxWaitTime << endl;
    return;
}

void processCustomers(double time, string mode) {
    //update system time
    /**
     * Every time a command is entered, apart from adding a customer, opening or closing a register, the system time
     * has to be updated.
     * The system time starts from 0, and it gets incremented with the elapsed time every command is entered.
     * Along with the update in time, each register has to be checked.
     *
     * If the time taken to process the customer at the head of the queue of a register has passed, the customer has to depart.
     * The time taken to process a customer is secPerItem of the register × items of the customer + setupTime of the register.
     * If the customer at the head of the queue arrived after the register became available, then the departure time is the
     * processing time of the customer plus the arrival time of the customer.
     * Otherwise, the departure time is the processing time of the customer plus the time the register became available.
     *
     * Once it is time for a customer to depart, its departure time is updated, and the customer should leave the queue and
     * get added to a linked list of customers that exited the system.
     * This linked list would be of help when we calculate some statistics about the wait time of the simulation.
     *
     * If the simulation mode is “single”, as soon as a register departs a customer, another customer from the single queue
     * should join this free register.
     * Hint: We have to depart customers in order of their departure times, NOT by the order of registers in the linked list,
     * to ensure customers get served by the correct registers.
     * Also, the elapsed time can be enough to depart more than one customer, so it is important to check that we have departed
     * enough customers from each register.
     */

    //1. update system time:
    expTimeElapsed += time;
    double internalTime = time;
    if(mode == "single") goto single;
    else if(mode == "multiple") goto multiple;

single:
{
    //2. check all registers
    Register* head = registerList->get_head();
    while(head != nullptr){  //TODO: Maybe need a better way to go through registers
        double headCustomerTime = head->calculateDepartTime();
        //TODO: pick up here
        //calc departure time
        //if the system time is greater than their departure time, depart them and check remaining customers in queue until time runs out
        while(headCustomerTime <= expTimeElapsed){
            //dequeue all customers in register until no time left
            head->departCustomer(doneList);
            //doneList->enqueue(head->get_queue_list()->dequeue());
            head->get_queue_list()->enqueue(singleQueue->dequeue());
            head->get_queue_list()->get_head()->set_arrivalTime(headCustomerTime);//arrives right when person leaves
            headCustomerTime = head->calculateDepartTime();
            //no need to take in account extra stuff, since all synced with system time will be all good
        }
        head = head->get_next(); //loops through all registers
    }
    //a. check if time has elapsed for customer to depart
    //b if so,
}
multiple:
{
    //one queue for each register, so a smidge easier
    //2. check all registers
    Register* head = registerList->get_head();
    while(head != nullptr){
        double headCustomerTime = head->calculateDepartTime();
        //TODO: pick up here
        //calc departure time
        //if the system time is greater than their departure time, depart them and check remaining customers in queue until time runs out
        while(headCustomerTime <= expTimeElapsed){
            //dequeue all customers in register until no time left
            head->departCustomer(doneList);
            //doneList->enqueue(head->get_queue_list()->dequeue());
            //head->get_queue_list()->enqueue(singleQueue->dequeue()); No need for this since queue list is already updated
            head->get_queue_list()->get_head()->set_arrivalTime(headCustomerTime);//arrives right when person leaves
            headCustomerTime = head->calculateDepartTime();
            //no need to take in account extra stuff, since all synced with system time will be all good
        }
        head = head->get_next(); //loops through all registers
    }
}
}




string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
    int items;
    double timeElapsed;
    if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
        cout << "Error: too few arguments." << endl;
        return;
    }
    if (foundMoreArgs(lineStream)) {
        cout << "Error: too many arguments." << endl;
        return;
    }
    Customer* next = new Customer(timeElapsed, items);
    if(mode == "multiple"){ // the one that makes the most fucking sense
        //steps:
        //find register with smallest queue
        Register* res = registerList->get_free_register();
        if(res == nullptr) res = registerList->get_min_items_register(); //if no free registers, find the register with the minimum number of items
        res->get_queue_list()->enqueue(next); // adds customer to register

    } else if(mode == "single"){
        //in this case, add customer to queue list
        if(registerList->get_head()->get_queue_list()->get_head() == nullptr){
            registerList->get_head()->get_queue_list()->enqueue(next);
        } else{
            singleQueue->enqueue(next);
        }
    }//do we have to check for mode?
    // Depending on the mode of the simulation (single or multiple),
    // add the customer to the single queue or to the register with
    // fewest items
    processCustomers(timeElapsed, mode);
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  if(registerList->foundRegister(ID)){
      cout << "Error: register " << ID << " is already open." << endl;
      return;
  }
  // If it's open, print an error message
  // Otherwise, open the register
  Register* temp = new Register(ID, secPerItem, setupTime, timeElapsed);
  registerList->enqueue(temp);
  if(mode == "single") {
      //take customer from queue and add them to list
      temp->get_queue_list()->enqueue(singleQueue->dequeue());
  }
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register
  processCustomers(timeElapsed, mode);
}

void closeRegister(stringstream &lineStream, string mode) {
    int ID;
    double timeElapsed;
    // convert string to int
    if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
        cout << "Error: too few arguments." << endl;
        return;
    }
    if (foundMoreArgs(lineStream)) {
        cout << "Error: too many arguments" << endl;
        return;
    }
    if(!registerList->foundRegister(ID)){ //checks if register is not open
        // error message
        cout << "Error: register " << ID << " is not open" << endl;
        return;
    }
    Register* temp = registerList->get_head();
    Register* prev = nullptr;
    while(temp != nullptr){
        if(temp->get_ID() == ID) break;
        prev = temp;
        temp = temp->get_next();
    }
    //dequeue temp
    prev->set_next(temp->get_next());
    delete temp;  //TODO: check if customers are in queue and move them around
    processCustomers(timeElapsed, mode)
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}
