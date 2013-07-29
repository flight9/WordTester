#include "WSheet.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

WSheet::WSheet(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_WIN32
    connect(&_voice, SIGNAL(readingCompleted()),
            this, SIGNAL(voiceCompleted()));
#endif
}

QString WSheet::textAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return QString::null;
    return _warry.at(idx).text();
}

Word::WordStatus WSheet::statusAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return Word::NonTest;
    return (Word::WordStatus)_warry.at(idx).status();
}

QString WSheet::statusNameAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return QString::null;
    return Word::statusName(_warry.at(idx).status());
}

QString WSheet::answerAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return QString::null;
    return _warry.at(idx).answer();
}

void WSheet::setText(QString txt, int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return;
    _warry.at(idx).setText(txt);
}

void WSheet::setStatus(Word::WordStatus sta, int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return;
    _warry.at(idx).setStatus(sta);
}

void WSheet::setAnswer(QString ans, int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return;
    _warry.at(idx).setAnswer(ans);
}

void WSheet::eraseAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return;
    _warry.erase(_warry.begin()+idx);
}

void WSheet::pushBack(const Word& word)
{
    _warry.push_back( word);
}

bool WSheet::initReader()
{
#ifdef Q_OS_WIN32
    return _voice.initial();
#else
    return false;
#endif
}

void WSheet::readTextAt(int idx)
{
    QString text = textAt(idx);
    if( !text.isEmpty()) {
#ifdef Q_OS_WIN32
        _voice.speakForeign( text);
#endif
    }
}

void WSheet::readPause()
{
#ifdef Q_OS_WIN32
    _voice.pause();
#endif
}

void WSheet::readResume()
{
#ifdef Q_OS_WIN32
    _voice.resume();
#endif
}

int WSheet::size()
{
    return (int)_warry.size();
}

bool WSheet::isRememberedAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return false;
    return _warry.at(idx).isRemembered();
}

bool WSheet::isForgottenAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return false;
    return _warry.at(idx).isForgotten();
}

bool WSheet::hasAnswerAt(int idx)
{
    if( idx<0 || idx>= (int)_warry.size())  return false;
    return _warry.at(idx).hasAnswer();
}

bool WSheet::loadFile( const QString& fileName, bool append)
{
    QFile file( fileName);
    if( file.open(QIODevice::ReadOnly)) {
        if( !append) {
            _warry.clear();
        }

        QTextStream in(&file);
        in.setCodec("UTF-8");
        while( !in.atEnd()) {
            QString line = in.readLine();
            if( line.isEmpty())
                continue;

            Word w;
            QStringList fields = line.split("\\\\");
            if( fields.size()>= 1) {  //text
                w.setText( fields.at(0));
            }

            if( fields.size()> 1) {
                QStringList subs = fields.at(1).split(',');
                int  status = -1;
                bool found = false;
                if( subs.size()>=2 ) {
                    bool ok;
                    status = subs.last().trimmed().toInt(&ok);
                    if( ok && subs.last().trimmed().size()<=2)
                        found = true;
                }
                if( found) {
                    w.setAnswer( fields.at(1).left( fields.at(1).lastIndexOf(',')) );  //asnwer
                    w.setStatus( status);      //status
                }
                else
                    w.setAnswer( fields.at(1));
            }
            _warry.push_back( w);

        }

        return true;
    }
    else {
        return false;
    }
}

bool WSheet::saveFile(const QString &fileName)
{
    QFile file( fileName);
    if( file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        WordArray::iterator it;
        for(it= _warry.begin(); it!= _warry.end(); ++it) {
            out<< it->text();
            if( it->hasAnswer())
                out<<"\\\\"<< it->answer();
            out<< ", ";
            out<< QString::number(it->status())<< endl;
        }
        return true;
    }
    return false;
}
