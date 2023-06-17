#include "proj659.h"

Proj659::Proj659()
{

}

Proj659::Proj659(QString name, QString description)
{
    this->name = name;
    this->description = description;
}

const QString &Proj659::getName() const
{
    return name;
}

void Proj659::setName(const QString &newName)
{
    name = newName;
}

const QString &Proj659::getDescription() const
{
    return description;
}

void Proj659::setDescription(const QString &newDescription)
{
    description = newDescription;
}
