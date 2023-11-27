//
//  Rectangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class here

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "Shape.h"
#include "Rectangle.h"

Rectangle::Rectangle(string n, float xcent, float ycent, float len, float wid){
    length = len;
    width = wid;
}

Rectangle::~Rectangle(){
    //nothing since no dynamic allocation
}

float Rectangle::getLength() const{
    return length;
}
float Rectangle::getWidth() const{
    return width;
}

void Rectangle::setLength(float l){
    length = l;
}
void Rectangle::setWidth(float w){
    width = w;
}

void Rectangle::draw() const{
    cout << std::fixed;
    cout << std::setprecision(2); //idk what it is but put it just in case

    cout << "rectangle: " << name << " "
         << x_centre << " " << y_centre
         << " " << length << " " << width  << " " << computeArea()
         << endl;
}

float Rectangle::computeArea() const{
    return length * width;
}

Shape* Rectangle::clone() const{
    return (new Rectangle(*this));
}
