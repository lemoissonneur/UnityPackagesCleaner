#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "unity.h"
#include "unityprojectsfinder.h"
#include "unitypackagesfinder.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("UnityPackagesCleaner");

    projects = new UnityProjectsFinder(this, ui->logger);
    if(!projects->FindProjects())
    {
        ui->logger->append("ERROR: fail to find unity projects");
        ui->deleteButton->setEnabled(false);
        return;
    }
    packages = new UnityPackagesFinder(this, ui->logger);

    ui->logger->append("\n\n\n");
    ui->logger->append("-----------------------------");
    ui->logger->append("STEP 4 : find unused packages");
    ui->logger->append("-----------------------------");
    ui->logger->append("found " + QVariant(projects->projects.size()).toString() + " projects");
    ui->logger->append("found " + QVariant(packages->packages.size()).toString() + " packages");


    foreach(UnityPackage pack, packages->packages)
    {
        bool used = false;
        foreach(UnityProjectsFinder::UnityProject proj, projects->projects)
        {
            foreach(UnityPackage usedPack, proj.packages)
            {
                if(usedPack.name == pack.name && usedPack.version == pack.version)
                    used = true;
            }
        }

        if(!used) unused.append(pack);
    }

    ui->logger->append("found " + QVariant(unused.size()).toString() + " unused packages : ");
    foreach(UnityPackage pack, unused)
    {
        ui->logger->append(pack.name + "@" + pack.version);
    }

    QObject::connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::Delete);
}

void MainWindow::Delete(void)
{
    ui->logger->append("\n\n\n");
    ui->logger->append("---------------------------------");
    ui->logger->append("STEP 5 : deleting unused packages");
    ui->logger->append("---------------------------------");
    foreach(UnityPackage p, unused)
    {
        if(packages->Contains(p))
        {
            ui->logger->append("Deleting package : " + p.name + "@" + p.version );
            packages->Delete(p.name, p.version);
        }
    }

    ui->logger->append("Done.");
    QObject::disconnect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::Delete);
}

MainWindow::~MainWindow()
{
    delete ui;
}

