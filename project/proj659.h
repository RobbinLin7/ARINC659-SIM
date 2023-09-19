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
    Proj659();
    ~Proj659();
    Proj659(QString name, QString description);
    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);

    bool getSave() const;
    void setSave(bool newSave);
    uint getMinUnusedId() const;
    bool addBodyFrameItem(const BodyFrameItem&);
    bool deleteBodyFrameItem(uint id);
    BodyFrameItem getBodyFrameItem(uint id) const;
private:
    QString name;
    QString description;
    bool save = true;
    QMap<uint, BodyFrameItem> bodyFrameItems;
    bool used[maxFrameId] = {false};
};



#endif // PROJ659_H
