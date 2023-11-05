//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes = 0;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here

void bananas(int num, string &val){ //this is the method I came up with for "function overloading". Yes, its bananas
    cout << "Error: ";
    switch(num){
        case 1: cout << "invalid command";
        break;
        case 2: cout << "invalid argument";
        break;
        case 3: cout << "invalid shape name";
        break;
        case 4: cout << "shape "<< val << " exists";
        break;
        case 5: cout << "shape "<< val << " not found";
        break;
        case 6: cout << "invalid shape type";
        break;
        case 7: cout << "invalid value";
        break;
        case 8: cout << "too many arguments";
        break;
        case 9: cout << "too few arguments";
        break;
        case 10: cout << "shape array is full";
        break;
        default: cout << "programmer made an oopsie";
    }
    cout << endl;
}

void error(int num){
    bananas(num, (string &) "foo");
}

void error(int num, string &val){ //polymorphism >>
    bananas(num, val);
}



int readIn(stringstream &ss, int &buffer){
    if(ss.eof()) { //checks for too few arguements
        error(9);
        return 0;
    }
    string temp;
    ss >> temp;

    for(char c : temp){
        if(!isdigit(c) && c != '-'){
            error(2);
            return 0;
        }
    }
    buffer = stoi(temp);
    return 1;

}

int readIn(stringstream &ss, string &buffer){ //we love polymorphism
    if(ss.peek() == EOF) { //checks for too few arguments
        error(9);
        return 0;
    }
    ss >> buffer;
    if(ss.fail()){
        error(2); //checks if somehow this fails, idk how but like whatever
        return 0;
    } else return 1;
}

int readCmd(stringstream &ss, string &buffer){ //specifically for reading in commands so it throws the right error
    if(ss.str().empty()){
        error(1);
        return 0;
    }
    ss >> buffer;
    if(ss.fail()){
        error(1);
        return 0;
    }
    return 1;
}

int locShape(string &name){
    if (shapesArray == nullptr) {
        //cout << "null arr" << endl; //we love it when the debugger doesnt work
        return -1; // or some appropriate error handling
    }
    for(int i = 0; i < shapeCount; ++i){
        //cout << shapesArray[i]->getName() << endl;
        if (shapesArray[i] != nullptr)
            if(name == shapesArray[i]->getName()) return i;
    }
    return -1;
}



void maxShapes(stringstream &ss){  //works from initial testing!
    int size;
    int temp;

    temp = readIn(ss, size);
    if(temp == 0) return;
    if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
        error(8);
        return;
    }

    //free all of shapesArray
    if (shapesArray != nullptr) {
        // Free each object in the array
        for (int i = 0; i < shapeCount; i++) {
            if (shapesArray[i] != nullptr) {
                delete shapesArray[i];
            }
        }

        // Now, delete the array itself
        delete[] shapesArray;

        // Reset the pointer and count
        shapesArray = nullptr;
        shapeCount = 0;
    }


    //proceed with dynamically allocating memory;
    shapesArray = new Shape*[size];

    max_shapes = size;


    cout << "New database: max shapes is " << size << endl;
    return;
}

void create(stringstream &ss){
    string name, type;
    int xloc, yloc, xsz, ysz;

    int temp = 0;

    temp = readIn(ss, name);
    if(!temp) return;
    //error checking for name
    for(int i = 0; i < NUM_KEYWORDS; ++i){
        if(name == keyWordsList[i]){
            error(3);
            return;
        }
    }
    //cout << 1;
    for(int i = 0; i < NUM_TYPES; ++i){
        if(name == shapeTypesList[i]) {
            error(3);
            return;
        }
    }


    temp = readIn(ss, type);
    if(!temp) return;
    for(int i = 0; i < NUM_KEYWORDS; ++i){
        if(type == keyWordsList[i]) {
            error(6);
            return;
        }
    } //error checking for type
    for(int i = 0; i < NUM_TYPES; ++i){
        if(type == shapeTypesList[i]) break;
        if(i == NUM_TYPES-1){
            error(6);
            return;
        }
    }

    if(locShape(name) != -1){
        error(4, name);
        return;
    }

    temp = readIn(ss, xloc);
    if(!temp) return;
    if(xloc < 0){
        error(7);
        return;
    }

    temp = readIn(ss, yloc);
    if(!temp) return;
    if(yloc < 0){
        error(7);
        return;
    }

    temp = readIn(ss, xsz);
    if(!temp) return;
    if(xsz < 0){
        error(7);
        return;
    }

    temp = readIn(ss, ysz);
    if(!temp) return;
    if(ysz < 0){
        error(7);
        return;
    }

    if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
        error(8);
        return;
    }
    //cout << "empty check evaluated" << endl;


    if(shapeCount >= max_shapes){ //final check to ensure there is space in the array
        error(10);
        return;
    }


    if(type == "circle"){ //check for circle being equal
        if(xsz != ysz){
            error(7);
            return;
        }
    }


    Shape* create = new Shape(name, type, xloc, xsz, yloc, ysz);
    shapesArray[shapeCount] = create;
    ++shapeCount;

    cout << "Created ";
    create->draw();

}


/* DEPRECATED
void create(stringstream &ss){ //deprecated
    string name;
    string type;
    int xloc, yloc, xsz, ysz;

    Shape *create = shapesArray[shapeCount]; //whoops this shit didnt work mb homies

    //cout << "shape created" << endl;
    readIn(ss, name);  //read in name and error check
    if(ss.fail()) return;

    //cout << "first value read in" << endl;

    for(int i = 0; i < NUM_KEYWORDS; ++i){
        if(name == keyWordsList[i]) goto invShape;
    }
    //cout << 1;
    for(int i = 0; i < NUM_TYPES; ++i){
        if(name == shapeTypesList[i]) goto invShape;
    }
    //cout << 2;
    if(locShape(name) != -1){
        error(4, name);
        return;
    }
    //cout << 3 << endl;
    //cout << "first value checks ok" << endl;

    if(ss.peek() == EOF) goto noMas;
    readIn(ss, type); //same for type
    if(ss.fail()) goto exit;
    for(int i = 0; i < NUM_KEYWORDS; ++i){
        if(type == keyWordsList[i]) goto invShape;
    }
    for(int i = 0; i < NUM_TYPES; ++i){
        if(type == shapeTypesList[i]) break;
        if(i == NUM_TYPES-1) goto invType;
    }
    //cout << "second value read in" << endl;

    if(ss.peek() == EOF) goto noMas;
    readIn(ss, xloc);
    if(ss.fail()) goto exit;
    if(xloc < 0){
        error(7);
        return;
    }
    //cout << "third value read in" << endl;

    if(ss.peek() == EOF) goto noMas;
    readIn(ss, yloc);
    if(ss.fail()) goto exit;
    if(yloc < 0){
        error(7);
        return;
    }
    //cout << "fourth value read in" << endl;

    if(ss.peek() == EOF) goto noMas;
    readIn(ss, xsz);
    if(ss.fail()) goto exit;
    if(xsz < 0){
        error(7);
        return;
    }
    //cout << "fifth value read in" << endl;

    if(ss.peek() == EOF) goto noMas;
    readIn(ss, ysz);
    if(ss.fail()) goto exit;
    if(ysz < 0){
        error(7);
        return;
    }
    //cout << "6th value read in" << endl;

    if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
        error(8);
        return;
    }
    //cout << "empty check evaluated" << endl;

    if(shapeCount >= max_shapes){ //final check to ensure there is space in the array
                                    //TODO: Figure out why this doesnt work
        //cout << ss.str() << endl;
        error(10);
        return;
    }
    //cout << "confirmed space in array" << endl;


    if(type == "circle"){ //check for circle being equal
        if(xsz != ysz){
            error(7);
            return;
        }
    }
    //cout << "cicle check" << endl;



    create = new Shape(name, type, xloc, xsz, yloc, ysz);
    //shapesArray[shapeCount] = new Shape(name, type, xloc, xsz, yloc, ysz);
    //++shapeCount;
    shapesArray[shapeCount] = create;

    ++shapeCount;

    cout << "Created ";

    create->draw();

    //shapesArray[shapeCount-1]->draw();

    return;

exit: return; //this was something at some point and now it's not and I don't want to break everything so it's staying in

noMas:
    {
        error(9);
        return;
    };


invShape:
    {
        error(3);
        return;
    }
invType:
    {
        error(6);
        return;
    }

}*/


void move(stringstream &ss){
    string name;
    int x, y, temp;

    temp = readIn(ss, name);
    if(temp == 1) {
        int loc = locShape(name);
        if (loc == -1) {
            error(5, name);
            return;
        }else {
            temp = readIn(ss, x);
            if (temp == 0) return;
            temp = readIn(ss, y);
            if (temp == 0) return;
            if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
                error(8);
                return;
            }
            if (x >= 0 && y >= 0) {
                shapesArray[loc]->setXlocation(x);
                shapesArray[loc]->setYlocation(y);
                cout << "Moved " << name << " to " << x << " " << y << endl;
                return;
            } else {
                error(7);
                return;
            }
        }
    } else return;
}

void rotate(stringstream &ss){
    string name;
    int angle, temp;

    temp = readIn(ss, name);
    if(temp == 1){
        int loc = locShape(name);
        if(loc == -1) {
            error(5, name);
            return;
        } else {
            temp = readIn(ss, angle);
            if (temp == 0) return;
            if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
                error(8);
                return;
            }
            if(angle >= 0 && angle <= 360){
                shapesArray[loc]->setRotate(angle);
                cout << "Rotated " << name << " by " << angle << " degrees" << endl;
                return;
            } else{
                error(7);
                return;
            }
        }
    } else return;
}

void draw(stringstream &ss){
    string name;

    int temp = readIn(ss, name);
    if (temp == 0) return;
    if(name != "all"){
        int loc = locShape(name);
        if(loc == -1) {
            error(5, name);
            return;
        } else{
            if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
                error(8);
                return;
            }
            cout << "Drew ";
            shapesArray[loc]->draw();
            return;
        }
    } else {
        cout << "Drew all shapes" << endl; //TODO: should this be after all shapes are drawn?
        for(int i = 0; i < shapeCount; i++){
            if(shapesArray[i] != nullptr) shapesArray[i]->draw();
        }
        return;
    }
}

void del(stringstream &ss){
    string name;

    int temp = readIn(ss, name);
    if (temp == 0) return;
    if(name != "all"){
        int loc = locShape(name);
        if(loc == -1){
            error(5, name);
            return;
        } else {
            if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
                error(8);
                return;
            }
            //Shape *sel = shapesArray[loc]; I forgor how pointers work
            delete shapesArray[loc]; //TODO: doesnt work
            shapesArray[loc] = nullptr;
            cout << "Deleted shape " << name << endl;
        }
    } else {
        if(ss.peek() != EOF){ //checks if there are more arguments in the stringstream
            error(8);
            return;
        }
        for (int i = 0; i < shapeCount; i++) {
            if (shapesArray[i] != nullptr) {
                delete shapesArray[i];
            }
        }
        shapeCount = 0;
        cout << "Deleted: all shapes" << endl;
    }
    return;
}

int main() {

    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while ( !cin.eof () ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        //lineStream >> command;

        //if(lineStream.str() == EOF) break;  I think this is redundant but who knows
        int temp = readCmd(lineStream, command);
        if(temp == 1) {
            if(command == "maxShapes") maxShapes(lineStream);
            else if(command == "create") create(lineStream);
            else if(command == "move") move(lineStream);
            else if(command == "rotate") rotate(lineStream);
            else if(command == "draw") draw(lineStream);
            else if(command == "delete") del(lineStream);
            else error(1);
        }
        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        
    }  // End input loop until EOF.
    
    return 0;
}
