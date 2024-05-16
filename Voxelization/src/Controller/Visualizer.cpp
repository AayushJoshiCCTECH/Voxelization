#include<string>
#include <QRandomGenerator>
#include "Model/Voxelizer.h"
#include "Model/stdafx.h"
#include "Model/STLReader.h"
#include "View/OpenGLWindow.h"
#include "Controller/Visualizer.h"

// Constructor
Visualizer::Visualizer()
{
	setupUi(); // Initialize UI

	// Initial visibility settings for buttons
	mBrowseButton->setVisible(true);
	mRenderButton->setVisible(false);
	mSizeLabel->setVisible(false);
	mSpinBox->setVisible(false);
	mVoxelizeButton->setVisible(false);
	mColorDialogButton->setVisible(false);

	// Assign random background color to buttons
	setRandomBackgroundColor(mBrowseButton);
	setRandomBackgroundColor(mRenderButton);
	setRandomBackgroundColor(mSpinBox);
	setRandomBackgroundColor(mVoxelizeButton);
	setRandomBackgroundColor(mColorDialogButton);

	// Connect signals and slots
	connect(mBrowseButton, &QPushButton::clicked, this, &Visualizer::openFileDialog);
	connect(mRenderButton, &QPushButton::clicked, this, &Visualizer::renderSTL);
	connect(mVoxelizeButton, &QPushButton::clicked, this, &Visualizer::voxelizeSTL);
	connect(mColorDialogButton, &QPushButton::clicked, this, &Visualizer::onColorDialogButtonClicked);

}
// Destructor
Visualizer::~Visualizer()
{
}

/// <summary>
/// Initialize UI Elements
/// </summary>
void Visualizer::setupUi()
{
	QFont font("Robo", 14); // Custom font creation
	font.setBold(true);

	// Initialize layout
	mGridLayout = new QGridLayout(this);

	// Add OpenGL window
	mRenderer = new OpenGLWindow(QColor(1.0, 1.0, 0.0), this); // Set background color to dark gray
	mGridLayout->addWidget(mRenderer, 0, 0, 100, 8);

	// Browse STL button
	mBrowseButton = new QPushButton("Browse STL", this);
	// Set button properties
	mBrowseButton->setFixedSize(150, 50);
	mBrowseButton->setFont(font);
	mBrowseButton->setStyleSheet("QPushButton{border: 5px solid black;}"); // Apply border style
	// Add to layout
	mGridLayout->addWidget(mBrowseButton, 5, 9, 2, 1);

	// Render STL button
	mRenderButton = new QPushButton("Render STL", this);
	// Set button properties
	mRenderButton->setFixedSize(150, 50);
	mRenderButton->setFont(font);
	mRenderButton->setStyleSheet("border: 5px solid black;"); // Apply border style
	// Add to layout
	mGridLayout->addWidget(mRenderButton, 15, 9, 2, 1);

	// Voxel Size label
	mSizeLabel = new QLabel("Voxel Size", this);
	// Set label properties
	mSizeLabel->setTextFormat(Qt::RichText);
	mSizeLabel->setText("<b>" + mSizeLabel->text() + "</b>");
	// Add to layout
	mGridLayout->addWidget(mSizeLabel, 25, 9, 2, 1);

	// Spin Box for voxel size
	mSpinBox = new QSpinBox(this);
	// Set spin box properties
	mSpinBox->setMinimum(1); // Set minimum value
	mSpinBox->setMaximum(100); // Set maximum value
	mSpinBox->setValue(1); // Set default value
	// Add to layout
	mGridLayout->addWidget(mSpinBox, 28, 9, 2, 1);

	// Voxelize button
	mVoxelizeButton = new QPushButton("Voxelize", this);
	// Set button properties
	mVoxelizeButton->setFixedSize(150, 50);
	mVoxelizeButton->setFont(font);
	mVoxelizeButton->setStyleSheet("QPushButton{border: 5px solid black;}"); // Apply border style
	// Add to layout
	mGridLayout->addWidget(mVoxelizeButton, 38, 9, 2, 1);

	// Color dialog button
	mColorDialogButton = new QPushButton("Color", this);
	// Set button properties
	mColorDialogButton->setFixedSize(150, 50);
	mColorDialogButton->setFont(font);
	mColorDialogButton->setStyleSheet("border: 5px solid black;"); // Apply border style
	// Add to layout
	mGridLayout->addWidget(mColorDialogButton, 48, 9, 2, 1);

	// Set font for labels
	mSizeLabel->setFont(font);
	mSpinBox->setFont(font);

	// Set layout
	mWidget = new QWidget(this);
	mWidget->setLayout(mGridLayout);
	setCentralWidget(mWidget);

	// Set window title
	setWindowTitle(QCoreApplication::translate("Voxelizer", "Voxelizer Application", nullptr));
	// Show window maximized
	showMaximized();
}

// Generate a random color
QString Visualizer::randomColor()
{
	return QString("rgb(%1, %2, %3)")
		.arg(QRandomGenerator::global()->bounded(256)) // Red component
		.arg(QRandomGenerator::global()->bounded(256)) // Green component
		.arg(QRandomGenerator::global()->bounded(256)); // Blue component
}

// Set random background color to the button
void Visualizer::setRandomBackgroundColor(QPushButton* button)
{
	button->setStyleSheet(QString("background-color: %1").arg(randomColor()));
}

// Set random background color to the spin box
void Visualizer::setRandomBackgroundColor(QSpinBox* spinBox)
{
	spinBox->setStyleSheet(QString("background-color: %1").arg(randomColor()));
}

// Open file dialog to select STL file
void Visualizer::openFileDialog()
{
	// Show render button and hide other controls
	mRenderButton->setVisible(true);
	mSizeLabel->setVisible(false);
	mSpinBox->setVisible(false);
	mVoxelizeButton->setVisible(false);
	mColorDialogButton->setVisible(false);

	// Open file dialog to select STL file
	QString qFileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "", tr("STL Files (*.stl)"));

	if (!qFileName.isEmpty())
	{
		fileName = qFileName.toStdString();
	}
}

// Slot for rendering the STL file
void Visualizer::renderSTL()
{
	// Show voxel size controls
	mSizeLabel->setVisible(true);
	mSpinBox->setVisible(true);
	mVoxelizeButton->setVisible(true);
	mColorDialogButton->setVisible(false);

	// Call STL renderer in OpenGLWindow
	mRenderer->STLRenderer(fileName);
}

// Slot for voxelizing the STL file
void Visualizer::voxelizeSTL()
{
	// Show color dialog button
	mColorDialogButton->setVisible(true);

	// Get voxel size from spin box
	int voxelSize = mSpinBox->value();

	// Emit signal to voxelize STL in OpenGLWindow
	emit mRenderer->voxelRenderer(fileName, voxelSize);
}


//void Visualizer::onColorDialogButtonClicked()
//{
//	static QColor selectedColor = QColorDialog::getColor();
//	if (selectedColor.isValid())
//	{
//		// User selected a valid color, update the shape color
//		mRenderer->selectColor(selectedColor);
//	}
//	else
//	{
//		// Invalid color selected or dialog canceled
//		qWarning() << "Invalid color selected or dialog canceled.";
//	}
//}

//------------------------------------------------------
//void OpenGLWindow::selectColor()
//{
//	QColorDialog colorDialog;
//	QColor selectedColor = colorDialog.getColor();
//
//	if (selectedColor.isValid()) {
//		// Set the selected color
//		r = selectedColor.redF();
//		g = selectedColor.greenF();
//		b = selectedColor.blueF();
//		update();
//	}
//	else {
//		qWarning() << "Invalid color selected or dialog canceled.";
//	}
//}

void Visualizer::onColorDialogButtonClicked()
{
	mRenderer->selectColor();
}

//--------------------------------------------------------

