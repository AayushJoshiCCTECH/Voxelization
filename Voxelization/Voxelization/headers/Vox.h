#pragma once
#include <qgridlayout.h>
#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QSpinBox>
#include <QColorDialog>
#include <qlabel.h>
#include "Triangle.h"
class OpenGLWindow;

class Vox : public QMainWindow
{
    Q_OBJECT

public:
    Vox(QWindow* parent = nullptr);
    ~Vox();
    std::vector<Triangle> trianglesData;
    std::string fileName;
private:
    void setupUi();

    static QString randomColor();

private:
    QWidget* mWidget;
    QPushButton* mBrowseButton;
    QPushButton* mRenderButton;
    QPushButton* mVoxelizeButton;
    QPushButton* mColorDialogButton;
    QSpinBox* mSpinBox;
    QLabel* mSizeLabel;
    OpenGLWindow* mRenderer;
    QGridLayout* mGridLayout;

private:
    void setRandomBackgroundColor(QPushButton* button);
    void setRandomBackgroundColor(QSpinBox* spinBox);
    void openFileDialog();
    void showColorDialog();
    void renderSTL();
    void voxelizeSTL();
};

