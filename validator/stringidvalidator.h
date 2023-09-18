#ifndef STRINGIDVALIDATOR_H
#define STRINGIDVALIDATOR_H

#include <QValidator>
#include <string>
#include "data/dataframe.h"

class StringIdValidator : public QRegExpValidator
{
public:
    explicit StringIdValidator(QRegExp exp, const std::map<std::string, DataFrame>& dataFrames, QObject *parent = nullptr);

    // QValidator interface
public:
    State validate(QString &string, int &pos) const;

private:
    const std::map<std::string, DataFrame>& dataFrames;
};

#endif // STRINGIDVALIDATOR_H
