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

Triangle::Triangle(string n, float xcent, float ycent, float x_1, float y_1, float x_2, float y_2, float x_3, float y_3){
    x1 = x_1;
    y1 = y_1;
    x2 = x_2;
    y2 = y_2;
    x3 = x_3;
    y3 = y_3;
}

Triangle::~Triangle(){
    //nothing since no dynamic allocation
}

/*
float Triangle::getLength() const{ //fix
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
}*/

void Triangle::draw() const{
    cout << std::fixed;
    cout << std::setprecision(2); //idk what it is but put it just in case

    cout << "triangle: " << name << " "
         << x_centre << " " << y_centre
         << " " << x1 << " " << y1
        << " " << x2 << " " << y2
        << " " << x3 << " " << y3
        << " " << computeArea()
         << endl;
}

float Triangle::computeArea() const{
    return 0.5 * (x1 (y2-y3) + x2(y3-y1) + x3(y1-y2));
}

Shape* Triangle::clone() const{
    return (new Triangle(*this));
}
