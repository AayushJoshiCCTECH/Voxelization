#pragma once
#include "vector"
#include "Point3D.h"
#include "string"

// This class is exported from the dll
namespace IOOperation {
	class  STLReader {
	public:
		STLReader();
		STLReader(std::string filePath, std::vector<Point3D>& vertices, std::vector<Point3D>& colors, std::vector<Point3D>& normals);
		~STLReader();
	private:
		void readSTL(std::string filePath, std::vector<Point3D>& vertices, std::vector<Point3D>& colors, std::vector<Point3D>& normals);
	};
}