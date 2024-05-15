#include <fstream>
#include <sstream>
#include "Model/STLReader.h"
#include "string"

using namespace IOOperation;

// Default constructor
STLReader::STLReader()
{

}

// Constructor taking file path and vectors for vertices, colors, and normals
STLReader::STLReader(std::string filePath, std::vector<Point3D>& vertices, std::vector<Point3D>& colors, std::vector<Point3D>& normals)
{
    // Clear the vectors before reading
    vertices.clear();
    colors.clear();
    normals.clear();
    // Read the STL file
    readSTL(filePath, vertices, colors, normals);
}

// Destructor
STLReader::~STLReader()
{

}

// Method to read STL file and populate vectors with vertices, colors, and normals
void STLReader::readSTL(std::string filePath, std::vector<Point3D>& vertices, std::vector<Point3D>& colors, std::vector<Point3D>& normals)
{
    // Open the STL file
    std::ifstream dataFile;
    dataFile.open(filePath);
    if (!dataFile.is_open())
    {
        // If file cannot be opened, return
        return;
    }

    std::string line;
    // Read each line of the file
    while (std::getline(dataFile, line))
    {
        // Check if the line contains "normal" which indicates normal vectors
        if (line.find("normal") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string token;
            float x, y, z;
            iss >> token >> x >> y >> z;

            // Create normal vector
            Point3D normal(x, y, z);

            // Add normal to the normals vector (for each vertex of the triangle)
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
        }
        // Check if the line contains "vertex" which indicates vertex coordinates
        if (line.find("vertex") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string token;
            float x, y, z;
            iss >> token >> x >> y >> z;

            // Create first vertex
            Point3D p1(x, y, z);
            // Create first color (red)
            Point3D c1(1.0f, 0.0f, 0.0f);

            // Read next line for second vertex coordinates
            std::getline(dataFile, line);
            std::istringstream iss1(line);
            std::string token1;
            float x1, y1, z1;
            iss1 >> token1 >> x1 >> y1 >> z1;

            // Create second vertex
            Point3D p2(x1, y1, z1);
            // Create second color (red)
            Point3D c2(1.0f, 0.0f, 0.0f);

            // Read next line for third vertex coordinates
            std::getline(dataFile, line);
            std::istringstream iss2(line);
            std::string token2;
            float x2, y2, z2;
            iss2 >> token2 >> x2 >> y2 >> z2;

            // Create third vertex
            Point3D p3(x2, y2, z2);
            // Create third color (red)
            Point3D c3(1.0f, 0.0f, 0.0f);

            // Add vertices (forming a triangle) to the vertices vector
            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);
            vertices.push_back(p1);

        }
    }
    // Close the file after reading
    dataFile.close();
}
