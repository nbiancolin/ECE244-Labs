//
//  Triangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Triangle here

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "Shape.h"
#include "Triangle.h"

Triangle::Triangle(string n, float xcent, float ycent, float b, float h){
    base = b;
    height = h;
}

Triangle::~Triangle(){
    //nothing since no dynamic allocation
}

float Triangle::getLength() const{
    return base;
}
float Triangle::getWidth() const{
    return height;
}

void Triangle::setBase(float b){
    base = b;
}
void Triangle::setHeight(float h){
    height = h;
}

void Triangle::draw() const{
    cout << std::fixed;
    cout << std::setprecision(2); //idk what it is but put it just in case

    cout << "triangle: " << name << " "
         << x_centre << " " << y_centre
         << " " << length << " " << width  << " " << computeArea()
         << endl;
}

float Triangle::computeArea() const{
    return length * width;
}

Shape* Triangle::clone() const{
    return (new Triangle(*this));
}
