#ifndef WORD_H
#define WORD_H

#include <vector>
using namespace std;

#include <QObject>
#include <QString>

class Word
{
public:
    Word();
    Word( QString _text, int _group=DefaultGroup, int _mastered=NonTest);
    bool isRemembered();
    bool isForgotten();
    static QString statusName( int status);

public:
    QString text() { return _text; }
    int  group()  { return _group; }
    int  status()  { return _status; }
    QString answer()  { return _answer; }
    void setText(const QString& t);
    void setStatus( int val);
    void setGroup( int val);
    void setAnswer( const QString& as);
    bool hasAnswer();

protected:
    QString _text;
    int  _group;
    int  _status;
    QString _answer;

public:
    enum TestOrder {
       OrderInturn,
       OrderShuffle,
    };

    enum WordStatus {
        NonTest = -1,
        Forgotten,
        Remembered,
    };

    enum GroupType {
        DefaultGroup,
        GreedGroup,
        RedGroup,
        BlueGroup,
    };
};

typedef vector<Word> WordArray;


#endif // WORD_H
