#ifndef STRINGIDVALIDATOR_H
#define STRINGIDVALIDATOR_H

#include <QValidator>
#include <string>
#include "data/dataframe.h"
#include "data/dataframes.h"

class StringIdValidator : public QRegExpValidator
{
public:
    explicit StringIdValidator(QRegExp exp, const DataFrames& dataFrames, QObject *parent = nullptr);

    // QValidator interface
public:
    State validate(QString &string, int &pos) const;

private:
    const DataFrames& dataFrames;
};

#endif // STRINGIDVALIDATOR_H
