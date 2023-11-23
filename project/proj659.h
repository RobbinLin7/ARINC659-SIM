#ifndef PROJ659_H
#define PROJ659_H
#include<QString>
#include<QMap>
#include<memory>
#include"deviceModel/bodyFrameGraphicsItem.h"
#include"data/bodyframe.h"
class Proj659
{
public:
    enum Status{saved, unsaved};
    Proj659();
    ~Proj659();
    Proj659(QString name, QString description, QTreeWidgetItem* projectTree = nullptr);
    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);

    bool getSave() const;
    void setSave(bool newSave);
    uint getMinUnusedId() const;
    bool addBodyFrameItem(const BodyFrame&);
    bool deleteBodyFrameItem(uint id);
    BodyFrame& getBodyFrameItem(uint id);
    const BodyFrame &getBodyFrameItem(uint id)const;
    Status getStatus() const;
    void setStatus(Status newStatus);

    const QMap<uint, BodyFrame> &getBodyFrameItems() const;

    const QString &getCommandFilePath() const;
    void setCommandFilePath(const QString &newCommandFilePath);

    QTreeWidgetItem *getProjectTree() const;

private:
    QString name;
    QString description;
    QString commandFilePath;
    QTreeWidgetItem* projectTree;
    bool save = true;
    QMap<uint, BodyFrame> bodyFrameItems;
    bool used[maxFrameId] = {false};
    Status status = unsaved;
};



#endif // PROJ659_H
