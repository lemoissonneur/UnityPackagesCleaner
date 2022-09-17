#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "unity.h"
#include "unitypackagesfinder.h"
#include "unityprojectsfinder.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void Delete(void);

private:
    Ui::MainWindow *ui;
    QList<UnityPackage> unused = QList<UnityPackage>();
    UnityProjectsFinder *projects;
    UnityPackagesFinder *packages;
};
#endif // MAINWINDOW_H
