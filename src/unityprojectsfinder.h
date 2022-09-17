#ifndef UNITYPROJECTSFINDER_H
#define UNITYPROJECTSFINDER_H

#include "unity.h"

#include <QObject>
#include <QTextBrowser>
#include <QString>
#include <QFile>
#include <QDir>


class UnityProjectsFinder : public QObject
{
    Q_OBJECT
public:
    struct UnityProject
    {
    public:
        QString name;
        QDir folder;
        QList<UnityPackage> packages = QList<UnityPackage>();

        UnityProject() {}
        UnityProject(QString path);

        ~UnityProject() {}
    };

    QList<UnityProject> projects = QList<UnityProject>();
    QTextBrowser* output;

    explicit UnityProjectsFinder(QObject *parent = nullptr, QTextBrowser* output = nullptr);

    ///
    /// \brief Find Path of projects opened with unity hub
    /// \return
    ///
    bool FindProjects();

private:

    ///
    /// \brief Find Path of opened projects from a unity hub log file
    /// \param logFile
    /// \return
    ///
    QStringList FindProjectsPathInLog(QFileInfo logFile);

    ///
    /// \brief Remove duplicate '/' and '\', and normalize path with '/' char
    /// \param str
    /// \return
    ///
    static QString CleanupPath(QString str);

signals:

};

#endif // UNITYPROJECTSFINDER_H
