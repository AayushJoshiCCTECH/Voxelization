#pragma once
#include <vector>
#include "Model/Point3D.h" // Including header file for Point3D class

class GeomContainer {
public:
	// Static function to get an instance of GeomContainer
	static GeomContainer* getContainer();

	// Static function to get an instance of GeomContainer with initial points
	static GeomContainer* getContainer(std::vector<Point3D> inPoints);

	// Function to create vertices for triangles
	void makeTriangleVertices();

	// Function to create vertices for quads (four-sided polygons)
	void makeQuadVertices(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4);

	// Function to return vertices of the geometry
	std::vector<float> vertices();

	// Function to return colors of the geometry
	std::vector<float> colors();

	// Function to return normals of the geometry
	std::vector<float> normals();

private:
	// Private default constructor
	GeomContainer();

	// Private constructor with initial points
	GeomContainer(std::vector<Point3D> inPoints);

private:
	std::vector<Point3D> mPoints; // Vector to store points
	std::vector<float> mVertices; // Vector to store vertices
	std::vector<float> mColors; // Vector to store colors
	std::vector<float> mNormals; // Vector to store normals

	Point3D mColor; // Color object
};
