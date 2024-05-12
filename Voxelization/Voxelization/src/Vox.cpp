#include<string>
#include <QRandomGenerator>
#include "CubeCreator.h"
#include "stdafx.h"
#include "Vox.h"
#include "STLReader.h"
#include "OpenGLWindow.h"


Vox::Vox(QWindow* parent)
{
	setupUi();

	//Initial Visibility
	mBrowseButton->setVisible(true);
	mRenderButton->setVisible(false);
	mSizeLabel->setVisible(false);
	mSpinBox->setVisible(false);
	mVoxelizeButton->setVisible(false);
	mColorDialogButton->setVisible(false);


	//Assigned random colour to all the buttons
	setRandomBackgroundColor(mBrowseButton);
	setRandomBackgroundColor(mRenderButton);
	setRandomBackgroundColor(mSpinBox);
	setRandomBackgroundColor(mVoxelizeButton);
	setRandomBackgroundColor(mColorDialogButton);

	//Connections
	connect(mBrowseButton, &QPushButton::clicked, this, &Vox::openFileDialog);
	connect(mRenderButton, &QPushButton::clicked, this, &Vox::renderSTL);
	connect(mVoxelizeButton, &QPushButton::clicked, this, &Vox::voxelizeSTL);
	connect(mColorDialogButton, &QPushButton::clicked, mRenderer, &OpenGLWindow::selectColor);
}

Vox::~Vox()
{
}

void Vox::setupUi()
{
	// Custom font creation
	QFont font("Robo", 14);
	font.setBold(true);

	mGridLayout = new QGridLayout(this);

	// window
	mRenderer = new OpenGLWindow(QColor(1.0, 1.0, 0.0), this); // Set background color to dark gray
	mGridLayout->addWidget(mRenderer, 0, 0, 100, 8);

	mBrowseButton = new QPushButton("Browse STL", this);
	mBrowseButton->setFixedSize(150, 50);
	mBrowseButton->setFont(font);
	mBrowseButton->setStyleSheet("QPushButton{border: 5px solid black;}"); // Apply border style
	mGridLayout->addWidget(mBrowseButton, 5, 9, 2, 1);

	mRenderButton = new QPushButton("Render STL", this);
	mRenderButton->setFixedSize(150, 50);
	mRenderButton->setFont(font);
	mRenderButton->setStyleSheet("border: 5px solid black;"); // Apply border style
	mGridLayout->addWidget(mRenderButton, 15, 9, 2, 1);

	mSizeLabel = new QLabel("Voxel Size", this);
	mSizeLabel->setTextFormat(Qt::RichText);
	mSizeLabel->setText("<b>" + mSizeLabel->text() + "</b>");
	mGridLayout->addWidget(mSizeLabel, 25, 9, 2, 1);

	mSpinBox = new QSpinBox(this);
	mSpinBox->setMinimum(1); // Set minimum value
	mSpinBox->setMaximum(100); // Set maximum value
	mSpinBox->setValue(1); // Set default value
	mGridLayout->addWidget(mSpinBox, 28, 9, 2, 1);

	mVoxelizeButton = new QPushButton("Voxelize", this);
	mVoxelizeButton->setFixedSize(150, 50);
	mVoxelizeButton->setFont(font);
	mVoxelizeButton->setStyleSheet("QPushButton{border: 5px solid black;}"); // Apply border style
	mGridLayout->addWidget(mVoxelizeButton, 38, 9, 2, 1);

	mColorDialogButton = new QPushButton("Color", this);
	mColorDialogButton->setFixedSize(150, 50);
	mColorDialogButton->setFont(font);
	mColorDialogButton->setStyleSheet("border: 5px solid black;"); // Apply border style
	mGridLayout->addWidget(mColorDialogButton, 48, 9, 2, 1);

	// Set Custom font
	mSizeLabel->setFont(font);
	mSpinBox->setFont(font);

	mWidget = new QWidget(this);
	mWidget->setLayout(mGridLayout);
	setCentralWidget(mWidget);
	setWindowTitle(QCoreApplication::translate("Voxelizer", "Voxelizer Application", nullptr));
	showMaximized();
}

// Generate a random color
QString Vox::randomColor() 
{
	return QString("rgb(%1, %2, %3)")
		.arg(QRandomGenerator::global()->bounded(256)) // Red component
		.arg(QRandomGenerator::global()->bounded(256)) // Green component
		.arg(QRandomGenerator::global()->bounded(256)); // Blue component
}	

// Set random background color to the button
void Vox::setRandomBackgroundColor(QPushButton* button) 
{
	button->setStyleSheet(QString("background-color: %1").arg(randomColor()));
}

void Vox::setRandomBackgroundColor(QSpinBox* spinBox)
{
	spinBox->setStyleSheet(QString("background-color: %1").arg(randomColor()));
}

void Vox::openFileDialog()
{
	mRenderButton->setVisible(true);
	mSizeLabel->setVisible(false);
	mSpinBox->setVisible(false);
	mVoxelizeButton-> setVisible(false);
	mColorDialogButton-> setVisible(false);

	QString qFileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "", tr("STL Files (*.stl)"));

	if (!qFileName.isEmpty())
	{
		fileName = qFileName.toStdString();
	}
}

// Slot for rendering the STL file
void Vox::renderSTL()
{
	mSizeLabel->setVisible(true);
	mSpinBox->setVisible(true);
	mVoxelizeButton->setVisible(true);
	mColorDialogButton->setVisible(false);

	mRenderer->STLRenderer(fileName);
}

// Slot for voxelizing the STL file
void Vox::voxelizeSTL()
{
	mColorDialogButton->setVisible(true);
	// Get the voxel size from the SpinBox
	int voxelSize = mSpinBox->value();
	// Assuming you have a slot in OpenGLWindow to handle voxelization
	emit  mRenderer->voxelRenderer(fileName, voxelSize);
}


void Vox::showColorDialog()
{
	QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
	if (color.isValid())
	{
		// Do something with the selected color
		qDebug() << "Selected Color: " << color;
	}
}

