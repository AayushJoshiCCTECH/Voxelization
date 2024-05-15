#pragma once

#include "Model/Point3D.h" // Including header file for Point3D class

class Triangle
{
public:
	// Constructors
	Triangle(); // Default constructor
	Triangle(Point3D inP1, Point3D inP2, Point3D inP3); // Constructor with three points
	Triangle(Point3D inNormal, Point3D inP1, Point3D inP2, Point3D inP3); // Constructor with normal and three points
	~Triangle(); // Destructor

	// Getter functions for points
	Point3D p1() const;
	Point3D p2() const;
	Point3D p3() const;

	// Getter function for normal
	Point3D normal() const;

private:
	Point3D mP1; // First point of the triangle
	Point3D mP2; // Second point of the triangle
	Point3D mP3; // Third point of the triangle
	Point3D mNormal; // Normal vector of the triangle
};

