#include "stringidvalidator.h"
#include <iostream>
StringIdValidator::StringIdValidator(QRegExp exp, const DataFrames& dataFrames, QObject *parent):
    QRegExpValidator(exp, parent),
    dataFrames(dataFrames)
{

}

QValidator::State StringIdValidator::validate(QString &string, int &pos) const{
    QValidator::State state;
    if((state = QRegExpValidator::validate(string, pos)) == QValidator::Acceptable){
        QString str = string.mid(pos);
        if(dataFrames.exist(str.toStdString()) == false){
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }
    else{
        return state;
    }
}
