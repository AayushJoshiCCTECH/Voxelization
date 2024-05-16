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
	: mBackground(background), renderSTL(false)
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

	glClearColor(0.8f, 0.6f, 0.8f, 1.0f); // Light purple background

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//// Bind shader program
	//mProgram->bind();

	//// Set up transformation matrix
	//QMatrix4x4 matrix;
	//matrix.ortho(-30.0f * zoomFactor, 30.0f * zoomFactor, -30.0f * zoomFactor, 30.0f * zoomFactor, -100.0f, 100.0f * zoomFactor);
	//matrix.translate(0, 0, -2);
	//matrix.rotate(rotationAngle);
	//mProgram->setUniformValue(m_matrixUniform, matrix);
	//mProgram->setUniformValue("r", r);
	//mProgram->setUniformValue("g", g);
	//mProgram->setUniformValue("b", b);

	//------------------------------------------------------
	mProgram->bind();
	QMatrix4x4 modelMatrix;
	QMatrix4x4 translationMatrix;
	QMatrix4x4 scaleMatrix;
	QMatrix4x4 rotationMatrix;
	QMatrix4x4 rotationMatrix1;
	QMatrix4x4 rotationMatrix2;
	QMatrix4x4 viewMatrix;
	QMatrix4x4 projectionMatrix;
	QMatrix3x3 normalMatrix;
	QVector3D lightPos;

	lightPos = QVector3D(0.0f, 0.0f, 20.0f);
	projectionMatrix.ortho(-30.0f * zoomFactor, 30.0f * zoomFactor, -30.0f * zoomFactor, 30.0f * zoomFactor, -100.0f, 100.0f * zoomFactor);
	projectionMatrix.translate(0, 0, -2);
	scaleMatrix.scale(30.0);
	rotationMatrix1.rotate(rotationAngle);
	rotationMatrix2.rotate(180.0, 0.0, 1.0, 0.0);
	rotationMatrix = rotationMatrix1 * rotationMatrix2;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	viewMatrix.setToIdentity();
	normalMatrix = (modelMatrix.normalMatrix()).transposed();
	normalMatrix = (modelMatrix.normalMatrix());

	mProgram->setUniformValue(m_modelMatrixUniform, modelMatrix);
	mProgram->setUniformValue(m_viewMatrixUniform, viewMatrix);
	mProgram->setUniformValue(m_projectionMatrixUniform, projectionMatrix);
	mProgram->setUniformValue(m_normalMatrixUniform, normalMatrix);
	mProgram->setUniformValue(m_lightPosUniform, lightPos);
	//-----------------------------------------------------------

	//// Enable vertex and color attributes
	//glEnableVertexAttribArray(m_posAttr);
	//glEnableVertexAttribArray(m_colAttr);
	//glEnableVertexAttribArray(m_normAttr);


	// Render depending on the mode (STL or voxel)
	if (renderSTL)
	{
		// Enable vertex and color attributes
		glEnableVertexAttribArray(m_posAttr);
		glEnableVertexAttribArray(m_colAttr);
		glEnableVertexAttribArray(m_normAttr);
		// Render STL file (wireframe)
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, mVertices.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, mColors.data());
		glVertexAttribPointer(m_normAttr, 3, GL_FLOAT, GL_FALSE, 0, mNormals.data());

		glDrawArrays(GL_LINE_LOOP, 0, mVertices.size() / 3);
	}
	else
	{
		// Enable vertex and color attributes
		glEnableVertexAttribArray(m_posAttr);
		glEnableVertexAttribArray(m_colAttr);
		glEnableVertexAttribArray(m_normAttr);
		// Render voxel data (solid)
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, mVertices.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, mColors.data());
		glVertexAttribPointer(m_normAttr, 3, GL_FLOAT, GL_FALSE, 0, mNormals.data());


		glDrawArrays(GL_QUADS, 0, mVertices.size() / 3);
	}

	// Disable vertex and color attributes
	glDisableVertexAttribArray(m_normAttr);
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

//------------------------------------------

//static const char* vertexShaderSource =
//"attribute highp vec4 posAttr; \n"
//"attribute lowp vec4 colAttr;\n"
//"attribute lowp vec3 norAttr;\n"
//"varying lowp vec4 col;\n"
//"varying vec3 vert;\n"
//"varying vec3 vertNormal;\n"
//"uniform highp mat4 projMatrix;\n"
//"uniform highp mat4 viewMatrix;\n"
//"uniform highp mat4 modelMatrix;\n"
//"uniform mat3 normalMatrix;\n"
//"void main() {\n"
//"   col = colAttr;\n"
//"   vert = posAttr.xyz;\n"
//"   gl_PointSize = 10.0;\n"
//"   vertNormal = normalMatrix * norAttr;\n"
//"   gl_Position = projMatrix * viewMatrix * modelMatrix * posAttr;\n"
//"}\n";
//
//static const char* fragmentShaderSource =
//"varying lowp vec4 col;\n"
//"varying highp vec3 vert;\n"
//"varying highp vec3 vertNormal;\n"
//"uniform highp vec3 lightPos;\n"
//"void main() {\n"
//"  highp vec3 L = normalize(lightPos - vert);\n"
//"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
//"   highp vec3 color = vec3(col);\n"
//"   highp vec3 col1 = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
//"   gl_FragColor = vec4(col1, 1.0);\n"
//"}\n";

//--------------------------------------------------------------

void OpenGLWindow::initializeGL()
{
	// Initialize OpenGL functions
	initializeOpenGLFunctions();

	// Create shader program
	mProgram = new QOpenGLShaderProgram(this);
	mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	mProgram->link();

	/*m_posAttr = mProgram->attributeLocation("posAttr");
	Q_ASSERT(m_posAttr != -1);
	m_matrixUniform = mProgram->uniformLocation("matrix");
	Q_ASSERT(m_matrixUniform != -1);*/

	//----------------------------------------------------------
	glViewport(0, 0, width(), height());
	float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
	float nearPlane = 1.0f;
	float farPlane = 1000.0f;
	QMatrix4x4 projectionMatrix;
	projectionMatrix.perspective(45.0f, aspectRatio, nearPlane, farPlane);
	mProgram->setUniformValue(m_projectionMatrixUniform, projectionMatrix);


	m_posAttr = mProgram->attributeLocation("posAttr");
	Q_ASSERT(m_posAttr != -1);
	//m_colAttr = mProgram->attributeLocation("colAttr");
	//Q_ASSERT(m_colAttr != -1);
	m_normAttr = mProgram->attributeLocation("norAttr");
	Q_ASSERT(m_normAttr != -1);
	m_modelMatrixUniform = mProgram->uniformLocation("modelMatrix");
	Q_ASSERT(m_modelMatrixUniform != -1);
	m_viewMatrixUniform = mProgram->uniformLocation("viewMatrix");
	Q_ASSERT(m_viewMatrixUniform != -1);
	m_projectionMatrixUniform = mProgram->uniformLocation("projMatrix");
	Q_ASSERT(m_projectionMatrixUniform != -1);
	m_normalMatrixUniform = mProgram->uniformLocation("normalMatrix");
	Q_ASSERT(m_normalMatrixUniform != -1);
	m_lightPosUniform = mProgram->uniformLocation("lightPos");
	Q_ASSERT(m_lightPosUniform != -1);
	//-----------------------------------------------------------

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

//void OpenGLWindow::selectColor(const QColor& color)
//{
//	QColorDialog colorDialog(this);
//	QColor selectedColor = colorDialog.getColor();
//
//	if (selectedColor.isValid()) {
//		// Set the selected color
//		r = selectedColor.redF();
//		g = selectedColor.greenF();
//		b = selectedColor.blueF();
//		update();
//	}
//}
//-----------------------------------------------------------
void OpenGLWindow::selectColor()
{
	QColorDialog colorDialog;
	QColor selectedColor = colorDialog.getColor();

	if (selectedColor.isValid()) {
		r = selectedColor.redF();
		g = selectedColor.greenF();
		b = selectedColor.blueF();
		update();
	}
	else {
		qWarning() << "Invalid color selected or dialog canceled.";
	}
}

//------------------------------------------------------------