// OpenGLWindow.h
#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <vector>
#include <QQuaternion>
#include "Model/Point3D.h" // Including header file for Point3D class
#include "Model/Triangle.h" // Including header file for Triangle class

class QOpenGLTexture;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLPaintDevice;

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

signals:
	void shapeUpdate();

public:
	// Constructor
	OpenGLWindow(const QColor& background, QWidget* parent);

	// Destructor
	~OpenGLWindow();

	// Mouse move event handler
	void mouseMoveEvent(QMouseEvent* event);

	// Wheel event handler
	void wheelEvent(QWheelEvent* event);

	// Render an STL file
	void STLRenderer(std::string fileName);

	//---------------------------------------------
	void selectColor();
	//---------------------------------------------
	//void selectColor(const QColor& color);


	// Render voxels
	void voxelRenderer(std::string fileName, int voxelSize);

public:
	// Read shader code from file
	QString readShader(QString filePath);

protected:
	// OpenGL paint event handler
	void paintGL() override;

	// Initialize OpenGL context
	void initializeGL() override;

private:
	// Reset OpenGL settings
	void reset();

	QOpenGLShader* mVshader = nullptr; // Vertex shader
	QOpenGLShader* mFshader = nullptr; // Fragment shader
	QOpenGLShaderProgram* mProgram = nullptr; // Shader program
	QOpenGLBuffer mVbo; // Vertex buffer object
	int mVertexAttr; // Vertex attribute location
	int mNormalAttr; // Normal attribute location
	int mMatrixUniform; // Matrix uniform location
	QColor mBackground; // Background color
	QMetaObject::Connection mContextWatchConnection; // Connection for OpenGL context
	GLint m_posAttr = 0; // Attribute location for position
	GLint m_colAttr = 0; // Attribute location for color
	GLint m_normAttr = 0; // Attribute location for normal
	GLint m_matrixUniform = 0; // Uniform location for matrix
	QVector<QVector2D> mPixelVertices; // Pixel vertices

	std::vector<float> mVertices; // Vertices
	std::vector<float> mColors; // Colors
	std::vector<float> mNormals; // Normals
	std::vector<Point3D> mV; // Vector of points for vertices
	std::vector<Point3D> mC; // Vector of points for colors
	std::vector<Point3D> mN; // Vector of points for normals
	std::vector<float> voxelData; // Voxel data
	int gridSize = 12; // Grid size
	float zoomFactor = 1.0f; // Zoom factor
	bool renderSTL; // Render mode for STL
	QQuaternion rotationAngle; // Rotation angle
	QPoint lastPos; // Last mouse position
	float r = 0.0f, g = 1.0f, b = 1.0f; // Color components

	GLint mLightPosUniform;
	GLint mViewPosUniform;
	GLint mLightColorUniform;
	GLint mObjectColorUniform;
	GLint m_projectionMatrixUniform = 0;
	GLint m_viewMatrixUniform = 0;
	GLint m_modelMatrixUniform = 0;
	GLint m_normalMatrixUniform = 0;
	GLint m_lightPosUniform = 0;


};

