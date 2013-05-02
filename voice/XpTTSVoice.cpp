#include "XpTTSVoice.h"

#include "QDebug"

XpTTSVoice::XpTTSVoice(QObject *parent) :
    QObject(parent)
{
    _tok = false;
    _busy = false;
}

bool XpTTSVoice::initial()
{
    if( _tok)  return true;
    _tok = _activx.setControl("{96749377-3391-11D2-9EE3-00C04F797396}");
    connect(&_activx, SIGNAL(signal(QString, int, void*)),
            this, SLOT(handleActivxSignal(QString,int,void*)));
    return _tok;
}

int XpTTSVoice::speak(QString& txt)
{
    if(!_tok)  return 0;
    if(_busy)  return 0;
    int spk = _activx.dynamicCall("Speak(QString, SpeechVoiceSpeakFlags)", txt ,1).toInt();
    qDebug("spk=%d", spk);
    return spk;
}

int XpTTSVoice::speakJapanese(QString& txt)
{
    QString txtJapn =
            "<lang langid='411'>" + txt + "</lang>";
    return speak(txtJapn);
}

int XpTTSVoice::speakEnglish(QString& txt)
{
    QString txtJapn =
            "<lang langid='409'>" + txt + "</lang>";
    return speak(txtJapn);
}

int XpTTSVoice::speakChinese(QString& txt)
{
    QString txtJapn =
            "<lang langid='804'>" + txt + "</lang>";
    return speak(txtJapn);
}

int XpTTSVoice::speakForeign(QString& txt)
{
    if(!txt.isEmpty()) {
        QChar ch = txt.at(0);
        if( ch.unicode()< 128)
            return speakEnglish( txt);
        else
            return speakJapanese( txt);
    }
    return false;
}

void XpTTSVoice::pause()
{
    // NOT SUGGEST!(may not work)
    _activx.dynamicCall("Pause()");
}

void XpTTSVoice::resume()
{
    // NOT SUGGEST!(may not work)
    _activx.dynamicCall("Resume()");
}

void XpTTSVoice::handleActivxSignal(QString name, int arc , void* argv)
{
    //qDebug() << name;
    if( name=="StartStream(int,QVariant)") {
        _busy = true;
        emit readingStarted();
    }
    else if( name=="EndStream(int,QVariant)") {
        _busy = false;
        emit readingCompleted();
    }
}
