#include "CubeCreator.h"
#include "STLReader.h"
#include "GeomContainer.h"

CubeCreator::CubeCreator(std::string fileName, int inVoxelSize) : mVoxelSize(inVoxelSize)
{
	// Call makeCubes to process the STL file and create cubes
	makeCubes(fileName);
}

CubeCreator::~CubeCreator()
{
	// Destructor
}

CubeCreator* CubeCreator::getVoxelizer(std::string fileName, int voxelSize)
{
	// Factory method to create a CubeCreator instance
	CubeCreator* cubeCreator = new CubeCreator(fileName, voxelSize);
	return cubeCreator;
}

std::vector<float> CubeCreator::vertices() const
{
	// Getter method for the vertices
	return mVertices;
}

std::vector<float> CubeCreator::colors() const
{
	// Getter method for the colors
	return mColors;
}

void CubeCreator::setVoxelSize(int inVoxelSize)
{
	// Setter method for the voxel size
	mVoxelSize = inVoxelSize;
}

void CubeCreator::makeCubes(std::string fileName)
{
	std::vector<Point3D> mV;
	std::vector<Point3D> mC;
	std::vector<Point3D> mN;

	// Read the STL file to get vertices, colors, and normals
	IOOperation::STLReader reader(fileName, mV, mC, mN);

	// Clear existing vertices and colors before voxelizing
	mVertices.clear();
	mColors.clear();

	for (int i = 0; i < mV.size(); i += 3)
	{
		// Iterate over triangles in the STL file
		Point3D point1 = mV[static_cast<std::vector<Point3D, std::allocator<Point3D>>::size_type>(i)];
		Point3D point2 = mV[static_cast<std::vector<Point3D, std::allocator<Point3D>>::size_type>(i) + 1];
		Point3D point3 = mV[static_cast<std::vector<Point3D, std::allocator<Point3D>>::size_type>(i) + 2];

		// Voxelize each edge of the triangle
		addCubicalVetices(point1, point2, mVoxelSize);
		addCubicalVetices(point2, point3, mVoxelSize);
		addCubicalVetices(point3, point1, mVoxelSize);
	}
}

void CubeCreator::addCubicalVetices(const Point3D& point1, const Point3D& point2, int voxelSize)
{
	Point3D edgeVector = Point3D(point2.x() - point1.x(), point2.y() - point1.y(), point2.z() - point1.z());
	float edgeLength = sqrt(pow(point2.x() - point1.x(), 2) +
		pow(point2.y() - point1.y(), 2) +
		pow(point2.z() - point1.z(), 2));

	float hypo = sqrtf(pow(voxelSize, 2) * 2);
	int numVoxels = static_cast<int>(edgeLength / (voxelSize));

	// Calculate the step size for each voxel
	Point3D voxelStep = edgeVector / numVoxels;

	// Generate cubes along the edge
	for (int i = 0; i < numVoxels * 2; ++i)
	{
		Point3D p(static_cast<double>(i) * voxelStep.x(), static_cast<double>(i) * voxelStep.y(), static_cast<double>(i) * voxelStep.z());
		Point3D voxelCorner = point1 + p / 2;

		// Add cube at voxel corner
		addCube(voxelCorner, voxelSize);
	}
}

void CubeCreator::addCube(const Point3D& corner, int voxelSize)
{
	float xMin = floor(corner.x());
	float yMin = floor(corner.y());
	float zMin = floor(corner.z());

	xMin = abs(xMin) > voxelSize ? xMin - (int(xMin) % voxelSize) : 0;
	yMin = abs(yMin) > voxelSize ? yMin - (int(yMin) % voxelSize) : 0;
	zMin = abs(zMin) > voxelSize ? zMin - (int(zMin) % voxelSize) : 0;
	float xMax = xMin + voxelSize;
	float yMax = yMin + voxelSize;
	float zMax = zMin + voxelSize;

	// Add vertices for each face of the cube

	// Front face
	addQuad(Point3D(xMin, yMin, zMin), Point3D(xMax, yMin, zMin), Point3D(xMax, yMax, zMin), Point3D(xMin, yMax, zMin));

	// Right side face
	addQuad(Point3D(xMax, yMin, zMin), Point3D(xMax, yMax, zMin), Point3D(xMax, yMax, zMax), Point3D(xMax, yMin, zMax));

	// Back face
	addQuad(Point3D(xMax, yMax, zMax), Point3D(xMax, yMin, zMax), Point3D(xMin, yMin, zMax), Point3D(xMin, yMax, zMax));

	// Left face
	addQuad(Point3D(xMin, yMin, zMax), Point3D(xMin, yMax, zMax), Point3D(xMin, yMax, zMin), Point3D(xMin, yMin, zMin));

	// Top face
	addQuad(Point3D(xMin, yMax, zMin), Point3D(xMax, yMax, zMin), Point3D(xMax, yMax, zMax), Point3D(xMin, yMax, zMax));

	// Bottom face
	addQuad(Point3D(xMin, yMin, zMin), Point3D(xMax, yMin, zMin), Point3D(xMax, yMin, zMax), Point3D(xMin, yMin, zMax));
}

void CubeCreator::addQuad(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4)
{
	// Add vertices for a quad and assign colors

	// Vertices
	mVertices.push_back(p1.x());
	mVertices.push_back(p1.y());
	mVertices.push_back(p1.z());

	mVertices.push_back(p2.x());
	mVertices.push_back(p2.y());
	mVertices.push_back(p2.z());

	mVertices.push_back(p3.x());
	mVertices.push_back(p3.y());
	mVertices.push_back(p3.z());

	mVertices.push_back(p4.x());
	mVertices.push_back(p4.y());
	mVertices.push_back(p4.z());

	// Colors (yellow)
	mColors.push_back(0.8f);
	mColors.push_back(0.4f);
	mColors.push_back(0.2f);

	mColors.push_back(0.8f);
	mColors.push_back(0.4f);
	mColors.push_back(0.2f);

	mColors.push_back(0.8f);
	mColors.push_back(0.4f);
	mColors.push_back(0.2f);

	mColors.push_back(0.8f);
	mColors.push_back(0.4f);
	mColors.push_back(0.2f);

}