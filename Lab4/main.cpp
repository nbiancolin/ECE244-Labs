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
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  // You have to make sure all dynamically allocated memory is freed 
  // before return 0
  return 0;
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
    if(mode == "multiple"){ // the one that makes the most fucking sense
        //steps:
        //find register with smallest queue

        //add customer to that register




    } //do we have to check for mode?
    // Depending on the mode of the simulation (single or multiple),
    // add the customer to the single queue or to the register with
    // fewest items
  
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
    delete temp;

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
