#include "word.h"

Word::Word()
{
    _group = DefaultGroup;
    _status = NonTest;
}

Word::Word( QString _text, int _group, int _mastered)
{
    this->_text = _text;
    this->_group = _group;
    this->_status = _mastered;
}

bool Word::isRemembered()
{
    return (_status == Word::Remembered);
}

bool Word::isForgotten()
{
    return (_status == Word::Forgotten);
}

void Word::setText(const QString& t)
{
    if( _text != t) {
        _text = t;
    }
}

void Word::setStatus( int val)
{
    if( this->_status != val) {
        _status = val;
    }
}

void Word::setGroup( int val)
{
    if( this->_group != val) {
        _group = val;
    }
}

void Word::setAnswer( const QString& as)
{
    if( this->answer() != as) {
        _answer = as;
    }
}

bool Word::hasAnswer()
{
    return !_answer.isEmpty();
}

QString Word::statusName( int status)
{
    if( status == Word::NonTest) {
        return QObject::tr("No-Test");
    }
    else if( status == Word::Forgotten) {
        return QObject::tr("Forgotten");
    }
    else if( status == Word::Remembered) {
        return QObject::tr("Remembered");
    }
    else {
        return QObject::tr("Unkown");
    }
}
