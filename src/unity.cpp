#include "unity.h"
#include <QList>

UnityPackage::UnityPackage()
{
    this->name = QString("None");
    this->version = QString("0.0.0");
}

UnityPackage::UnityPackage(QString info)
{
    QStringList packageInfos = info.split('@');

    this->name = packageInfos.first();
    this->version = packageInfos.last();
}

UnityPackage::UnityPackage(QString name, QString version)
{
    this->name = name;
    this->version = version;
}

UnityPackage::~UnityPackage()
{

}

bool UnityPackage::IsValidVersion(QString version)
{
    QStringList versionInfo = version.split('.');
    if(versionInfo.count() == 3)
    {
        foreach(QString subV, versionInfo)
        {
            bool ok;
            subV.toUInt(&ok);
            if(!ok) return false;
        }
        return true;
    }
    return false;
}
