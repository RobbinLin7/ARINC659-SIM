#ifndef PROJ659_H
#define PROJ659_H
#include<QString>

class Proj659
{
public:
    Proj659();
    Proj659(QString name, QString description);
    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);

private:
    QString name;
    QString description;
};

#endif // PROJ659_H
