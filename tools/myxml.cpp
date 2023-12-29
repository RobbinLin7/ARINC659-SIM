#include "myxml.h"
std::string MyXml::windowTypeIdToString[] = {"DATA_SEND", "VERSION_SEND", "LONG_SYNC", "FRAME_SWITCH", "CALL_SUBFRAME", "INT_SEND", "FRAME_JUMP", "FREE", "SHORT_SYNC"};
std::map<std::string, uint> MyXml::windowTypeStringToId = {
    std::make_pair("DATA_SEND", 0),
    std::make_pair("VERSION_SEND", 1),
    std::make_pair("LONG_SYNC", 2),
    std::make_pair("FRAME_SWITCH", 3),
    std::make_pair("CALL_SUBFRAME", 4),
    std::make_pair("INT_SEND", 5),
    std::make_pair("FRAME_JUMP", 6),
    std::make_pair("FREE", 7),
    std::make_pair("SHORT_SYNC", 8)
};
#include<QDir>
bool MyXml::saveProjectToXml(const Proj659& project)
{
    QDir dir;
    if(dir.exists(project.getPath()) == false){
        dir.mkdir(project.getPath());
    }
    QFile file(project.getPath() + "/" + project.getName() + ".proj659");
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
        QDomText name_text = doc.createTextNode(project.getName());
        name.appendChild(name_text);
        root.appendChild(name);

        // 添加子节点description
        QDomElement description = doc.createElement("description");
        QDomText description_text = doc.createTextNode(project.getDescription());
        description.appendChild(description_text);
        root.appendChild(description);

        // 添加BodyFrames
        if(project.getBodyFrameItems().empty() == false){
            QDomElement bodyFrames = doc.createElement("bodyFrames");
            for(auto x : project.getBodyFrameItems()){
                QDomElement bodyFrame = doc.createElement("bodyFrame");
                //TODO
                //QDomElement bodyFrameId = doc.createElement("bodyframeid");
                //bodyFrameId.appendChild(doc.createTextNode(QString::number(x.getBodyFrameItemID())));
                //bodyFrame.appendChild(bodyFrameId);
                func(&doc, &bodyFrame, "bodyFrameId", QString::number(x.getBodyFrameItemID()));
                func(&doc, &bodyFrame, "arbitrationStepDuration", QString::number(x.getArbitrationStepDuration()));
                func(&doc, &bodyFrame, "timeCalibrationFactor", QString::number(x.getTimeCalibrationFactor()));
                func(&doc, &bodyFrame, "majorVersionNumber", QString::number(x.getMajorVersionNumber()));
                func(&doc, &bodyFrame, "subVersionNumber", QString::number(x.getSubVersionNumber()));
                func(&doc, &bodyFrame, "messageInterval", QString::number(x.getMessageInterval()));


                if(x.getModules().empty() == false){
                    QDomElement modules = doc.createElement("modules");
                    for(auto module : x.getModules()){
                        QDomElement moduleItem = doc.createElement("module");
                        func(&doc, &moduleItem, "moduleNumber", QString::number(module.second.getModuleNumber()));
                        func(&doc, &moduleItem, "initialWaitTime", QString::number(module.second.getInitialWaitTime()));
                        func(&doc, &moduleItem, "moduleName", QString::fromStdString(module.second.getModuleName()));
                        func(&doc, &moduleItem, "moduleType", QString::number(module.second.getModuleType()));
                        modules.appendChild(moduleItem);
                    }
                    bodyFrame.appendChild(modules);
                }
                //TODO DATAFRAMES


                if(x.getDataFrames().empty() == false){
                    QDomElement dataFrames = doc.createElement("dataFrames");
                    for(auto it = x.getDataFrames().cbegin(); it != x.getDataFrames().cend(); ++it){
                        const DataFrame dataFrame = *it;
                        QDomElement dataFrameItem = doc.createElement("dataFrame");
                        func(&doc, &dataFrameItem, "idleWaitTime", QString::number(dataFrame.getIdleWaitTime()));
                        func(&doc, &dataFrameItem, "framePeriod", QString::number(dataFrame.getFramePeriod()));
                        func(&doc, &dataFrameItem, "totalWindow", QString::number(dataFrame.getTotalWindow()));
                        func(&doc, &dataFrameItem, "frameIdentification", QString::fromStdString(dataFrame.getFrameIdentification()));
                        func(&doc, &dataFrameItem, "timeAllocationType", QString::number(dataFrame.getTimeAllocationType()));
                        //func(&doc, &dataFrameItem, "windows");
                        QDomElement windows = doc.createElement("windows");
                        for(auto &window: it->getFrameWindows()){
                            QDomElement windowItem = doc.createElement("window");
                            switch (window.getWindowType()) {

                            }
                            func(&doc, &windowItem, "windowId", QString::number(window.getWindowId()));
                            func(&doc, &windowItem, "windowType", QString::fromStdString(windowTypeIdToString[window.getWindowType()]));
                            func(&doc, &windowItem, "mainLRM", QString::number(window.getMainLRM()));
                            windows.appendChild(windowItem);
                        }
                        dataFrameItem.appendChild(windows);
                        dataFrames.appendChild(dataFrameItem);
                    }
                    bodyFrame.appendChild(dataFrames);
                }


                bodyFrames.appendChild(bodyFrame);
            }
            root.appendChild(bodyFrames);
        }


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

Proj659 MyXml::loadProjectFromXml(const QString &path)
{
    Proj659 project;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        QXmlStreamReader xmlReader(&file);
        while(!xmlReader.atEnd() && !xmlReader.hasError()){
            xmlReader.readNext();
            if(xmlReader.isStartElement()){
                if(xmlReader.name() == "name"){
                    project.setName(xmlReader.readElementText());
                }
                else if(xmlReader.name() == "description"){
                    project.setDescription(xmlReader.readElementText());
                }
                else if(xmlReader.name() == "bodyFrames"){
                    readBodyFrames(&xmlReader, project);
                }
            }
            else{
                continue;
            }
        }
        file.close();
    }
    return project;
}

void MyXml::func(QDomDocument* doc, QDomElement *parent, const QString &name, const QString &value)
{
    QDomElement child = doc->createElement(name);
    child.appendChild(doc->createTextNode(value));
    parent->appendChild(child);
}

void MyXml::func(QDomDocument *doc, QDomElement *parent, const QString &name)
{
    parent->appendChild(QDomElement(doc->createElement(name)));
}

void MyXml::readBodyFrames(QXmlStreamReader* xmlReader, Proj659& project)
{
    xmlReader->readNext();
    while (xmlReader->name() != "bodyFrames") {
        if(xmlReader->name() == "bodyFrame"){
            readBodyFrame(xmlReader, project);
        }
        xmlReader->readNext();
    }
}

void MyXml::readBodyFrame(QXmlStreamReader *xmlReader, Proj659& project)
{
    xmlReader->readNext();
    BodyFrame bodyFrame;
    while(xmlReader->name() != "bodyFrame"){
        if(xmlReader->isStartElement()){
            if(xmlReader->name() == "bodyFrameId"){
                bodyFrame.setBodyFrameItemID(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "arbitrationStepDuration"){
                bodyFrame.setArbitrationStepDuration(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "timeCalibrationFactor"){
                bodyFrame.setTimeCalibrationFactor(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "majorVersionNumber"){
                bodyFrame.setMajorVersionNumber(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "subVersionNumber"){
                bodyFrame.setSubVersionNumber(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "messageInterval"){
                bodyFrame.setMessageInterval(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "modules"){
                readModules(xmlReader, bodyFrame);
            }
            else if(xmlReader->name() == "dataFrames"){
                readDataFrames(xmlReader, bodyFrame);
            }
        }
        xmlReader->readNext();
    }
    project.addBodyFrameItem(bodyFrame);
}

void MyXml::readModules(QXmlStreamReader *xmlReader, BodyFrame &bodyFrame)
{
    xmlReader->readNext();
    while (xmlReader->name() != "modules") {
        if(xmlReader->name() == "module"){
            readModule(xmlReader, bodyFrame);
        }
        xmlReader->readNext();
    }
}

void MyXml::readModule(QXmlStreamReader *xmlReader, BodyFrame &bodyFrame)
{
    xmlReader->readNext();
    Module module;
    while(xmlReader->name() != "module"){
        if(xmlReader->isStartElement()){
            if(xmlReader->name() == "moduleNumber"){
                module.setModuleNumber(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "initialWaitTimer"){
                module.setInitialWaitTime(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "moduleName"){
                module.setModuleName(xmlReader->readElementText().toStdString());
            }
            else if(xmlReader->name() == "moduleType"){
                if(xmlReader->readElementText().toUInt() == Module::physicalModule){
                    module.setModuleType(Module::physicalModule);
                }
                else{
                    module.setModuleType(Module::simulationModule);
                }
            }
        }
        xmlReader->readNext();
    }
    bodyFrame.addModule(module);
}

void MyXml::readDataFrames(QXmlStreamReader *xmlReader, BodyFrame &bodyFrame)
{
    xmlReader->readNext();
    while (xmlReader->name() != "dataFrames") {
        if(xmlReader->name() == "dataFrame"){
            readDataFrame(xmlReader, bodyFrame);
        }
        xmlReader->readNext();
    }
}

void MyXml::readDataFrame(QXmlStreamReader *xmlReader, BodyFrame &bodyFrame)
{
    xmlReader->readNext();
    DataFrame dataFrame;
    while(xmlReader->name() != "dataFrame"){
        if(xmlReader->isStartElement()){
            if(xmlReader->name() == "idleWaitTime"){
                dataFrame.setIdleWaitTime(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "framePeriod"){
                dataFrame.setFramePeriod(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "totalWindow"){
                dataFrame.setTotalWindow(xmlReader->readElementText().toUInt());
            }
            else if(xmlReader->name() == "frameIdentification"){
                dataFrame.setFrameIdentification(xmlReader->readElementText().toStdString());
            }
            else if(xmlReader->name() == "timeAllocationType"){
                dataFrame.setTimeAllocationType(
                            xmlReader->readElementText().toUInt() == DataFrame::downConcentrationAlloc ?
                                DataFrame::downConcentrationAlloc :
                                DataFrame::equalAlloc);
            }
            else if(xmlReader->name() == "windows"){
                readWindows(xmlReader, dataFrame);
            }
        }
        xmlReader->readNext();
    }
    bodyFrame.addDataFrame(dataFrame);
}

void MyXml::readWindows(QXmlStreamReader *xmlReader, DataFrame &dataFrame)
{
    xmlReader->readNext();
    while(xmlReader->name() != "windows"){
        if(xmlReader->name() == "window"){
            readWindow(xmlReader, dataFrame);
        }
        xmlReader->readNext();
    }
}

void MyXml::readWindow(QXmlStreamReader *xmlReader, DataFrame &dataFrame)
{
    xmlReader->readNext();
    FrameWindow window;
    while(xmlReader->name() != "window"){
        if(xmlReader->name() == "windowId"){
            window.setWindowId(xmlReader->readElementText().toUInt());
        }
        else if(xmlReader->name() == "windowType"){
            window.setWindowType((FrameWindow::WindowType)windowTypeStringToId[xmlReader->readElementText().toStdString()]);
        }
        xmlReader->readNext();
    }
    dataFrame.insertFrameWindow(window);
}
