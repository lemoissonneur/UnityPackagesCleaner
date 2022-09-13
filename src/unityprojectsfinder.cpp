#include "unityprojectsfinder.h"
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

UnityProjectsFinder::UnityProjectsFinder(QObject *parent, QTextBrowser* output)
    : QObject{parent}
{
    this->output = output;
    FindProjects();
}

bool UnityProjectsFinder::FindProjects()
{
    QStringList projectsPathList = QStringList();

    // get unity hub log folder path
    QString hubLogFolderPath = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::AppDataLocation)[0];
    hubLogFolderPath.replace("UnityPackagesCleaner", "UnityHub/logs");

    output->append("------------------------------------------------------------");
    output->append("STEP 1 : looking for unity project paths from Unity Hub logs");
    output->append("------------------------------------------------------------");
    output->append("Unity Hub log foler path : " + hubLogFolderPath);


    // get all log files in the log folder
    QDir hubLogFolder = QDir(hubLogFolderPath);
    hubLogFolder.setNameFilters(QStringList("*log*.json"));
    hubLogFolder.setFilter(QDir::Files | QDir::Readable);

    QList<QFileInfo> hubLogFiles = hubLogFolder.entryInfoList();

    //qDebug() << "Unity Hub log files : " << hubLogFiles;


    // search for project path in each log file
    foreach(QFileInfo logFile, hubLogFiles)
    {
        output->append("found log file : " + logFile.absoluteFilePath());
        projectsPathList.append(FindProjectsPathInLog(logFile));
    }

    // clean for duplicate
    projectsPathList.removeDuplicates();

    output->append("\n\n\n");
    output->append("-------------------------------------------------------------");
    output->append("STEP 2 : extract packages dependencies from existing projects");
    output->append("-------------------------------------------------------------");

    // clean non-existing folder or folder that doesn't contains a packages-lock.json
    foreach(QString s, projectsPathList)
    {
        QFileInfo f = QFileInfo(s);

        if(f.exists() && f.isDir()) // check if folder still exists
        {
            QFileInfo lockFile = QFileInfo(f.absoluteFilePath() + "/Packages/packages-lock.json");

            // check packages-lock.json exists and can be read
            if(lockFile.exists() && lockFile.isFile() && lockFile.isReadable())
            {
                output->append("found packages-lock.json for project : " + s);
                projects.append(UnityProject(s));
            }
        }
        else output->append("no project found at : " + s);
    }

    return projects.count() > 0;
}


QStringList UnityProjectsFinder::FindProjectsPathInLog(QFileInfo logFile)
{
    // check if file can be open, return empty list if not
    QFile file(logFile.absoluteFilePath());

    if(!file.open(QIODevice::ReadOnly)) {
        return QStringList();
    }

    QTextStream logData(&file);
    QStringList projectsPathList = QStringList();

    // read log  file  line by line
    while(!logData.atEnd())
    {
        QString line = logData.readLine();

        if(line.contains("projectPath: '"))
        {
            qsizetype pathStart = line.indexOf("projectPath: '") + 14;
            qsizetype pathEnd = line.indexOf("'", pathStart);

            // extract path from log line
            QString projectPath = line.mid(pathStart, pathEnd-pathStart);

            projectPath = CleanupPath(projectPath);

            if(!projectsPathList.contains(projectPath))
            {
                projectsPathList.append(projectPath);

                output->append("found reference to unity project at path : " + projectPath);
            }
        }
    }

    file.close();

    return projectsPathList;
}

QString UnityProjectsFinder::CleanupPath(QString str)
{
    // normalize path with '/' instead of '\'
    str.replace('\\', '/');

    // remove duplicates
    while(str.contains("//"))
        str.replace("//", "/");

    return str;
}

UnityProjectsFinder::UnityProject::UnityProject(QString path)
{
    folder = QDir(path);
    if(folder.exists())
    {
        name = folder.absolutePath().split('/').last();

        // check if file can be open, return empty list if not
        QFile file(folder.absolutePath() + "/Packages/packages-lock.json");

        if(file.open(QIODevice::ReadOnly))
        {
            QJsonDocument data = QJsonDocument::fromJson(file.readAll());

            QJsonObject dependencies = data.object().value("dependencies").toObject();

            foreach(QString packageName, dependencies.keys())
            {
                QJsonObject packageData = dependencies.value(packageName).toObject();
                QString packageVersion = packageData.value("version").toString();

                if(UnityPackage::IsValidVersion(packageVersion))
                    packages.append(UnityPackage(packageName, packageVersion));
            }
        }
    }
}





