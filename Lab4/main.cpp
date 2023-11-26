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


void printStatistics();

double findEarliestDeparture(Register* &res);

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

  delete registerList;
  delete doneList;
  delete singleQueue;

  return 0;
}

double maxWaitTime(){
    Customer * iterate = doneList->get_head();
    double res = 0;
    double temp;
    while(iterate != nullptr){
        temp = iterate->get_departureTime() - iterate->get_arrivalTime();
        if(temp > res) res = temp;
        iterate = iterate->get_next();
    }
    return res;
}

double avgWaitTime(){
    //count all customers for total
    //sum them all
    Customer* iterate = doneList->get_head();
    double num = 0, denom = 0;
    while(iterate != nullptr){
        ++denom;
        num += iterate->get_departureTime() - iterate->get_arrivalTime();
        iterate = iterate->get_next();
    }
    return (num / denom);
}

double stdDev(double avg){
    Customer* iterate = doneList->get_head();
    double num = 0, denom = 0;
    while(iterate != nullptr){
        num += pow(iterate->get_departureTime() - iterate->get_arrivalTime() - avg, 2);
        ++denom;
        iterate = iterate->get_next();
    }
    return pow((num / denom), 0.5);
}

void printStatistics() {
    cout << "Finished at time ";
    cout << expTimeElapsed << endl;
    cout << "Statistcs:" << endl;
    cout << "Maximum wait time: ";
    cout << maxWaitTime() << endl;
    cout << "Average wait time: ";
    double mean = avgWaitTime();
    cout << mean << endl;
    cout << "Standard Deviation of wait time: ";
    cout << stdDev(mean) << endl;
}

void processCustomers(double time, string &mode, string src) {

    //1. update system time:
    double curTime = expTimeElapsed;
    expTimeElapsed += time;
    if(mode == "single") goto single;
    else if(mode == "multiple") goto multiple;

    //if src is add, have to print "customer entered"


single:
    {
        /**
         * Lets think about this as if it were a real grocery store and go from there
         *
         * in a grocery store, when all the customers are waiting in one line,
         * once a register is finished, it calls the next customer in line
         */
        /**
         * so, in the context of this code
         *
         * we have a max time limit and a "current time"
         * going through all registers, checking for which one will be the first to depart a customer.
         * while that departure time is less than the max time limit, depart that customer, add another to the queue and compute its departure time
         * and continue to check for whichever has the soonest departure time, and that the soonest time is below the current time
         */
        //maxTime == expTimeElapsed
        //current time == curTime

        //for singleQueue, if there is a register with no one in its queue, add someone from the single queue.
        Register* iterate = registerList->get_head();
        while(iterate != nullptr && singleQueue->get_head() != nullptr){
            if(iterate->get_queue_list()->get_head() == nullptr) {
                iterate->get_queue_list()->enqueue(singleQueue->dequeue());
                cout << "Queued a customer with quickest register " << iterate->get_ID() << endl;
            }
            iterate = iterate->get_next();
        }

        iterate = registerList->get_head();
        //int i = 0;
        while(iterate != nullptr){ //calculates all departure times
            iterate->calculateDepartTime(); //checks if there is actually a customer to compute the depaarture time of
            iterate = iterate->get_next();
        }
        //find one with minimum
        curTime = findEarliestDeparture(iterate);
        while(curTime != -1 && curTime <= expTimeElapsed){
            //depart customer, queue next and calc departure time, and continue checking, set curTime to dep time
            curTime = iterate->get_queue_list()->get_head()->get_departureTime();
            iterate->departCustomer(doneList);
            cout << "Departed a customer at register ID " << iterate->get_ID() << " at " << curTime << endl;
            iterate->get_queue_list()->enqueue(singleQueue->dequeue());
            cout << "Queued a customer with quickest register " << iterate->get_ID() << endl;
            iterate->calculateDepartTime();
            curTime = findEarliestDeparture(iterate);//should keep getting rid and queueing people until time is up
        }
        return;
    }



oldSingle: //deprecated
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
    return;
}

multiple: //need to account for if individual queues
    {
        /**
         *  we have a max time limit and a "current time"
         * going through all registers, checking for which one will be the first to depart a customer.
         * while that departure time is less than the max time limit, depart that customer, add another to the queue and compute its departure time
         * and continue to check for whichever has the soonest departure time, and that the soonest time is below the current time
         */
        Register* iterate = registerList->get_head();
        while(iterate != nullptr){ //calculates all the departure times
            iterate->calculateDepartTime();
            iterate = iterate->get_next();
        }

        //go through all registers to check for which will be the first to depart a customer
        double iterateTime = findEarliestDeparture(iterate);
        while(iterateTime < expTimeElapsed && iterateTime > 0){ //the above fn returns -1 if no register is found
            //only triggered if there is a register with someone that departs
            //depart customer (announce it too)
            cout << "Departed a customer at register ID " << iterate->get_ID() << " at " << iterateTime << endl;
            iterate->departCustomer(doneList);
            iterate->calculateDepartTime();
            iterateTime = findEarliestDeparture(iterate);//should keep getting rid and queueing people until time is up
        }





        //Register* iterate = registerList->get_head();
        //int i = 0;
        while(iterate != nullptr){ //calculates all departure times
            iterate->calculateDepartTime();
            iterate = iterate->get_next();
        }
        //find one with minimum
        curTime = findEarliestDeparture(iterate);
        while(curTime != -1 && curTime <= expTimeElapsed){
            //depart customer, queue next and calc departure time, and continue checking, set curTime to dep time
            curTime = iterate->get_queue_list()->get_head()->get_departureTime();
            iterate->departCustomer(doneList);
            //iterate->get_queue_list()->enqueue(singleQueue->dequeue());
            iterate->calculateDepartTime();
            curTime = findEarliestDeparture(iterate);//should keep getting rid and queueing people until time is up
        }
        return;

        return;
    }


oldMultiple:
    {
        //one queue for each register, so a smidge easier
        //2. check all registers
        Register* head = registerList->get_head();
        while(head != nullptr){
            if(head->get_queue_list()->get_head() != nullptr) {
                double headCustomerTime = head->calculateDepartTime();
                //TODO: pick up here
                //calc departure time
                //if the system time is greater than their departure time, depart them and check remaining customers in queue until time runs out
                while (headCustomerTime <= expTimeElapsed) {
                    //dequeue all customers in register until no time left
                    //cout << "Departed a customer at register "
                    head->departCustomer(doneList);

                    //doneList->enqueue(head->get_queue_list()->dequeue());
                    //head->get_queue_list()->enqueue(singleQueue->dequeue()); No need for this since queue list is already updated
                    head->get_queue_list()->get_head()->set_arrivalTime(
                            headCustomerTime);//arrives right when person leaves
                    headCustomerTime = head->calculateDepartTime();
                    //no need to take in account extra stuff, since all synced with system time will be all good
                }
            }
            head = head->get_next(); //loops through all registers
        }
        return;
    }
}


double findEarliestDeparture(Register* &res) {
    Register* iterate = registerList->get_head();
    double temp = -1;
    while(iterate != nullptr){
        if(iterate->get_queue_list()->get_head() != nullptr) {
            double currentDepartureTime = iterate->get_queue_list()->get_head()->get_departureTime();
            if (temp < 0 || currentDepartureTime < temp) {
                res = iterate;
                temp = currentDepartureTime;
            }
        }
        iterate = iterate->get_next();
    }
    return temp;
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

    processCustomers(timeElapsed, mode, "add");
    Customer* next = new Customer(expTimeElapsed, items);
    cout << "A customer entered" << endl; //since the customer arrives after the deadline
    if(mode == "multiple") { // the one that makes the most fucking sense
        //new steps
        //when adding a customer, the customer that is just added is queueud last (maybe just do it manually?)
        //queue to quickest register
        Register *temp = registerList->get_free_register();
        if (temp == nullptr) temp = registerList->get_min_items_register();
        //findEarliestDeparture(temp); //can't actually use this
        temp->get_queue_list()->enqueue(next); //problem here
        cout << "Queued a customer with quickest register " << temp->get_ID() << endl;
        return;
    }
    if(mode == "single"){
        //cout << "A customer entered" << endl;
        singleQueue->enqueue(next);
        //check if customer should get queued in a register
        Register* iterate = registerList->get_head();
        while(iterate != nullptr){
            //find if there is an empty register
            if(iterate->get_queue_list()->get_head() == nullptr) {
                iterate->get_queue_list()->enqueue(next);
                cout << "Queued a customer with quickest register " << iterate->get_ID() << endl;
                return;
            }
            iterate = iterate->get_next();
        }
        singleQueue->enqueue(next);
    }

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
  Register* temp = new Register(ID, secPerItem, setupTime, expTimeElapsed + timeElapsed);
  registerList->enqueue(temp);
  cout << "Opened register " << temp->get_ID() << endl;
  //if(mode == "single") {
  //    //take customer from queue and add them to list
  //    if(temp->get_queue_list() != nullptr && singleQueue->get_head() != nullptr) temp->get_queue_list()->enqueue(singleQueue->dequeue());
  //}
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register
    processCustomers(timeElapsed, mode, "open");
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
    processCustomers(timeElapsed, mode, "close");
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
