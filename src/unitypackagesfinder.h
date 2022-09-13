#ifndef UNITYPACKAGESFINDER_H
#define UNITYPACKAGESFINDER_H

#include <QObject>
#include <QTextBrowser>
#include <QDir>

#include "unity.h"

class UnityPackagesFinder : public QObject
{
    Q_OBJECT
public:

    explicit UnityPackagesFinder(QObject *parent = nullptr, QTextBrowser* output = nullptr);

    void GetDownloadedPackages();
    bool Contains(UnityPackage p);
    void Delete(QString name, QString version);

    QDir pakCache;
    QDir npmCache;
    QList<UnityPackage> packages = QList<UnityPackage>();
    QTextBrowser* output;

private:
    void Delete(uint id);

signals:

};

#endif // UNITYPACKAGESFINDER_H
