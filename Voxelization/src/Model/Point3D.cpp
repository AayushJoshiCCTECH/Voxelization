#include "Model/Point3D.h"

// Default constructor
Point3D::Point3D() : mX(0.0), mY(0.0), mZ(0.0)
{
}

// Constructor with parameters
Point3D::Point3D(double inX, double inY, double inZ) : mX(inX), mY(inY), mZ(inZ) {}

// Constructor from character array
Point3D::Point3D(char* facet) {
    char f1[4] = { facet[0], facet[1], facet[2], facet[3] };
    char f2[4] = { facet[4], facet[5], facet[6], facet[7] };
    char f3[4] = { facet[8], facet[9], facet[10], facet[11] };
    float xx = *((float*)f1);
    float yy = *((float*)f2);
    float zz = *((float*)f3);
    mX = double(xx);
    mY = double(yy);
    mZ = double(zz);
}



// Destructor
Point3D::~Point3D()
{
}

// Getter for X coordinate
double Point3D::x() const
{
    return mX;
}

// Getter for Y coordinate
double Point3D::y() const
{
    return mY;
}

// Getter for Z coordinate
double Point3D::z() const
{
    return mZ;
}

// Setter for X coordinate
void Point3D::setX(double inX)
{
    mX = inX;
}

// Setter for Y coordinate
void Point3D::setY(double inY)
{
    mY = inY;
}

// Setter for Z coordinate
void Point3D::setZ(double inZ)
{
    mZ = inZ;
}

// Addition operator overloading
const Point3D Point3D::operator+(const Point3D& rv) const
{
    return Point3D(mX + rv.x(), mY + rv.y(), mZ + rv.z());
}

// Subtraction operator overloading
const Point3D Point3D::operator-(const Point3D& rv) const
{
    return Point3D(mX - rv.x(), mY - rv.y(), mZ - rv.z());
}

// Scalar multiplication operator overloading
const Point3D Point3D::operator*(const double& rv) const
{
    return Point3D(mX * rv, mY * rv, mZ * rv);
}

// Scalar division operator overloading
const Point3D Point3D::operator/(const double& rv) const
{
    return Point3D(mX / rv, mY / rv, mZ / rv);
}

// Cross product operator overloading
const Point3D Point3D::operator^(const Point3D& rv) const
{
    return Point3D(
        mY * rv.z() - mZ * rv.y(),
        mZ * rv.x() - mX * rv.z(),
        mX * rv.y() - mY * rv.x());
}

// Dot product operator overloading
const double Point3D::operator&(const Point3D& rv) const
{
    return mX * rv.x() + mY * rv.y() + mZ * rv.z();
}

// Compound addition assignment operator overloading
Point3D& Point3D::operator+=(const Point3D& rv)
{
    mX += rv.x();
    mY += rv.y();
    mZ += rv.z();
    return *this;
}

// Compound subtraction assignment operator overloading
Point3D& Point3D::operator-=(const Point3D& rv)
{
    mX -= rv.x();
    mY -= rv.y();
    mZ -= rv.z();
    return *this;
}

// Compound multiplication assignment operator overloading
Point3D& Point3D::operator*=(const double& rv)
{
    mX *= rv;
    mY *= rv;
    mZ *= rv;
    return *this;
}

// Compound division assignment operator overloading
Point3D& Point3D::operator/=(const double& rv)
{
    mX /= rv;
    mY /= rv;
    mZ /= rv;
    return *this;
}

// Equality comparison operator overloading
bool Point3D::operator==(const Point3D& rv) const {
    return (mX == rv.mX) && (mY == rv.mY) && (mZ == rv.mZ);
}

// Method to calculate the Euclidean norm of the vector represented by the point
double Point3D::normal() const
{
    return sqrt(mX * mX + mY * mY + mZ * mZ);
}

// Cross product of two vectors
Point3D Point3D::cross(const Point3D& other) const {
    return *this ^ other;
}

// Dot product of two vectors
double Point3D::dot(const Point3D& other) const {
    return *this & other;
}
// Method to normalize the vector represented by the point
void Point3D::normalize()
{
    // Calculate the length of the vector
    double length = normal();

    // Check if the length is not zero to avoid division by zero
    if (length != 0) {
        // Normalize the vector by dividing each component by the length
        mX /= length;
        mY /= length;
        mZ /= length;
    }
}