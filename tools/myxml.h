#ifndef MYXML_H
#define MYXML_H
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include "project/proj659.h"

class MyXml
{
public:
    MyXml(QString path);
    MyXml();
    ~MyXml();
    bool loadXmlFile(QString path);
    bool loadXmlFile();
    bool writeXmlFile(QString path);
    bool writeXmlFile();
    void setPath(const QString &newPath);

    Proj659 &getProject659();
    void setProject659(const Proj659 &newProject659);

private:
    QString path;   //存储XML文件路径
    Proj659 project659;
};

#endif // MYXML_H
