#pragma once

//#include <QtWidgets/QMainWindow>
//#include "ui_Vox.h"

//class Vox : public QMainWindow
//{
//    Q_OBJECT
//
//public:
//    Vox(QWidget *parent = nullptr);
//    ~Vox();
//
//private:
//    Ui::VoxClass ui;
//};






#pragma once
#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QSpinBox>
#include <QColorDialog>
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

private:
    QPushButton* mOpenButton;
    QPushButton* mRenderButton;
    QPushButton* mVoxelizeButton;
    QPushButton* mColorDialogButton;
    QSpinBox* mSpinBox;
    OpenGLWindow* mRenderer;

private:
    void openFileDialog();
    void showColorDialog();
    void renderSTL();
    void voxelizeSTL();
};

