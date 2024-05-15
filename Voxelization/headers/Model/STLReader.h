#pragma once
#include "vector"
#include "Model/Point3D.h" // Including header file for Point3D class
#include "string"

// Namespace for IOOperation
namespace IOOperation {

	// Class for reading STL files
	class STLReader {
	public:
		// Constructors
		STLReader(); // Default constructor
		STLReader(std::string filePath, std::vector<Point3D>& vertices, std::vector<Point3D>& colors, std::vector<Point3D>& normals); // Constructor with file path and vectors for vertices, colors, and normals
		~STLReader(); // Destructor

	private:
		// Private function to read STL file and populate vectors
		void readSTL(std::string filePath, std::vector<Point3D>& vertices, std::vector<Point3D>& colors, std::vector<Point3D>& normals);
	};
}

