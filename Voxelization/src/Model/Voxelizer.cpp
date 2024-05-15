#include <algorithm>
#include <limits>
#include <cmath>
#include "Model/Voxelizer.h" // Including header file for Voxelizer class
#include "Model/STLReader.h" // Including header file for STLReader class
#include "Model/GeomContainer.h" // Including header file for GeomContainer class

Voxelizer::Voxelizer(std::string fileName, int inVoxelSize) : mVoxelSize(inVoxelSize)
{
    // Call makeCubes to process the STL file and create cubes
    makeCubes(fileName);
}

Voxelizer::~Voxelizer()
{
    // Destructor
}

Voxelizer* Voxelizer::getVoxelizer(std::string fileName, int voxelSize)
{
    // Factory method to create a Voxelizer instance
    Voxelizer* voxelizer = new Voxelizer(fileName, voxelSize);
    return voxelizer;
}

std::vector<float> Voxelizer::vertices() const
{
    // Getter method for the vertices
    return mVertices;
}

std::vector<float> Voxelizer::colors() const
{
    // Getter method for the colors
    return mColors;
}
std::vector<float> Voxelizer::normals() const
{
    // Getter method for the colors
    return mNormals;
}

/// <summary>
/// 3D Grid for Voxels
/// </summary>
/// <param name="vertices"></param>
void Voxelizer::createBoundingBoxGrid(const std::vector<Point3D>& vertices) {
    // Determine the bounding box of the mesh
    Point3D minCorner(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Point3D maxCorner(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    for (const auto& vertex : vertices) {
        minCorner.setX(std::min(minCorner.x(), vertex.x()));
        minCorner.setY(std::min(minCorner.y(), vertex.y()));
        minCorner.setZ(std::min(minCorner.z(), vertex.z()));

        maxCorner.setX(std::max(maxCorner.x(), vertex.x()));
        maxCorner.setY(std::max(maxCorner.y(), vertex.y()));
        maxCorner.setZ(std::max(maxCorner.z(), vertex.z()));
    }

    // Create the 3D grid based on the bounding box and voxel size
    for (float x = minCorner.x(); x <= maxCorner.x(); x += mVoxelSize) {
        for (float y = minCorner.y(); y <= maxCorner.y(); y += mVoxelSize) {
            for (float z = minCorner.z(); z <= maxCorner.z(); z += mVoxelSize) {
                Point3D voxelCorner(x, y, z);
                // Check if the voxel intersects with any triangle and store it if it does
                if (intersectsAnyTriangle(voxelCorner)) {
                    addCube(voxelCorner, mVoxelSize);
                }
            }
        }
    }
}


bool Voxelizer::intersectsAnyTriangle(const Point3D& voxelCorner) {
    // Iterate through the triangles and check for intersection
    for (int i = 0; i < mV.size(); i += 3) {
        Point3D point1 = mV[i];
        Point3D point2 = mV[static_cast<std::vector<Point3D, std::allocator<Point3D>>::size_type>(i) + 1];
        Point3D point3 = mV[static_cast<std::vector<Point3D, std::allocator<Point3D>>::size_type>(i) + 2];

        if (triangleIntersectsVoxel(voxelCorner, point1, point2, point3, mVoxelSize)) {
            return true;
        }
    }
    return false;
}

bool Voxelizer::triangleIntersectsVoxel(const Point3D& voxelCorner, const Point3D& p1, const Point3D& p2, const Point3D& p3, int voxelSize) {
    // Implement a robust triangle-voxel intersection test
    // For simplicity, this example assumes AABB (Axis-Aligned Bounding Box) check
    Point3D voxelMax = voxelCorner + Point3D(voxelSize, voxelSize, voxelSize);

    // Perform AABB-Triangle intersection check
    return aabbIntersectsTriangle(voxelCorner, voxelMax, p1, p2, p3);
}

bool Voxelizer::aabbIntersectsTriangle(const Point3D& min, const Point3D& max, const Point3D& p1, const Point3D& p2, const Point3D& p3) {
    // Compute AABB center and extents
    Point3D c = (min + max) * 0.5f;
    Point3D e = (max - min) * 0.5f;

    // Translate triangle to origin
    Point3D v0 = p1 - c;
    Point3D v1 = p2 - c;
    Point3D v2 = p3 - c;

    // Compute edge vectors for the triangle
    Point3D f0 = v1 - v0;
    Point3D f1 = v2 - v1;
    Point3D f2 = v0 - v2;

    // Compute face normals for AABB
    Point3D u0(1, 0, 0);
    Point3D u1(0, 1, 0);
    Point3D u2(0, 0, 1);

    // Test axes a00..a22 (9 tests)
    Point3D axes[9] = {
        Point3D(0, -f0.z(), f0.y()),
        Point3D(0, -f1.z(), f1.y()),
        Point3D(0, -f2.z(), f2.y()),
        Point3D(f0.z(), 0, -f0.x()),
        Point3D(f1.z(), 0, -f1.x()),
        Point3D(f2.z(), 0, -f2.x()),
        Point3D(-f0.y(), f0.x(), 0),
        Point3D(-f1.y(), f1.x(), 0),
        Point3D(-f2.y(), f2.x(), 0)
    };

    for (int i = 0; i < 9; i++) {
        Point3D a = axes[i];
        float p0 = v0.dot(a);
        float p1 = v1.dot(a);
        float p2 = v2.dot(a);

        float r = e.x() * fabs(a.x()) + e.y() * fabs(a.y()) + e.z() * fabs(a.z());

        if (std::max({ -p0, -p1, -p2 }) > r || std::min({ -p0, -p1, -p2 }) < -r) return false;
    }

    // Test face normals of AABB (3 tests)
    if (std::max({ v0.x(), v1.x(), v2.x() }) < -e.x() || std::min({ v0.x(), v1.x(), v2.x() }) > e.x()) return false;
    if (std::max({ v0.y(), v1.y(), v2.y() }) < -e.y() || std::min({ v0.y(), v1.y(), v2.y() }) > e.y()) return false;
    if (std::max({ v0.z(), v1.z(), v2.z() }) < -e.z() || std::min({ v0.z(), v1.z(), v2.z() }) > e.z()) return false;

    // Test face normals of triangle (1 test)
    Point3D n = f0.cross(f1);
    float d = -n.dot(v0);
    float r = e.x() * fabs(n.x()) + e.y() * fabs(n.y()) + e.z() * fabs(n.z());
    if (fabs(d) > r) return false;

    return true;
}


void Voxelizer::makeCubes(std::string fileName)
{
    // Read the STL file to get vertices, colors, and normals
    IOOperation::STLReader reader(fileName, mV, mC, mN);

    // Clear existing vertices and colors before voxelizing
    mVertices.clear();
    mColors.clear();

    // Create bounding box grid and fill triangles
    createBoundingBoxGrid(mV);
}


void Voxelizer::setVoxelSize(int inVoxelSize)
{
    // Setter method for the voxel size
    mVoxelSize = inVoxelSize;
}

void Voxelizer::addCube(const Point3D& voxelCorner, int voxelSize)
{
    float xMin = floor(voxelCorner.x());
    float yMin = floor(voxelCorner.y());
    float zMin = floor(voxelCorner.z());

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

void Voxelizer::addQuad(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4)
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
}
