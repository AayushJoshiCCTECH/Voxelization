# STL to Voxelized Mesh Converter

## Description

This application converts STL files (triangulated mesh) into voxelized meshes. It provides buttons to switch between STL view and voxelized view, and allows users to adjust the dimensions of voxels.


## Classes

1. **CubeCreator**: Responsible for processing STL files and creating cubes for voxelization.
2. **GeomContainer**: Holds vertex, color, and normal data for geometry.
3. **OpenGLWindow**: Handles rendering of geometry using OpenGL.
4. **Point3D**: Represents a point in 3D space and stores information.
5. **STLReader**: Reads STL files to extract vertex, color, and normal data.
6. **Triangle**: Represents a triangle in 3D space.
7. **Vox**: Main application class, handles UI interactions and application flow.

## Installation

To run the application, ensure you have the necessary dependencies installed. This application uses Qt for UI and OpenGL for rendering.

1. Install Qt: Follow the instructions on the [Qt website](https://www.qt.io/download) to download and install Qt.
2. Ensure you have an OpenGL-compatible graphics driver installed on your system.

## Usage

1. Launch the application.
2. Click on the "Browse STL" button to select an STL file.
3. Click on the "Render STL" button to view the STL file.
4. Adjust the voxel size using the spin box.
5. Click on the "Voxelize" button to voxelize the STL file.
6. Optionally, click on the "Color" button to select a color for the voxelized mesh.

## Contributing

Contributions to this project are welcome! If you find any bugs or have suggestions for improvements, feel free to open an issue or submit a pull request.

### Clone this repository to your local machine.

```Clone
git clone https://github.com/AayushJoshiCCTECH/Voxelization.git
