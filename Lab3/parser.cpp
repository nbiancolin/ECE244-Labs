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
int max_shapes;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here

int readIn(stringstream &ss, int *buffer){ //we love polymorphism
    if(ss.str().empty()) {
        error(9);
        return 0;
    }
    ss >> buffer;
    if(ss.fail()){
        error(2);
        return 0;
    } else return 1;
}

int readIn(stringstream &ss, string &buffer){
    if(ss.str().empty()) {
        error(9);
        return 0;
    }
    ss >> buffer;
    if(ss.fail()){
        error(2);
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


void bananas(int num, string &val){ //this is the method I came up with for "function overloading". Yes, its bananas
    cout << "Error: "
    switch(num){
        case 1: cout << "invalid command";
        case 2: cout << "invalid argument";
        case 3: cout << "invalid shape name";
        case 4: cout << "shape "<< val << " exists";
        case 5: cout << "shape "<< val << " not found";
        case 6: cout << "invalid shape type";
        case 7: cout << "invalid value";
        case 8: cout << "too many arguments";
        case 9: cout << "too few arguments";
        case 10: cout << "shape array is full";
        default: cout << "programmer made an oopsie";
    }
}

void error(int num){
    bananas(num, "foo");
}

void error(int num, string &val){ //polymorphism >>
    bananas(num, val);
}

int shapeExists(string name){
    for(int i = 0; i < shapeCount; i++){
        if (name == shapesArray[i]->getName()) return i;
    }
    return -1;
}

void maxShapes(stringstream &ss){ //TODO probably fix this
    int size;
    ss >> size;
    if(ss.fail()){
        error(2);
        ss.clear();
        return;
    }


    /*
    if(**shapesArray != NULL){
        //free array
        for(int i = 0; i < max_shapes; i++){
            delete shapesArray[i];
        }
        delete [] shapesArray;
        shapeCount = 0;
    }*/

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
    *shapesArray = new Shape[size];

    max_shapes = size;

    //for(int i = 0; i < size; i++){
    //    shapesArray[i] = new Shape();
    //}


    cout << "New database: max shapes is " << size << endl;
    return;
}

void create(stringstream &ss){
    string name;
    string type;
    int xloc, yloc, xsz, ysz;
    int flag;

    Shape *create = *shapesArray[shapeCount];

    readIn(ss, name);  //read in name and error check
    if(ss.fail()) return;
    for(int i = 0; i < NUM_KEYWORDS; ++i){
        if(name == keyWordsList[i]) goto invShape;
    }
    if(shapeExists(name) >=0){
        error(4, name);
        return;
    }

    readIn(ss, type); //same for type
    if(ss.fail()) goto exit;
    for(int i = 0; i < NUM_KEYWORDS; ++i){
        if(type == keyWordsList[i]) goto invShape;
    }
    for(int i = 0; i < NUM_TYPES; ++i){
        if(type == shapeTypesList[i]) goto invType;
    }

    readIn(ss, &xloc);
    if(ss.fail()) goto exit;
    if(xloc < 0){
        error(7);
        return;
    }

    readIn(ss, &yloc);
    if(ss.fail()) goto exit;
    if(yloc < 0){
        error(7);
        return;
    }

    readIn(ss, &xsz);
    if(ss.fail()) goto exit;
    if(xsz < 0){
        error(7);
        return;
    }

    readIn(ss, &ysz);
    if(ss.fail()) goto exit;
    if(ysz < 0){
        error(7);
        return;
    }

    if(!ss.str().empty()){ //checks if there are more arguments in the stringstream
        error(8);
        return;
    }

    if(shapeCount >= max_shapes){ //final check to ensure there is space in the array
        error(10);
        return;
    }


    if(type == "circle"){ //check for circle being equal
        if(x != y){
            error(7);
            return;
        }
    }


    ++shapeCount;
    *create = new Shape(name, type, xloc, xsz, yloc, ysz);

    cout << "created ";
    create->draw();

    return;

exit: return;

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

}

void move(stringstream &ss){
    string name;
    int x, y, temp;

    temp = readIn(ss, name) == 1;
    if(temp == 1) {
        int loc = shapeExists(name);
        if (loc == -1) {
            error(5);
            return;
        }else {
            Shape *modify = *shapesArray[loc];
            temp = readIn(ss, &x);
            if (temp == 0) return;
            temp = readIn(ss, &y);
            if (temp == 0) return;
            if (x >= 0 * *y >= 0) {
                modify->setXlocation(x);
                modify->setYlocation(y);
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
        int loc = shapeExists(name);
        if(loc == -1) {
            error(5);
            return;
        } else {
            Shape *modify = *shapesArray[loc];
            temp = readIn(ss, &angle);
            if (temp == 0) return;
            if(angle >= 0 && angle <= 360){
                modify->setRotate(angle);
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
    if (temp == 0) return 0;
    if(name != "all"){
        int loc = shapeExists(name);
        if(loc == -1) {
            error(5);
            return;
        } else{
            Shape *sel = *shapesArray[loc];
            cout << "Drew " << sel->draw();
            return;
        }
    } else {
        cout << "Drew all shaapes" << endl; //TODO: should this be after all shapes are drawn?
        for(int i = 0; i < shapeCount; i++){
            *shapesArray[i]->draw();
        }
        return;
    }
}

void del(stringstream &ss){
    string name;

    int temp = readIn(ss, name);
    if (temp == 0) return 0;
    if(name != "all"){
        int loc = shapeExists(name);
        if(loc == -1){
            error(5);
            return;
        } else {
            Shape *sel = *shapesArray[loc];
            delete sel;
            cout << "Deleted shape" << name << endl;
        }
    } else {

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

        if(lineStream.str() == EOF) break;
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
