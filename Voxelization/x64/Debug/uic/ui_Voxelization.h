/********************************************************************************
** Form generated from reading UI file 'Voxelization.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOXELIZATION_H
#define UI_VOXELIZATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VoxelizationClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VoxelizationClass)
    {
        if (VoxelizationClass->objectName().isEmpty())
            VoxelizationClass->setObjectName("VoxelizationClass");
        VoxelizationClass->resize(600, 400);
        menuBar = new QMenuBar(VoxelizationClass);
        menuBar->setObjectName("menuBar");
        VoxelizationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VoxelizationClass);
        mainToolBar->setObjectName("mainToolBar");
        VoxelizationClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(VoxelizationClass);
        centralWidget->setObjectName("centralWidget");
        VoxelizationClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(VoxelizationClass);
        statusBar->setObjectName("statusBar");
        VoxelizationClass->setStatusBar(statusBar);

        retranslateUi(VoxelizationClass);

        QMetaObject::connectSlotsByName(VoxelizationClass);
    } // setupUi

    void retranslateUi(QMainWindow *VoxelizationClass)
    {
        VoxelizationClass->setWindowTitle(QCoreApplication::translate("VoxelizationClass", "Voxelization", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VoxelizationClass: public Ui_VoxelizationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOXELIZATION_H
