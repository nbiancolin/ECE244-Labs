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
    ss >> buffer;
    if(ss.fail()){
        error(2);
        return 0;
    } else return 1;
}

int readIn(stringstream &ss, string &buffer){
    ss >> buffer;
    if(ss.fail()){
        error(2);
        return 0;
    }
    for(int i = 0; i < NUM_KEYWORDS; ++i){
        if(buffer == keyWordsList[i]){
            //error(?)
            return -1;
        }

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

void error(int num, string &val){
    bananas(num, val);
}

void resetShape(int num){
    Shape *modify = shapesArray[num];

    modify->setName()
}




string commandParser(stringtream &ss){ //function to read the command name
    string cmd;
    ss >> cmd;
    if(ss.fail()) {
        ss.clear();
        error(1);
        return "oops"; //error case to be
    }
    return cmd;
}

void maxShapes(stringstream &ss){ //TODO probably fix this
    int size;
    ss >> size;
    if(ss.fail()){
        error(2);
        ss.clear();
        return;
    }


    //free all of shapesArray
    if(**shapesArray != NULL){
        //free array
        for(int i = 0; i < max_shapes; i++){
            delete shapesArray[i];
        }
        delete [] shapesArray;
        index = 0;
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
    string name, type;
    int xloc, yloc, xsz, ysz;
    int flag;

    if(shapeCount >= max_shapes){
        error(10);
        return;
    }

    readIn(ss, name);
    if(ss.fail()) goto exit;
    Shape *create = *shapesArray[index];
    ++index;

    readIn(ss, type);
    if(ss.fail()) goto exit;

    readIn(ss, &xloc);
    if(ss.fail()) goto exit;

    readIn(ss, &yloc);
    if(ss.fail()) goto exit;

    readIn(ss, &xsz);
    if(ss.fail()) goto exit;

    readIn(ss, &ysz);
    if(ss.fail()) goto exit;

    if(ss.peek() == EOF){
        error(8);
        return; //TODO: should clear entry or no?
    }

    return;
exit:
    {
        error(9);
        //reset all values
        //return
    }

}




void move(stringstream &ss){
    string buffer;
    int buf;

    ss >> buffer;
    if(ss.fail()) {
        error(2);
        return;
    }
    //find shape in 'database'





    //update location

    ss >> buf;

}



/**
 * https://cplusplus.com/reference/sstream/stringstream/
 * ^^ whatever this weird stringstream is, there thankfully exists documentation for it
 */


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

        string cmd = commandParser(lineStream);
        if(cmd != "oops") {
            //do command stuff
            switch(cmd){
                case "maxShapes": maxShapes(linestream); //TODO: recap how to pass by reference
                case "create": create(linestream);
                case "move": move(linestream);
                case "rotate": rotate(linestream);
                default: error(1);
            }






        }



        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        
    }  // End input loop until EOF.
    
    return 0;
}

