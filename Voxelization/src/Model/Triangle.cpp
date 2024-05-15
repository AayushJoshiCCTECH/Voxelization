#include "Model/Triangle.h"

// Default constructor
Triangle::Triangle() : mP1(Point3D(0, 0, 0)), mP2(Point3D(0, 0, 0)), mP3(Point3D(0, 0, 0)), mNormal(Point3D(0, 0, 0))
{
}

// Constructor with three vertices
Triangle::Triangle(Point3D inP1, Point3D inP2, Point3D inP3) : mP1(Point3D(0, 0, 0)), mP2(Point3D(0, 0, 0)), mP3(Point3D(0, 0, 0)), mNormal(Point3D(0, 0, 0))
{
    // Assign vertices
    mP1 = inP1;
    mP2 = inP2;
    mP3 = inP3;
}

// Constructor with normal and three vertices
Triangle::Triangle(Point3D inNormal, Point3D inP1, Point3D inP2, Point3D inP3) :
    mNormal(inNormal), mP1(inP1), mP2(inP2), mP3(inP3)
{
}

// Destructor
Triangle::~Triangle()
{

}

// Getter method for vertex 1
Point3D Triangle::p1() const
{
    return mP1;
}

// Getter method for vertex 2
Point3D Triangle::p2() const
{
    return mP2;
}

// Getter method for vertex 3

Point3D Triangle::p3() const
{
    return mP3;
}

// Getter method for normal
Point3D Triangle::normal() const
{
    return mNormal;
}

