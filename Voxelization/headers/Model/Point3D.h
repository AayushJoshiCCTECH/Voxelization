#pragma once
#include<iostream>

class Point3D
{
public:
	// Constructors
	Point3D(); // Default constructor
	Point3D(double inX, double inY, double inZ); // Constructor with coordinates
	Point3D(char* facet); // Constructor from facet data
	~Point3D(); // Destructor

	// Getter functions for coordinates
	double x() const;
	double y() const;
	double z() const;

	// Setter functions for coordinates
	void setX(double inX);
	void setY(double inY);
	void setZ(double inZ);

	// Overloaded operators for vector arithmetic
	const Point3D operator+(const Point3D& rv) const; // Addition
	const Point3D operator-(const Point3D& rv) const; // Subtraction
	const Point3D operator*(const double& rv) const; // Scalar multiplication
	const Point3D operator/(const double& rv) const; // Scalar division
	const Point3D operator^(const Point3D& rv) const; // Cross product
	const double operator&(const Point3D& rv) const; // Dot product
	Point3D& operator+=(const Point3D& rv); // Compound addition
	Point3D& operator-=(const Point3D& rv); // Compound subtraction
	Point3D& operator*=(const double& rv); // Compound multiplication
	Point3D& operator/=(const double& rv); // Compound division

	// Comparison operator
	bool operator==(const Point3D& rv) const;

	// Calculate the norm (length) of the vector
	double normal() const;

	Point3D cross(const Point3D& other) const;

	double dot(const Point3D& other) const;

	void normalize();

private:
	double mX; // x-coordinate
	double mY; // y-coordinate
	double mZ; // z-coordinate
};

