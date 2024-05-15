#include "Model/GeomContainer.h"

GeomContainer::GeomContainer()
{
    // Constructor: Initialize member vectors
    mVertices.clear();
    mColors.clear();
    mNormals.clear();
}

GeomContainer::GeomContainer(std::vector<Point3D> inPoints) : mPoints(inPoints)
{
    // Constructor with points: Initialize member vectors
    mVertices.clear();
    mColors.clear();
    mNormals.clear();
}

GeomContainer* GeomContainer::getContainer()
{
    // Factory method to create a GeomContainer instance
    GeomContainer* geomContainer = new GeomContainer();
    return geomContainer;
}

GeomContainer* GeomContainer::getContainer(std::vector<Point3D> inPoints)
{
    // Factory method to create a GeomContainer instance with points
    GeomContainer* geomContainer = new GeomContainer(inPoints);
    return geomContainer;
}

void GeomContainer::makeTriangleVertices()
{
    // Clear existing vertices, colors, and normals
    mVertices.clear();
    mColors.clear();
    mNormals.clear();

    // Iterate over points and populate vectors
    for (int i = 0; i < mPoints.size(); i++)
    {
        mVertices.push_back(mPoints[i].x());
        mVertices.push_back(mPoints[i].y());
        mVertices.push_back(mPoints[i].z());

        mColors.push_back(mColor.x());
        mColors.push_back(mColor.y());
        mColors.push_back(mColor.z());

        mNormals.push_back(mPoints[i].normal());
    }
}

void GeomContainer::makeQuadVertices(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4)
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

    // Colors (assigned to each vertex)
    mColors.push_back(mColor.x());
    mColors.push_back(mColor.y());
    mColors.push_back(mColor.z());

    mColors.push_back(mColor.x());
    mColors.push_back(mColor.y());
    mColors.push_back(mColor.z());

    mColors.push_back(mColor.x());
    mColors.push_back(mColor.y());
    mColors.push_back(mColor.z());

    mColors.push_back(mColor.x());
    mColors.push_back(mColor.y());
    mColors.push_back(mColor.z());
}

std::vector<float> GeomContainer::vertices()
{
    // Getter method for vertices
    return mVertices;
}

std::vector<float> GeomContainer::colors()
{
    // Getter method for colors
    return mColors;
}

std::vector<float> GeomContainer::normals()
{
    // Getter method for normals
    return mNormals;
}
