//
//  Triangle.h
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

#ifndef Triangle_h
#define Triangle_h

#include <iostream>
#include <string>
using namespace std;


// This class implements the triangle Shape objects.
// ECE244 Student: Write the definition of the class here.

#include "Shape.h";


class Triangle : public Shape {
private:
    float base;
    float height;

public:
    Triangle(string n, float xcent, float ycent, float b, float h);

    virtual ~Triangle();

    float getBase() const;
    float getHeight() const;

    void setBase(float b);
    void setHeight(float h);

    virtual void draw() const;

    virtual float computeArea() const;

    virtual Shape* clone() const;
};

#endif /* Triangle_h */


