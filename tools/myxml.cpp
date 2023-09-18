#include "myxml.h"

MyXml::MyXml(QString path)
{
    this->path = path;
    this->loadXmlFile();
}

MyXml::MyXml()
{

}

MyXml::~MyXml()
{
    writeXmlFile();
}

bool MyXml::loadXmlFile(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        this->path = path;
        QXmlStreamReader xmlReader(&file);
        while(!xmlReader.atEnd() && !xmlReader.hasError()){
            xmlReader.readNext();
            if(xmlReader.isStartElement()){
                if(xmlReader.name() == "name"){
                    project659.setName(xmlReader.readElementText());
                }
                else if(xmlReader.name() == "description"){
                    project659.setDescription(xmlReader.readElementText());
                }
            }
            else{
                continue;
            }
        }
        file.close();
        return !xmlReader.hasError();
    }
    else{
        return false;
    }
}

bool MyXml::loadXmlFile()
{
    if(path == "") return false;
    return loadXmlFile(path);
}

bool MyXml::writeXmlFile(Proj659& project659, QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadWrite)){
        QDomDocument doc;

        // 创建XML处理类，通常用于处理第一行描述信息
        QDomProcessingInstruction instruction;

        // 创建XML头部格式
        instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

        // 添加到XML文件中
        doc.appendChild(instruction);

        // 创建根节点
        QDomElement root = doc.createElement("paralist");


        // 添加子节点name
        QDomElement name = doc.createElement("name");
        QDomText name_text = doc.createTextNode(project659.getName());
        name.appendChild(name_text);
        root.appendChild(name);

        // 添加子节点description
        QDomElement description = doc.createElement("description");
        QDomText description_text = doc.createTextNode(project659.getDescription());
        description.appendChild(description_text);
        root.appendChild(description);

        doc.appendChild(root);

        QTextStream stream(&file);
        doc.save(stream, 4);		// 缩进四格

        file.close();

    }
    else{
        return false;
    }
    return true;
}

bool MyXml::writeXmlFile()
{
    if(path == "") return false;
    return writeXmlFile(project659, path);
}

void MyXml::setPath(const QString &newPath)
{
    path = newPath;
}

Proj659 &MyXml::getProject659()
{
    return project659;
}

void MyXml::setProject659(const Proj659 &newProject659)
{
    project659 = newProject659;
}
