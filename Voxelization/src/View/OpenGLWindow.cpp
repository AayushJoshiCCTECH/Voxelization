#include <QMouseEvent>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QFileInfo>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <string>
#include <cmath>
#include <QColorDialog>
#include "Model/stdafx.h"
#include "View/OpenGLWindow.h"
#include "Model/STLReader.h"
#include "Model/Voxelizer.h"
#include "Model/GeomContainer.h"
#include "Controller/Visualizer.h"



OpenGLWindow::OpenGLWindow(const QColor& background, QWidget* parent)
	: mBackground(background), renderSTL(false),
	mMatrixUniform(-1),
	mNormalAttr(-1),
	mVertexAttr(-1)
{
	// Set parent and minimum size of the OpenGL window
	setParent(parent);
	setMinimumSize(1300, 784);
}

OpenGLWindow::~OpenGLWindow()
{
	// Destructor: Reset OpenGL resources
	reset();
}

void OpenGLWindow::reset()
{
	// Reset OpenGL resources
	makeCurrent();
	delete mProgram;
	mProgram = nullptr;
	delete mVshader;
	mVshader = nullptr;
	delete mFshader;
	mFshader = nullptr;
	mVbo.destroy();
	doneCurrent();

	// Disconnect context watch connection
	QObject::disconnect(mContextWatchConnection);
}

void OpenGLWindow::paintGL()
{
	// Paint OpenGL scene

	glClearColor(0.9f, 0.1f, 0.1f, 1.0f); // Dark gray background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Bind shader program
	mProgram->bind();

	// Set up transformation matrix
	QMatrix4x4 matrix;
	matrix.ortho(-30.0f * zoomFactor, 30.0f * zoomFactor, -30.0f * zoomFactor, 30.0f * zoomFactor, -100.0f, 100.0f * zoomFactor);
	matrix.translate(0, 0, -2);
	matrix.rotate(rotationAngle);
	mProgram->setUniformValue(m_matrixUniform, matrix);
	mProgram->setUniformValue("r", r);
	mProgram->setUniformValue("g", g);
	mProgram->setUniformValue("b", b);


	// Enable vertex and color attributes
	glEnableVertexAttribArray(m_posAttr);
	glEnableVertexAttribArray(m_colAttr);


	// Render depending on the mode (STL or voxel)
	if (renderSTL)
	{
		// Render STL file (wireframe)
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, mVertices.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, mColors.data());

		glDrawArrays(GL_LINE_LOOP, 0, mVertices.size() / 3);
	}
	else
	{
		// Render voxel data (solid)
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, mVertices.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, mColors.data());

		glDrawArrays(GL_QUADS, 0, mVertices.size() / 3);
	}

	// Disable vertex and color attributes
	glDisableVertexAttribArray(m_colAttr);
	glDisableVertexAttribArray(m_posAttr);


}

static const char* vertexShaderSource =
"attribute highp vec4 posAttr;\n"
"attribute lowp vec4 colAttr;\n"
"varying lowp vec4 col;\n"
"uniform highp mat4 matrix;\n"
"void main() {\n"
"   col = colAttr;\n"
"   gl_Position = matrix * posAttr;\n"
"}\n";

static const char* fragmentShaderSource =

"uniform lowp float r;\n"
"uniform lowp float g;\n"
"uniform lowp float b;\n"

"void main() { \n"
"gl_FragColor = vec4(r, g, b, 1.0);\n"
"}";



void OpenGLWindow::initializeGL()
{
	// Initialize OpenGL functions
	initializeOpenGLFunctions();

	// Create shader program
	mProgram = new QOpenGLShaderProgram(this);
	mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	mProgram->link();

	m_posAttr = mProgram->attributeLocation("posAttr");
	Q_ASSERT(m_posAttr != -1);
	m_matrixUniform = mProgram->uniformLocation("matrix");
	Q_ASSERT(m_matrixUniform != -1);

}

QString OpenGLWindow::readShader(QString filePath)
{
	// Read shader source code from file
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "Invalid file";

	QTextStream stream(&file);
	QString line = stream.readLine();
	while (!stream.atEnd()) {
		line.append(stream.readLine());
	}
	file.close();
	return line;
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
	// Handle mouse move event for rotation
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();
	if (event->buttons() & Qt::LeftButton) {
		QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.5f * dx);
		QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.5f * dy);
		rotationAngle = rotX * rotY * rotationAngle;
		update();
	}
	lastPos = event->pos();
}

void OpenGLWindow::wheelEvent(QWheelEvent* event)
{
	// Handle mouse wheel event for zooming
	int delta = event->angleDelta().y();
	if (delta > 0) {
		// Zoom in
		zoomFactor *= 1.1f;
	}
	else {
		// Zoom out
		zoomFactor /= 1.1f;
	}
	update();
}

void OpenGLWindow::voxelRenderer(std::string fileName, int voxelSize)
{
	// Render voxel data
	renderSTL = false;
	Voxelizer* Voxelizer = Voxelizer::getVoxelizer(fileName, voxelSize);
	mVertices = Voxelizer->vertices();
	mColors = Voxelizer->colors();
	update();
}

void OpenGLWindow::STLRenderer(std::string fileName)
{
	// Render STL file
	mVertices.clear();
	mColors.clear();
	mNormals.clear();
	update();
	renderSTL = true;
	IOOperation::STLReader reader(fileName, mV, mC, mN);
	GeomContainer* geomContainer = GeomContainer::getContainer(mV);
	geomContainer->makeTriangleVertices();
	mVertices = geomContainer->vertices();
	mColors = geomContainer->colors();
	mNormals = geomContainer->normals();
	update();
}

void OpenGLWindow::selectColor(const QColor& color)
{
	QColorDialog colorDialog(this);
	QColor selectedColor = colorDialog.getColor();

	if (selectedColor.isValid()) {
		// Set the selected color
		r = selectedColor.redF();
		g = selectedColor.greenF();
		b = selectedColor.blueF();
		update();
	}
}