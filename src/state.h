#ifndef STATE_H
#define STATE_H
#include <QString>
#include <QVector>
#include <QPoint>
class state
{
public:
    state(QString latexCode, int drawType);
    void pb(int arg);
    QString latexCode;
    int drawType;
    QVector<int> args;
};

#endif // STATE_H
