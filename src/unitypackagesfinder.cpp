#include "unitypackagesfinder.h"
#include <QStandardPaths>
#include <QDebug>
#include <QFile>


UnityPackagesFinder::UnityPackagesFinder(QObject *parent, QTextBrowser* output)
    : QObject{parent}
{
    this->output = output;
    GetDownloadedPackages();
}

void UnityPackagesFinder::GetDownloadedPackages()
{
    // get unity cache folder path
    QStringList standardPaths = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::AppDataLocation);
    QString unityCacheFolder = standardPaths.first();
    unityCacheFolder.replace("Roaming/UnityPackagesCleaner", "Local/Unity/cache");

    packages.clear();

    output->append("\n\n\n");
    output->append("-------------------------------------");
    output->append("STEP 3 : list packages in unity cache");
    output->append("-------------------------------------");

    pakCache = QDir(unityCacheFolder + "/packages/packages.unity.com");
    if(pakCache.exists())
    {
        output->append("packages found at 'Local/Unity/cache/packages/packages.unity.com' : ");
        foreach(QString s, pakCache.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            if(s.contains('@'))
            {
                UnityPackage package = UnityPackage(s);
                if(!Contains(package))
                {
                    packages.append(UnityPackage(s));
                    output->append(package.name + " @ " + package.version);
                }
            }
        }
    }

    npmCache = QDir(unityCacheFolder + "/npm/packages.unity.com");
    if(npmCache.exists())
    {
        output->append("packages found at 'Local/Unity/cache/npm/packages.unity.com' : ");
        foreach(QString packageName, npmCache.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            QDir packageDir = QDir(npmCache.absolutePath() + "/" + packageName);

            foreach(QString version, packageDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
            {
                if(UnityPackage::IsValidVersion(version))
                {
                    UnityPackage package = UnityPackage(packageName, version);
                    if(!Contains(package))
                    {
                        packages.append(UnityPackage(packageName, version));
                        output->append(package.name + " @ " + package.version);
                    }
                }
            }
        }
    }
}

bool UnityPackagesFinder::Contains(UnityPackage p)
{
    foreach(UnityPackage up, packages)
    {
        if(up.name == p.name && up.version == p.version)
            return true;
    }
    return false;
}

void UnityPackagesFinder::Delete(QString name, QString version)
{
    for(int i=0; i<packages.count(); i++)
    {
        if(name == packages.at(i).name && version == packages.at(i).version)
        {
            Delete(i);
            return;
        }
    }
}

void UnityPackagesFinder::Delete(uint id)
{
    if(id >= packages.count())
        return;

    if(pakCache.exists())
    {
        QDir pakDir = QDir(pakCache.absolutePath() + "/" + packages.at(id).name + "@" + packages.at(id).version);
        if(pakDir.exists())
            pakDir.removeRecursively();
    }

    if(npmCache.exists())
    {
        QDir npmDir = QDir(npmCache.absolutePath() + "/" + packages.at(id).name + "/" + packages.at(id).version);
        if(npmDir.exists())
        {
            npmDir.removeRecursively();
            QDir rootDir = QDir(npmCache.absolutePath() + "/" + packages.at(id).name);
            if(rootDir.isEmpty())
                rootDir.removeRecursively();
        }
    }

    packages.removeAt(id);
}
