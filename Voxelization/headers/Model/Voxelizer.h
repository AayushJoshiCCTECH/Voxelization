#pragma once
#include <string>
#include <vector>
#include "Model/Point3D.h" // Including header file for Point3D class

class Voxelizer
{
public:
	// Static function to get an instance of Voxelizer
	static Voxelizer* getVoxelizer(std::string fileName, int voxelSize);

	// Function to return vertices of created cubes
	std::vector<float> vertices() const;

	// Function to return colors of created cubes
	std::vector<float> colors() const;

	std::vector<float> normals() const;

	void createBoundingBoxGrid(const std::vector<Point3D>& vertices);

	bool intersectsAnyTriangle(const Point3D& voxelCorner);

	bool lineIntersectsVoxel(const Point3D& voxelCorner, const Point3D& p1, const Point3D& p2, int voxelSize);

	bool planeAABBIntersect(const Point3D& normal, const Point3D& voxelCorner);

	void findPlaneCubeIntersections(const Point3D& normal, const Point3D& voxelCorner, std::vector<Point3D>& intersectionPoints);

	bool triangleIntersectsVoxel(const Point3D& voxelCorner, const Point3D& p1, const Point3D& p2, const Point3D& p3, int voxelSize);

	bool aabbIntersectsTriangle(const Point3D& min, const Point3D& max, const Point3D& p1, const Point3D& p2, const Point3D& p3);

	bool isInsideTriangle(const Point3D& point, const Point3D& p1, const Point3D& p2, const Point3D& p3);

	// Function to set voxel size
	void setVoxelSize(int inVoxelSize);

private:
	// Private constructor taking filename and voxel size as parameters
	Voxelizer(std::string fileName, int inVoxelSize);
	// Private destructor
	~Voxelizer();

	// Function to create cubes from the input file
	void makeCubes(std::string fileName);

	// Function to add vertices for a cuboid defined by two points
	void addCubicalVetices(const Point3D& point1, const Point3D& point2, int voxelSize);

	// Function to add a cube with a specified corner and voxel size
	void addCube(const Point3D& voxelCorner, int voxelSize);

	// Function to add a quad (a face of a cube) defined by four points
	void addQuad(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4);

private:
	int mVoxelSize; // Voxel size
	std::vector<float> mVertices; // Vector to store vertices of cubes
	std::vector<float> mColors; // Vector to store colors of cubes
	std::vector<float>mNormals;
	std::vector<Point3D> mV;  // Member variable for vertices
	std::vector<Point3D> mC;  // Member variable for colors
	std::vector<Point3D> mN;  // Member variable for normals
};
