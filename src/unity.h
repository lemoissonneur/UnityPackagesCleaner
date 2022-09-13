#ifndef UNITY_H
#define UNITY_H

#include <QString>

struct UnityPackage
{
    public:
        QString name;
        QString version;

        UnityPackage();
        UnityPackage(QString info);
        UnityPackage(QString name, QString version);

        ~UnityPackage();

        static bool IsValidVersion(QString version);
};

#endif // UNITY_H
